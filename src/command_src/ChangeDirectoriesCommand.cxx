//
// Created by Loaf on 9/4/2025.
//

#include "ChangeDirectoriesCommand.hxx"
#include <iostream>
#include "ANSI.hxx"
#include "ErrorPrinter.hxx"

bool ChangeDirectoriesCommand::validateArguments(arguments const& args)
{
    if (args.empty())
        return true;

    if (args.size() != 1)
    {
        ErrorPrinter::setLastError(
            ansi::withForeground("Usage", ansi::Foreground::RED)
            + ": CHDIR [--RECURSIVE | -r] <source> <destination>");
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
            ErrorPrinter::setLastError("Error changing to home directory: " + std::string(e.what()));
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
            ErrorPrinter::setLastError(
                ansi::withForeground("Error", ansi::Foreground::RED)
                + ": Directory '" + directory.string() + "' does not exist.");
            return CommandResult::PathNotFound;
        }

        if (!fs::is_directory(directory))
        {
            ErrorPrinter::setLastError(
                ansi::withForeground("Error", ansi::Foreground::RED)
                + ": '" + directory.string() + "' is not a directory.");
            return CommandResult::InvalidSyntax;
        }

        fs::current_path(directory);

        fs::path const new_current{ fs::current_path() };

        return CommandResult::Success;
    }
    catch (fs::filesystem_error const& e)
    {
        ErrorPrinter::setLastError("Error changing directory: " + std::string(e.what()));
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