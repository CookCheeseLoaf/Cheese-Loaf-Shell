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
        << std::string(8 - wordStr.size(), ' ')
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

void version_command(const std::string&)
{
    char versionBuf[64];
    std::snprintf(versionBuf, sizeof(versionBuf), "%d.%d.%d",
                  REPL::MAJOR, REPL::MINOR, REPL::PATCH);
    std::string colored = ansi::withForeground(versionBuf, ansi::Foreground::RED);

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
}