//
// Created by Loaf on 9/4/2025.
//

#include "CopyCommand.hxx"
#include <iostream>
#include <algorithm>

#include "StringUtils.hxx"

void CopyCommand::execute(const std::string& args)
{
    std::string err;
    auto p = parse_args(args, err);
    if (!p)
    {
        std::cerr << err << '\n';
        return;
    }
    auto [option, source, destination] = *p;

    if (!fs::exists(source))
    {
        std::cerr << "The system cannot find the path specified: " << source << '\n';
        return;
    }

    try
    {
        const std::string opt_upper = option;
        make_upper(option);
        if (option == "-r" || opt_upper == "--RECURSIVE")
            fs::copy(source, destination,
                     fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        else
            fs::copy(source, destination, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error renaming file: " << e.what() << '\n';
    }
}

std::unique_ptr<Command> CopyCommand::clone() const
{
    return std::make_unique<CopyCommand>(*this);
}

std::optional<std::tuple<std::string, fs::path, fs::path>>
CopyCommand::parse_args(const std::string& args, std::string& err)
{
    const auto tokens = split_quoted_args(args);

    if (tokens.size() != 2 && tokens.size() != 3)
    {
        err = "Usage: copy [--recursive | -r] <source> <destination>";
        return std::nullopt;
    }

    std::string option;
    fs::path source, destination;

    if (tokens.size() == 3)
    {
        option = tokens[0];

        bool valid = false;
        if (option == "-r")
            valid = true;
        else
        {
            const std::string opt_upper = option;
            make_upper(option);
            if (opt_upper == "--RECURSIVE")
                valid = true;
        }

        if (!valid)
        {
            err = "Unknown option: " + option;
            return std::nullopt;
        }

        source = fs::path(tokens[1]);
        destination = fs::path(tokens[2]);
    }
    else
    {
        option.clear();
        source = fs::path(tokens[0]);
        destination = fs::path(tokens[1]);
    }

    return std::make_tuple(option, source, destination);
}



std::vector<std::string> CopyCommand::split_quoted_args(const std::string& s)
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