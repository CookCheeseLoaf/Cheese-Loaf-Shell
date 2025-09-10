//
// Created by Loaf on 9/5/2025.
//

#include "FunctionCommands.hxx"
#include <cstdint>
#include "ReservedWords.hxx"
#include <iostream>
#include "ANSI.hxx"
#include "FileSystemUtils.hxx"
#include <fstream>

void help_command(std::string const&)
{
    for (uint8_t i = 0; i < static_cast<uint8_t>(ReservedWords::UNKNOWN); ++i)
    {
        const auto w{ static_cast<ReservedWords>(i) };
        std::string_view wordStr{ reservedWordToString(w) };
        std::cout << '<'
        << ansi::foreground(ansi::Foreground::BLUE)
        << wordStr
        << ansi::RESET
        << std::string(6 - wordStr.size(), ' ')
        << "> "
        << informationAboutReservedWords(w)
        << '\n';
    }
}

void touch_command(const std::string& args)
{
    if (args.empty())
    {
        std::cerr << "The syntax of the command is incorrect. Usage: TOUCH <file>\n";
        return;
    }

    if (fs::exists(fs::path{ args }))
        return;

    std::ofstream{ args };
}

void show_command(const std::string& args)
{
    if (args.empty())
    {
        std::cerr << "The syntax of the command is incorrect. Usage: SHOW <file>\n";
        return;
    }

    std::cout << std::ifstream{ args }.rdbuf() << '\n';
}