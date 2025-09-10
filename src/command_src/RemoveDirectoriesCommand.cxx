//
// Created by Loaf on 9/4/2025.
//

#include "RemoveDirectoriesCommand.hxx"
#include "FileSystemUtils.hxx"
#include <iostream>

void RemoveDirectoriesCommand::execute(const std::string& args)
{
    if (args.empty())
    {
        std::cerr << "The syntax of the command is incorrect.\n";
        return;
    }

    fs::path dir;

    try
    {
        dir = fs::u8path(args);
    }
    catch (const fs::filesystem_error&)
    {
        std::cerr << "The filename, directory name, or volume label syntax is incorrect.\n";
        return;
    }

    if (!fs::exists(dir))
    {
        std::cerr << "The system cannot find the path specified.\n";
        return;
    }

    if (!fs::is_directory(dir))
    {
        std::cerr << "The specified path is not a directory.\n";
        return;
    }

    if (std::error_code ec;
        !fs::remove(dir, ec))
    {
        if (ec)
        {
            if (ec.value() == static_cast<int>(std::errc::directory_not_empty))
                std::cerr << "The directory is not empty.\n";
            else
                std::cerr << "Error removing directory: " << ec.message() << '\n';
        }
        else
            std::cerr << "Failed to remove the directory.\n";
    }
}

std::unique_ptr<Command> RemoveDirectoriesCommand::clone() const
{
    return std::make_unique<RemoveDirectoriesCommand>(*this);
}