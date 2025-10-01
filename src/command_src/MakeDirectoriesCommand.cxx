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

std::optional<MakeDirectoriesCommand::two_paths>
MakeDirectoriesCommand::parse_args(const std::string& args, std::string& err)
{
    const auto tokens = split_quoted_args(args);

    if (tokens.size() != 1 && tokens.size() != 2)
    {
        err = "The syntax of the command is incorrect. Usage: remove [--recursive | -r] <source>";
        return std::nullopt;
    }

    std::string option;
    fs::path source, destination;

    if (tokens.size() == 2)
    {
        option = tokens[0];

        bool valid = false;
        if (option == "-r")
            valid = true;
        else
        {
            std::string opt_upper = option;
            make_upper(opt_upper);
            if (opt_upper == "--RECURSIVE")
                valid = true;
        }

        if (!valid)
        {
            err = "Unknown option: " + option;
            return std::nullopt;
        }

        source = fs::path(tokens[1]);
    }
    else
    {
        option.clear();
        source = fs::path(tokens[0]);
    }

    return std::make_pair(option, source);
}

std::vector<std::string> RemoveCommand::split_quoted_args(const std::string& s)
{
    std::vector<std::string> tokens;
    std::string cur;
    bool inq = false;
    for (const char ch : s)
    {
        if (ch == '"') { inq = !inq; continue; }
        if (std::isspace(static_cast<unsigned char>(ch)) && !inq)
        {
            if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
        }
        else cur.push_back(ch);
    }
    if (!cur.empty()) tokens.push_back(cur);
    return tokens;
}