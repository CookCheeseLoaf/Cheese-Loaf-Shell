#include "RenameCommand.hxx"
#include <iostream>
#include <vector>

void RenameCommand::execute(const std::string& args)
{
    std::string err;
    auto p = parse_args(args, err);
    if (!p)
    {
        std::cout << err << '\n';
        return;
    }

    auto [source, destination] = *p;

    if (!fs::exists(source))
    {
        std::cerr << "The system cannot find the path specified: " << source << '\n';
        return;
    }

    try
    {
        fs::rename(source, destination);
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error renaming file: " << e.what() << '\n';
    }
}

std::unique_ptr<Command> RenameCommand::clone() const
{
    return std::make_unique<RenameCommand>(*this);
}

std::optional<std::pair<fs::path, fs::path>> RenameCommand::parse_args(const std::string& args, std::string& err)
{
    const auto tokens = split_quoted_args(args);

    if (tokens.size() != 2)
    {
        err = "The syntax of the command is incorrect. Usage: RENAME <old> <new>";
        return std::nullopt;
    }

    return std::make_pair( fs::path{ tokens[0] }, fs::path{ tokens[1] } );
}


std::vector<std::string> RenameCommand::split_quoted_args(const std::string& s)
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