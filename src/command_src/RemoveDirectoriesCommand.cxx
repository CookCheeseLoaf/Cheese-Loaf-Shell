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

void RemoveDirectoriesCommand::reportError(std::error_code const& ec)
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

    if (std::error_code ec{}; !fs::remove(dir, ec))
    {
        reportError(ec);
        return CommandResult::Failure;
    }

    return CommandResult::Success;
}

CommandResult RemoveDirectoriesCommand::execute(arguments const& args)
{
    if (!validateArguments(args))
        return CommandResult::InvalidSyntax;

    fs::path const directory{ args[0] };
    return removeDirectory(directory);
}

auto RemoveDirectoriesCommand::clone() const -> std::unique_ptr<Command>
{
    return std::make_unique<RemoveDirectoriesCommand>(*this);
}
