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

REPL::REPL()
{
    m_commands[ReservedWords::DIR] =    [](const std::string& args) { ListDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::CHDIR] =  [](const std::string& args) { ChangeDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::COPY] =   [](const std::string& args) { CopyCommand{}.execute(args); };
    m_commands[ReservedWords::DELETE] = [](const std::string& args) { RemoveCommand{}.execute(args); };

    m_commands[ReservedWords::MKDIR] =  [](const std::string& args) { MakeDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::RMDIR] =  [](const std::string& args) { RemoveDirectoriesCommand{}.execute(args); };
    m_commands[ReservedWords::RENAME] = [](const std::string& args) { RenameCommand{}.execute(args); };

    m_commands[ReservedWords::EXIT] =   [](std::string const&) { std::exit(0); };
    m_commands[ReservedWords::PRINT] =   [](const std::string& args) { std::cout << args << '\n'; };
    m_commands[ReservedWords::CLEAR] =  [](std::string const&) { std::cout << ansi::CLEAR_SCREEN; };
    m_commands[ReservedWords::WHEREAMI] = [](const std::string&) { std::cout << get_dir() << '\n'; };
    m_commands[ReservedWords::PAUSE] =  [](const std::string&) { portable_getch(); };

    m_commands[ReservedWords::SHOW] =   show_command;
    m_commands[ReservedWords::HELP] =   help_command;
    m_commands[ReservedWords::TOUCH] =  touch_command;
    m_commands[ReservedWords::VER] =    version_command;

    m_commands[ReservedWords::CD] =     m_commands[ReservedWords::CHDIR];
    m_commands[ReservedWords::CLS] =    m_commands[ReservedWords::CLEAR];
    m_commands[ReservedWords::DEL] =    m_commands[ReservedWords::DELETE];
    m_commands[ReservedWords::ERASE] =  m_commands[ReservedWords::DELETE];
    m_commands[ReservedWords::MD] =     m_commands[ReservedWords::MKDIR];
    m_commands[ReservedWords::RD] =     m_commands[ReservedWords::RMDIR];
    m_commands[ReservedWords::REN] =    m_commands[ReservedWords::RENAME];
}

bool REPL::operator()(const std::string& str)
{
    auto parsed = parse_args_opt(str);
    if (!parsed) return false;

    auto [commandToken, args] = *parsed;

    if (const auto it = m_commands.find(stringToReservedWord(commandToken)); it != m_commands.end())
    {
        it->second(args);
        return true;
    }
    return false;
}

std::optional<ParsedCmd> REPL::parse_args(std::string_view input)
{
    std::istringstream iss{ std::string(input) };
    std::string cmd;
    if (!(iss >> cmd)) return std::nullopt;
    std::string args;
    std::getline(iss, args);

    auto trim = [](const std::string& s) -> std::string
    {
        const auto first = s.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return {};
        const auto last = s.find_last_not_of(" \t\r\n");
        return s.substr(first, last - first + 1);
    };
    args = trim(args);

    std::ranges::transform(cmd, cmd.begin(),
                           [](const unsigned char c) { return static_cast<char>(std::toupper(c)); });

    parsed_command.command = cmd;

    for ()

    parsed_command = ParsedCmd{ cmd, args };
    return ParsedCmd{ std::move(cmd), std::move(args) };
}

std::string REPL::get_dir()
{
    return fs::current_path().string();
}