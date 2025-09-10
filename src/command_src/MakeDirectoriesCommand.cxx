//
// Created by Loaf on 9/4/2025.
//

#include "FileSystemUtils.hxx"
#include "CommandShell.hxx"
#include "MakeDirectoriesCommand.hxx"
#include <iostream>

void MakeDirectoriesCommand::execute(const std::string& args)
{
    if (args.empty())
    {
        std::cerr << "The syntax of the command is incorrect.\n";
        return;
    }

    fs::path dir;

    try
    {
        dir = fs::path(args);
    }
    catch (const fs::filesystem_error&)
    {
        std::cerr << "The filename, directory name, or volume label syntax is incorrect.\n";
        return;
    }

    if (fs::exists(dir))
    {
        std::cerr << "A subdirectory or file already exists.\n";
        return;
    }

    std::error_code ec;
    fs::create_directories(dir, ec);
    if (ec)
    {
        if (ec.value() == static_cast<int>(std::errc::permission_denied))
            std::cerr << "Access is denied.\n";
        else if (ec.value() == static_cast<int>(std::errc::file_exists))
            std::cerr << "A subdirectory or file already exists.\n";
        else
            std::cerr << "The system cannot find the path specified.\n";
    }
}

std::unique_ptr<Command> MakeDirectoriesCommand::clone() const
{
    return std::make_unique<MakeDirectoriesCommand>(*this);
}