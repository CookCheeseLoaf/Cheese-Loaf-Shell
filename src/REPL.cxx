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

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
static int portable_getch(const std::string&)
{
    std::cout << "Press any key to continue." << '\n';
    return _getch();
}
#else
#include <unistd.h>
#include <termios.h>
#include <cstdio>

inline int portable_getch(const std::string&)
{
    std::cout << "Press any key to continue." << '\n';
    struct termios oldt, newt;
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) return EOF;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt) != 0) return EOF;
    unsigned char ch{};
    ssize_t n{ read(STDIN_FILENO, &ch, 1) };
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (n <= 0) return EOF;
    return static_cast<int>(ch);
}
#endif


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
    m_commands[ReservedWords::VER] =    [](const std::string&) { std::cout << "Cheese Loaf Shell (version: 0.01)\n"; };

    m_commands[ReservedWords::SHOW] =   show_command;
    m_commands[ReservedWords::PAUSE] =  portable_getch;
    m_commands[ReservedWords::HELP] =   help_command;
    m_commands[ReservedWords::TOUCH] =  touch_command;

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
    auto [valid, commandToken, args]{ parse_args(str) };
    if (!valid) return false;

    if (const auto it{ m_commands.find(stringToReservedWord(commandToken)) }; it != m_commands.end())
    {
        it->second(args);
        return true;
    }
    return false;
}

std::tuple<bool, std::string, std::string> REPL::parse_args(const std::string& str)
{
    std::istringstream iss{ str };
    std::string commandToken;

    if (!(iss >> commandToken))
        return { false, {}, {} };

    std::string args;
    std::getline(iss, args);
    if (const size_t firstNonSpace{ args.find_first_not_of(" \t") }; firstNonSpace != std::string::npos)
        args = args.substr(firstNonSpace);
    else
        args.clear();

    std::transform(commandToken.begin(), commandToken.end(), commandToken.begin(), toupper);
    return { true, commandToken, args };
}

std::string REPL::get_dir()
{
    return fs::current_path().string();
}