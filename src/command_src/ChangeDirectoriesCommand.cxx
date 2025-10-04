//
// Created by Loaf on 9/4/2025.
//

#include "ChangeDirectoriesCommand.hxx"
#include <iostream>


bool
ChangeDirectoriesCommand::validateArguments(arguments const& args)
{
    if (args.size() != 1)
    {
        std::cerr << "The syntax of the command is incorrect. Usage: chdir <directories>\n";
        return false;
    }
    return true;
}

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

CommandResult
ChangeDirectoriesCommand::execute(arguments const& args)
{
    if (!validateArguments(args))
        return CommandResult::InvalidSyntax;

    fs::path directory = args[0];

    try
    {
        fs::current_path(directory);
        current_directory = fs::current_path();
    }
    catch (const fs::filesystem_error&)
    {
        std::cerr << "Error changing directory: " << directory << " doesn't exists.\n";
        return CommandResult::CommandFailed;
    }

    return CommandResult::Success;
}

std::unique_ptr<Command>
ChangeDirectoriesCommand::clone() const
{
    return std::make_unique<ChangeDirectoriesCommand>(*this);
}

fs::path
ChangeDirectoriesCommand::get_current_directory() const
{
    return current_directory;
}