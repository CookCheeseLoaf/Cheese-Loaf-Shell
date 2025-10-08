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
#include "REPL.hxx"

CommandResult help_command(arguments const&)
{
    for (std::uint8_t i{ 0 }; i < static_cast<std::uint8_t>(ReservedWords::UNKNOWN); ++i)
    {
        auto const w{ static_cast<ReservedWords>(i) };
        std::string_view const wordStr{ reservedWordToString(w) };

        std::cout << '<'
                  << ansi::foreground(ansi::Foreground::BLUE)
                  << wordStr
                  << ansi::RESET
                  << std::string(8 - wordStr.size(), ' ')
                  << "> "
                  << informationAboutReservedWords(w)
                  << '\n';
    }

    return CommandResult::Success;
}

CommandResult touch_command(arguments const& args)
{
    if (args.size() != 1)
    {
        std::cerr << "The syntax of the command is incorrect. Usage: TOUCH <file>\n";
        return CommandResult::InvalidSyntax;
    }

    fs::path const filePath{ args[0] };

    if (fs::exists(filePath))
        return CommandResult::AlreadyExists;

    if (std::ofstream{ filePath })
        return CommandResult::Success;

    return CommandResult::PermissionDenied;
}

CommandResult show_command(arguments const& args)
{
    if (args.size() != 1)
    {
        std::cerr << "The syntax of the command is incorrect. Usage: SHOW <file>\n";
        return CommandResult::InvalidSyntax;
    }

    std::ifstream const file{ args[0] };
    if (!file)
    {
        std::cerr << "Error: Unable to open file '" << args[0] << "' for reading.\n";
        return CommandResult::PathNotFound;
    }

    std::cout << file.rdbuf() << '\n';
    return CommandResult::Success;
}

CommandResult version_command(arguments const&)
{
    char versionBuf[64];
    std::snprintf(
        versionBuf,
        sizeof(versionBuf),
        "%s.%s.%s",
        ansi::withForeground(std::to_string(REPL::MAJOR), ansi::Foreground::RED).c_str(),
        ansi::withForeground(std::to_string(REPL::MINOR), ansi::Foreground::BLUE).c_str(),
        ansi::withForeground(std::to_string(REPL::PATCH), ansi::Foreground::CYAN).c_str());

    std::cout << R"(
        d8b                              d8b
        ?88                              88P              d8P
         88b                            d88            d888888P
 d8888b  888888b  d8888b  d8888b d8888b 888   d888b8b    ?88'   d8888b
d8P' `P  88P `?8bd8P' ?88d8P' `Pd8P' ?88?88  d8P' ?88    88P   d8b_,dP
88b     d88   88P88b  d8888b    88b  d88 88b 88b  ,88b   88b   88b
`?888P'd88'   88b`?8888P'`?888P'`?8888P'  88b`?88P'`88b  `?8b  `?888P'



         d8b              d8b  d8b
         ?88              88P  88P
          88b            d88  d88
 .d888b,  888888b  d8888b888  888
 ?8b,     88P `?8bd8b_,dP?88  ?88
   `?8b  d88   88P88b     88b  88b
`?888P' d88'   88b`?888P'  88b  88b
)" << '\n';

    std::cout << "Chocolate Shell, version < " << versionBuf << " >\n"
              << ansi::withForeground("Major:", ansi::Foreground::RED) << '\t' << REPL::MAJOR << '\n'
              << ansi::withForeground("Minor:", ansi::Foreground::BLUE) << '\t' << REPL::MINOR << '\n'
              << ansi::withForeground("Patch:", ansi::Foreground::CYAN) << '\t' << REPL::PATCH << '\n';

    return CommandResult::Success;
}

CommandResult print_command(arguments const& args)
{
    for (auto it{ args.begin() }; it != args.end(); ++it)
    {
        std::cout << *it;
        if (std::next(it) != args.end())
            std::cout << ' ';
    }

    std::cout << '\n';
    return CommandResult::Success;
}
