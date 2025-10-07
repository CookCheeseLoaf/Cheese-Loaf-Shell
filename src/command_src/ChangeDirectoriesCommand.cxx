//
// Created by Loaf on 9/4/2025.
//

#include "ChangeDirectoriesCommand.hxx"
#include <iostream>

bool ChangeDirectoriesCommand::validateArguments(arguments const& args)
{
    if (args.empty())
        return true;

    if (args.size() != 1)
    {
        std::cerr << "The syntax of the command is incorrect. Usage: chdir <directory>\n";
        return false;
    }

    return true;
}

CommandResult ChangeDirectoriesCommand::execute(arguments const& args)
{
    if (!validateArguments(args))
        return CommandResult::InvalidSyntax;

    if (args.empty())
    {
        try
        {
            std::string const home{ FileSystemUtils::get_home_directory() };
            fs::current_path(home);
            return CommandResult::Success;
        }
        catch (fs::filesystem_error const& e)
        {
            std::cerr << "Error changing to home directory: " << e.what() << '\n';
            return CommandResult::Failure;
        }
    }

    fs::path directory{ args[0] };

    try
    {
        if (directory.is_relative())
        {
            directory = fs::absolute(directory);
        }

        directory = fs::weakly_canonical(directory);

        if (!fs::exists(directory))
        {
            std::cerr << "Error: Directory '" << directory.string() << "' does not exist.\n";
            return CommandResult::PathNotFound;
        }

        if (!fs::is_directory(directory))
        {
            std::cerr << "Error: '" << directory.string() << "' is not a directory.\n";
            return CommandResult::InvalidSyntax;
        }

        fs::current_path(directory);

        fs::path const new_current{ fs::current_path() };

        return CommandResult::Success;
    }
    catch (fs::filesystem_error const& e)
    {
        std::cerr << "Error changing directory: " << e.what() << '\n';
        return CommandResult::Failure;
    }
}

auto ChangeDirectoriesCommand::clone() const -> std::unique_ptr<Command>
{
    return std::make_unique<ChangeDirectoriesCommand>();
}

fs::path ChangeDirectoriesCommand::get_current_directory()
{
    return fs::current_path();
}