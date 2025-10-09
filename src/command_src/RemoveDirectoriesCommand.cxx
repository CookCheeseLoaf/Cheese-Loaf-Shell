#include "RemoveDirectoriesCommand.hxx"
#include <iostream>

#include "ANSI.hxx"
#include "ErrorPrinter.hxx"

bool RemoveDirectoriesCommand::validateArguments(arguments const& args)
{
    if (args.size() != 1)
    {
        ErrorPrinter::setLastError(ansi::withForeground("Usage", ansi::Foreground::RED)
        + ": RMDIR <directory>");
        return false;
    }
    return true;
}

void RemoveDirectoriesCommand::reportError(std::error_code const& ec)
{
    switch (ec.value())
    {
        case static_cast<int>(std::errc::directory_not_empty):
            ErrorPrinter::setLastError("The directory is not empty.");
            break;
        case static_cast<int>(std::errc::permission_denied):
            ErrorPrinter::setLastError("Access is denied.");
            break;
        default:
            ErrorPrinter::setLastError("The system cannot find the path specified.");
            break;
    }
}

CommandResult RemoveDirectoriesCommand::removeDirectory(fs::path const& dir)
{
    if (!fs::exists(dir))
    {
        ErrorPrinter::setLastError("The system cannot find the path specified.");
        return CommandResult::PathNotFound;
    }

    if (!fs::is_directory(dir))
    {
        ErrorPrinter::setLastError("The specified path is not a directory.");
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
