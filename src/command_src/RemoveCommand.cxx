//
// Created by Loaf on 9/4/2025.
//

#include "RemoveCommand.hxx"
#include <iostream>
#include "StringUtils.hxx"

void RemoveCommand::execute(const std::string& args)
{
    std::string err;
    auto p = parse_args(args, err);
    if (!p)
    {
        std::cerr << err << '\n';
        return;
    }
    auto [option, source] = *p;

    if (fs::is_directory(source))
    {
        std::cerr << "The specified path is not a file.\n";
        return;
    }

    if (!fs::exists(source))
    {
        std::cerr << "The system cannot find the path specified: " << source << '\n';
        return;
    }

    try
    {
        std::string opt_upper = option;
        make_upper(opt_upper);
        if (option == "-r" || opt_upper == "--RECURSIVE")
            fs::remove_all(source);
        else
            fs::remove(source);
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error removing file '" << source << "': " << e.what() << '\n';
    }
}

std::unique_ptr<Command> RemoveCommand::clone() const
{
    return std::make_unique<RemoveCommand>(*this);
}

std::optional<std::pair<std::string, fs::path>>
RemoveCommand::parse_args(const std::string& args, std::string& err)
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