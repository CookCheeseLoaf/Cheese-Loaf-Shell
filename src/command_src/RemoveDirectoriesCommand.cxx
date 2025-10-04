//
// Created by Loaf on 9/4/2025.
//

#include "RemoveDirectoriesCommand.hxx"
#include <iostream>

bool RemoveDirectoriesCommand::validateArguments(arguments const& args)
{
    if (args.size() != 1)
    {
        std::cerr << "Usage: rmdir <directory>\n";
        return false;
    }
    return true;
}

void RemoveDirectoriesCommand::reportError(std::error_code ec)
{
    switch (ec.value())
    {
        case static_cast<int>(std::errc::directory_not_empty):
            std::cerr << "The directory is not empty.\n";
            break;
        case static_cast<int>(std::errc::permission_denied):
            std::cerr << "Access is denied.\n";
            break;
        default:
            std::cerr << "The system cannot find the path specified.\n";
            break;
    }
}


CommandResult RemoveDirectoriesCommand::removeDirectory(fs::path const& dir)
{
    if (!fs::exists(dir))
    {
        std::cerr << "The system cannot find the path specified.\n";
        return CommandResult::PathNotFound;
    }

    if (!fs::is_directory(dir))
    {
        std::cerr << "The specified path is not a directory.\n";
        return CommandResult::InvalidSyntax;
    }

    std::error_code ec;
    if (!fs::remove(dir, ec))
    {
        if (ec == std::errc::directory_not_empty)
            std::cerr << "The directory is not empty.\n";
        else
            std::cerr << "Error removing directory: " << ec.message() << '\n';

        return CommandResult::Failure;
    }

    return CommandResult::Success;
}


CommandResult RemoveDirectoriesCommand::execute(arguments const& args)
{
    if (!validateArguments(args))
        return CommandResult::InvalidSyntax;

    bool recursive = false;
    fs::path directory;

    if (args.size() == 1)
    {
        directory = args[0];
    }
    else
    {
        const auto& option = args[0];
        if (option == "-r" || option == "--recursive")
        {
            recursive = true;
            directory = args[1];
        }
        else
        {
            std::cerr << "Unknown option: " << option << '\n';
            return CommandResult::UnknownOption;
        }
    }

    return removeDirectory(directory);
}

std::unique_ptr<Command> RemoveDirectoriesCommand::clone() const
{
    return std::make_unique<RemoveDirectoriesCommand>(*this);
}
