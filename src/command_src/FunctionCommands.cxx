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

    std::ifstream file{ args[0] };
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
        "%d.%d.%d",
        REPL::MAJOR,
        REPL::MINOR,
        REPL::PATCH);

    std::string const colored{ ansi::withForeground(versionBuf, ansi::Foreground::RED) };

    std::cout << R"(
         .-'''-.
.---.   '   _    \                                                                      .---..---.
|   | /   /` '.   \                                        .              __.....__     |   ||   |
|   |.   |     \  '            _.._                      .'|          .-''         '.   |   ||   |
|   ||   '      |  '         .' .._|                    <  |         /     .-''"'-.  `. |   ||   |
|   |\    \     / /  __      | '                         | |        /     /________\   \|   ||   |
|   | `.   ` ..' /.:--.'.  __| |__                   _   | | .'''-. |                  ||   ||   |
|   |    '-...-'`/ |   \ ||__   __|                .' |  | |/.'''. \\    .-------------'|   ||   |
|   |            `" __ | |   | |                  .   | /|  /    | | \    '-.____...---.|   ||   |
|   |             .'.''| |   | |                .'.'| |//| |     | |  `.             .' |   ||   |
'---'            / /   | |_  | |              .'.'.-'  / | |     | |    `''-...... -'   '---''---'
                 \ \._,\ '/  | |              .'   \_.'  | '.    | '.
                  `--'  `"   |_|                         '---'   '---'                            )" << '\n';

    std::cout << "Loaf Shell, version < " << colored << " >\n"
              << "Major:\t" << REPL::MAJOR << '\n'
              << "Minor:\t" << REPL::MINOR << '\n'
              << "Patch:\t" << REPL::PATCH << '\n';

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
