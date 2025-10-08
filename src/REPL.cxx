//
// Created by Loaf on 9/4/2025.
//

#include <iostream>
#include <map>
#include <functional>
#include <algorithm>

#include "ANSI.hxx"
#include "REPL.hxx"
#include <sstream>

#include "FunctionCommands.hxx"
#include "MakeDirectoriesCommand.hxx"
#include "RemoveDirectoriesCommand.hxx"
#include "ReservedWords.hxx"
#include "RenameCommand.hxx"
#include "CopyCommand.hxx"
#include "RemoveCommand.hxx"
#include "ChangeDirectoriesCommand.hxx"
#include "ListDirectoriesCommand.hxx"
#include "portable_getch.h"
#include "ErrorPrinter.hxx"

static std::string to_upper(std::string str)
{
    std::ranges::transform(str, str.begin(),
        [](unsigned char const c) { return static_cast<unsigned char>(std::toupper(c)); });
    return str;
}

REPL::REPL()
{
    m_commands[ReservedWords::DIR]    = [](auto&& args) { return ListDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::CHDIR]  = [](auto&& args) { return ChangeDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::COPY]   = [](auto&& args) { return CopyCommand{}.execute(args); };
    m_commands[ReservedWords::DELETE] = [](auto&& args) { return RemoveCommand{}.execute(args); };
    m_commands[ReservedWords::MKDIR]  = [](auto&& args) { return MakeDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::RMDIR]  = [](auto&& args) { return RemoveDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::RENAME] = [](auto&& args) { return RenameCommand{}.execute(args); };

    m_commands[ReservedWords::EXIT]     = [](auto&&) { return CommandResult::Exit; };
    m_commands[ReservedWords::CLEAR]    = [](auto&&) { std::cout << ansi::CLEAR_SCREEN; return CommandResult::Success; };
    m_commands[ReservedWords::WHEREAMI] = [](auto&&) { std::cout << get_dir() << '\n'; return CommandResult::Success; };
    m_commands[ReservedWords::PAUSE]    = [](auto&&) { portable_getch(); return CommandResult::Success; };

    m_commands[ReservedWords::SHOW]  = show_command;
    m_commands[ReservedWords::HELP]  = help_command;
    m_commands[ReservedWords::TOUCH] = touch_command;
    m_commands[ReservedWords::VER]   = version_command;
    m_commands[ReservedWords::PRINT] = print_command;

    m_commands[ReservedWords::CD]     = m_commands[ReservedWords::CHDIR];
    m_commands[ReservedWords::CLS]    = m_commands[ReservedWords::CLEAR];
    m_commands[ReservedWords::DEL]    = m_commands[ReservedWords::DELETE];
    m_commands[ReservedWords::ERASE]  = m_commands[ReservedWords::DELETE];
    m_commands[ReservedWords::MD]     = m_commands[ReservedWords::MKDIR];
    m_commands[ReservedWords::RD]     = m_commands[ReservedWords::RMDIR];
    m_commands[ReservedWords::REN]    = m_commands[ReservedWords::RENAME];
}

bool REPL::operator()(std::string const& input)
{
    parsed_command = parse_args(input);

    if (is_it_the_command_executable(parsed_command.command))
    {
        std::string command{ parsed_command.command };

        if (command.find("./") == 0 || command.find(".\\") == 0)
            command.erase(0, 2);

        fs::path const cmd_path{ command };
        std::string filename{ cmd_path.filename().string() };

        if (fs::exists(cmd_path)
            && fs::is_regular_file(cmd_path)
            && FileSystemUtils::is_executable(cmd_path.string()))
        {
            if (FileSystemUtils::execute_executable(cmd_path.string()))
                return true;
             return false;
        }
    }

    auto const it{ m_commands.find(stringToReservedWord(parsed_command.command)) };
    if (it == m_commands.end())
    {
        std::cerr << "Error: Unknown command '" << parsed_command.command << "'\n";
        return false;
    }

    std::optional const result{ it->second(parsed_command.args) };

    if (!result.has_value())
    {
        print_formatted_error(
            ansi::withForeground("Error", ansi::Foreground::RED)
            + ": Command '" + parsed_command.command + "' failed to execute\n");
        return false;
    }

    switch (*result)
    {
        case CommandResult::Exit:
            std::exit(EXIT_SUCCESS);

        case CommandResult::Success:
            return true;

        case CommandResult::Failure:
        case CommandResult::UnknownError:
        case CommandResult::InvalidSyntax:
        case CommandResult::AlreadyExists:
        case CommandResult::PermissionDenied:
        case CommandResult::PathNotFound:
        case CommandResult::UnknownOption:
        case CommandResult::AccessDenied:
            std::cerr << "Error: " << to_string(*result)
                      << " - command '" << parsed_command.command << "'\n";
            return false;

        default:
            std::cerr << "Error: Unexpected result from command '"
                      << parsed_command.command << "'\n";
            return false;
    }
}

ParsedCmd REPL::parse_args(std::string const& input)
{
    std::size_t const space{ input.find(' ') };
    std::string cmd{ input.substr(0, space) };
    std::string args_str{ space == std::string::npos ? "" : input.substr(space + 1) };

    if (std::size_t const end{ args_str.find_last_not_of(" \t\r\n") },
        start{ args_str.find_first_not_of(" \t\r\n") };
        start != std::string::npos && end != std::string::npos)
        args_str = args_str.substr(start, end - start + 1);
    else
        args_str.clear();

    if (!(cmd.find('/') != std::string::npos ||
          cmd.find('\\') != std::string::npos ||
          (!cmd.empty() && cmd.front() == '.')))
        cmd = to_upper(std::move(cmd));

    return { cmd, split_quoted_args(args_str) };
}

std::vector<std::string> REPL::split_quoted_args(std::string const& input)
{
    std::vector<std::string> tokens{};
    std::string current{};
    bool in_quotes{ false };

    for (char const c : input)
    {
        if (c == '"')
        {
            in_quotes = !in_quotes;
        }
        else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes)
        {
            if (!current.empty())
            {
                tokens.push_back(std::move(current));
                current.clear();
            }
        }
        else
        {
            current += c;
        }
    }

    if (!current.empty())
    {
        tokens.push_back(std::move(current));
    }

    return tokens;
}

std::string REPL::get_dir()
{
    return fs::current_path().string();
}

bool REPL::is_it_the_command_executable(std::string_view const cmd)
{
    return cmd.find('/') != std::string_view::npos ||
           cmd.find('\\') != std::string_view::npos ||
           (!cmd.empty() && cmd.front() == '.');
}

auto REPL::getCommands() const -> std::map<ReservedWords, std::function<CommandResult(arguments const&)>> const&
{
    return m_commands;
}