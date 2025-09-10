//
// Created by Loaf on 9/4/2025.
//

#include "ChangeDirectoriesCommand.hxx"
#include <iostream>

ChangeDirectoriesCommand::ChangeDirectoriesCommand()
{
    try
    {
        current_directory = FileSystemUtils::get_home_directory();
        fs::current_path(current_directory);
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error setting initial directory: " << e.what() << '\n';
        current_directory = ".";
    }
}

void ChangeDirectoriesCommand::execute(const std::string& args)
{
    if (args.empty())
    {
        std::cerr << "The syntax of the command is incorrect. Usage: chdir <directories>\n";
        return;
    }

    try
    {
        fs::current_path(args);
        current_directory = fs::current_path();
    }
    catch (const fs::filesystem_error&)
    {
        std::cerr << "Error changing directory: " << args << " doesn't exists.\n";
    }
}

std::unique_ptr<Command> ChangeDirectoriesCommand::clone() const
{
    return std::make_unique<ChangeDirectoriesCommand>(*this);
}

fs::path ChangeDirectoriesCommand::get_current_directory() const
{
    return current_directory;
}