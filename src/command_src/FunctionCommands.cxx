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
        ansi::withForeground(std::to_string(REPL::MINOR), ansi::Foreground::YELLOW).c_str(),
        ansi::withForeground(std::to_string(REPL::PATCH), ansi::Foreground::BLUE).c_str());

    std::cout << R"(
__/\\\_________________________________________________/\\\\\_______________/\\\\\\\\\\\____/\\\_________________________/\\\\\\_____/\\\\\\____
 _\/\\\_______________________________________________/\\\///______________/\\\/////////\\\_\/\\\________________________\////\\\____\////\\\____
  _\/\\\______________________________________________/\\\_________________\//\\\______\///__\/\\\___________________________\/\\\_______\/\\\____
   _\/\\\_________________/\\\\\_____/\\\\\\\\\_____/\\\\\\\\\_______________\////\\\_________\/\\\_____________/\\\\\\\\_____\/\\\_______\/\\\____
    _\/\\\_______________/\\\///\\\__\////////\\\___\////\\\//___________________\////\\\______\/\\\\\\\\\\____/\\\/////\\\____\/\\\_______\/\\\____
     _\/\\\______________/\\\__\//\\\___/\\\\\\\\\\_____\/\\\________________________\////\\\___\/\\\/////\\\__/\\\\\\\\\\\_____\/\\\_______\/\\\____
      _\/\\\_____________\//\\\__/\\\___/\\\/////\\\_____\/\\\_________________/\\\______\//\\\__\/\\\___\/\\\_\//\\///////______\/\\\_______\/\\\____
       _\/\\\\\\\\\\\\\\\__\///\\\\\/___\//\\\\\\\\/\\____\/\\\________________\///\\\\\\\\\\\/___\/\\\___\/\\\__\//\\\\\\\\\\__/\\\\\\\\\__/\\\\\\\\\_
        _\///////////////_____\/////______\////////\//_____\///___________________\///////////_____\///____\///____\//////////__\/////////__\/////////__
)" << '\n';

    std::cout << "Loaf Shell, version < " << versionBuf << " >\n"
              << ansi::withForeground("Major:", ansi::Foreground::RED) << '\t' << REPL::MAJOR << '\n'
              << ansi::withForeground("Minor:", ansi::Foreground::YELLOW) << '\t' << REPL::MINOR << '\n'
              << ansi::withForeground("Patch:", ansi::Foreground::BLUE) << '\t' << REPL::PATCH << '\n';

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
