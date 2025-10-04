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
#include "using_arguments.h"

REPL::REPL()
{
    m_commands[ReservedWords::DIR] = [](arguments const &args) { ListDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::CHDIR] = [](arguments const &args) { ChangeDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::COPY] = [](arguments const &args) { CopyCommand{}.execute(args); };
    m_commands[ReservedWords::DELETE] = [](arguments const &args) { RemoveCommand{}.execute(args); };

    m_commands[ReservedWords::MKDIR] = [](arguments const &args) { MakeDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::RMDIR] = [](arguments const &args) { RemoveDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::RENAME] = [](arguments const &args) { RenameCommand{}.execute(args); };

    m_commands[ReservedWords::EXIT] = [](arguments const &) { std::exit(0); };
    m_commands[ReservedWords::CLEAR] = [](arguments const &) { std::cout << ansi::CLEAR_SCREEN; };
    m_commands[ReservedWords::WHEREAMI] = [](arguments const &) { std::cout << get_dir() << '\n'; };
    m_commands[ReservedWords::PAUSE] = [](arguments const &) { portable_getch(); };

    m_commands[ReservedWords::SHOW] = show_command;
    m_commands[ReservedWords::HELP] = help_command;
    m_commands[ReservedWords::TOUCH] = touch_command;
    m_commands[ReservedWords::VER] = version_command;
    m_commands[ReservedWords::PRINT] = print_command;

    m_commands[ReservedWords::CD] = m_commands[ReservedWords::CHDIR];
    m_commands[ReservedWords::CLS] = m_commands[ReservedWords::CLEAR];
    m_commands[ReservedWords::DEL] = m_commands[ReservedWords::DELETE];
    m_commands[ReservedWords::ERASE] = m_commands[ReservedWords::DELETE];
    m_commands[ReservedWords::MD] = m_commands[ReservedWords::MKDIR];
    m_commands[ReservedWords::RD] = m_commands[ReservedWords::RMDIR];
    m_commands[ReservedWords::REN] = m_commands[ReservedWords::RENAME];
}

bool REPL::operator()(std::string_view str)
{
    parsed_command = parse_args(str);

    ReservedWords rw = stringToReservedWord(parsed_command.command);

    if (const auto it = m_commands.find(rw); it != m_commands.end())
    {
        it->second(parsed_command.args);
        return true;
    }
    return false;
}

ParsedCmd REPL::parse_args(std::string_view input)
{
    size_t space = input.find(' ');
    std::string cmd = std::string(input.substr(0, space));
    std::string_view args_view = space == std::string_view::npos ? "" : input.substr(space + 1);

    auto trim_view = [](const std::string_view s) -> std::string_view
    {
        const size_t first = s.find_first_not_of(" \t\r\n");
        if (first == std::string_view::npos) return {};
        const size_t last = s.find_last_not_of(" \t\r\n");
        return s.substr(first, last - first + 1);
    };
    args_view = trim_view(args_view);

    std::ranges::transform(cmd, cmd.begin(),
                           [](const unsigned char c) { return static_cast<char>(std::toupper(c)); });

    return { cmd, split_quoted_args(args_view) };
}

std::vector<std::string_view> REPL::split_quoted_args(std::string_view input) {
    std::vector<std::string_view> tokens;
    size_t start = 0;
    bool in_quotes = false;

    for (size_t i = 0; i <= input.size(); ++i) {
        if (i == input.size() || (!in_quotes && std::isspace(static_cast<unsigned char>(input[i]))))
        {
            if (start < i)
                tokens.push_back(input.substr(start, i - start));
            start = i + 1;
        }
        else if (input[i] == '"')
        {
            if (!in_quotes)
            {
                start = i + 1;
                in_quotes = true;
            }
            else
            {
                tokens.push_back(input.substr(start, i - start));
                start = i + 1;
                in_quotes = false;
            }
        }
    }

    return tokens;
}


std::string REPL::get_dir()
{
    return fs::current_path().string();
}