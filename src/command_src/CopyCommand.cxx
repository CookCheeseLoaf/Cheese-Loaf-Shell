//
// Created by Loaf on 9/4/2025.
//

#include "CopyCommand.hxx"
#include <iostream>
#include <sstream>
#include <vector>


void CopyCommand::execute(const std::string& args)
{
    auto [valid, current_file, new_file] = parse_args(args);
    if (!valid) return;

    if (!fs::exists(current_file))
    {
        std::cerr << "The system cannot find the path specified: " << current_file << '\n';
        return;
    }

    try
    {
        fs::copy(current_file, new_file);
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

std::tuple<bool, fs::path, fs::path> CopyCommand::parse_args(const std::string& args)
{
    std::istringstream iss{ args };
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);

    if (tokens.size() != 2)
    {
        std::cerr << "The syntax of the command is incorrect. Usage: Copy <current> <new>\n";
        return { false, {}, {} };
    }

    return { true, fs::path{ tokens[0] }, fs::path{ tokens[1] } };
}