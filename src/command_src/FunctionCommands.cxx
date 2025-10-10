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

#include "ErrorPrinter.hxx"
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
        ErrorPrinter::setLastError(ansi::withForeground("Usage", ansi::Foreground::RED) + ": TOUCH <file>");
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
        ErrorPrinter::setLastError(ansi::withForeground("Usage", ansi::Foreground::RED) + ": SHOW <file>");
        return CommandResult::InvalidSyntax;
    }

    std::ifstream const file{ args[0] };
    if (!file)
    {
        ErrorPrinter::setLastError(ansi::withForeground("Error", ansi::Foreground::RED) + ": Unable to open file '" + args[0] + "' for reading.");
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
        ansi::withForeground(std::to_string(REPL::PATCH), ansi::Foreground::YELLOW).c_str());

    std::cout << R"(
 d8b                      ,d8888b             d8b              d8b  d8b
 88P                      88P'                ?88              88P  88P
d88                    d888888P                88b            d88  d88
888   d8888b  d888b8b    ?88'         .d888b,  888888b  d8888b888  888
?88  d8P' ?88d8P' ?88    88P          ?8b,     88P `?8bd8b_,dP?88  ?88
 88b 88b  d8888b  ,88b  d88             `?8b  d88   88P88b     88b  88b
  88b`?8888P'`?88P'`88bd88'          `?888P' d88'   88b`?888P'  88b  88b

)" << '\n';

    std::cout << "Loaf Shell, version < " << versionBuf << " >\n"
              << ansi::withForeground("Major:", ansi::Foreground::RED) << '\t' << REPL::MAJOR << '\n'
              << ansi::withForeground("Minor:", ansi::Foreground::BLUE) << '\t' << REPL::MINOR << '\n'
              << ansi::withForeground("Patch:", ansi::Foreground::YELLOW) << '\t' << REPL::PATCH << '\n';

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
