//
// Created by Loaf on 9/4/2025.
//

#include "MakeDirectoriesCommand.hxx"
#include "FileSystemUtils.hxx"
#include <iostream>
#include <system_error>

bool MakeDirectoriesCommand::validateArguments(arguments const& args)
{
    if (args.size() != 1)
    {
        std::cerr << "The syntax of the command is incorrect. Usage: mkdir <directory>\n";
        return false;
    }
    return true;
}

CommandResult MakeDirectoriesCommand::createDirectory(std::string_view const arg)
{
    if (arg.empty())
    {
        std::cerr << "The syntax of the command is incorrect. Usage: mkdir <directory>\n";
        return CommandResult::InvalidSyntax;
    }

    fs::path directoryPath{ arg };

    std::error_code ec{};
    fs::create_directories(directoryPath, ec);

    if (ec)
    {
        reportError(ec);

        if (ec.value() == static_cast<int>(std::errc::file_exists))
            return CommandResult::AlreadyExists;

        if (ec.value() == static_cast<int>(std::errc::permission_denied))
            return CommandResult::AccessDenied;

        return CommandResult::PathNotFound;
    }

    return CommandResult::Success;
}

void MakeDirectoriesCommand::reportError(std::error_code const& ec)
{
    switch (ec.value())
    {
        case static_cast<int>(std::errc::permission_denied):
            std::cerr << "Access is denied.\n";
            break;
        case static_cast<int>(std::errc::file_exists):
            std::cerr << "A subdirectory or file already exists.\n";
            break;
        default:
            std::cerr << "The system cannot find the path specified.\n";
            break;
    }
}

CommandResult MakeDirectoriesCommand::execute(arguments const& args)
{
    if (!validateArguments(args))
        return CommandResult::InvalidSyntax;

    return createDirectory(args[0]);
}

auto MakeDirectoriesCommand::clone() const -> std::unique_ptr<Command>
{
    return std::make_unique<MakeDirectoriesCommand>(*this);
}
