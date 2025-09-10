//
// Created by Loaf on 9/4/2025.
//

#include "ListDirectoriesCommand.hxx"
#include <iostream>
#include "ANSI.hxx"


void ListDirectoriesCommand::execute(const std::string& args)
{
    const fs::path dir{ args.empty() ? "." : args };

    try
    {
        for (const auto& entry : fs::directory_iterator{ dir })
            PrintEntry(entry);
    }
    catch (const fs::filesystem_error&)
    {
        std::cerr << "The system cannot find the path specified.\n";
    }
}

void ListDirectoriesCommand::PrintEntry(const fs::directory_entry& entry)
{
    const std::string name{ entry.path().filename().string() };
    const std::string color{ DetermineColor(entry) };

    std::cout << color << name << (color.empty() ? "" : ansi::RESET) << '\n';
}

std::string ListDirectoriesCommand::DetermineColor(const fs::directory_entry& entry)
{
    if (entry.is_directory()) return ansi::background(ansi::Background::BLUE);
    if (entry.is_character_file()) return ansi::foreground(ansi::Foreground::YELLOW);
    if (entry.is_block_file()) return ansi::foreground(ansi::Foreground::BLUE);
    if (entry.is_other()) return ansi::foreground(ansi::Foreground::RED);
    if (entry.is_socket()) return ansi::foreground(ansi::Foreground::MAGENTA);
    if (entry.is_symlink()) return ansi::foreground(ansi::Foreground::CYAN);

    if (entry.is_regular_file() && FileSystemUtils::is_executable(entry))
        return ansi::foreground(ansi::Foreground::GREEN);

    return "";
}

std::unique_ptr<Command> ListDirectoriesCommand::clone() const
{
    return std::make_unique<ListDirectoriesCommand>(*this);
}