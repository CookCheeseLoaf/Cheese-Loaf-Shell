//
// Created by Loaf on 9/4/2025.
//

#include "RemoveCommand.hxx"
#include <iostream>
#include <sstream>

void RemoveCommand::execute(const std::string& args)
{
    auto [valid, current_file] { parse_args(args) };
    if (!valid) return;

    if (fs::is_directory(current_file))
    {
        std::cerr << "The specified path is not a file.\n";
        return;
    }

    if (!fs::exists(current_file))
    {
        std::cerr << "The system cannot find the path specified: " << current_file << '\n';
        return;
    }

    try
    {
        fs::remove(current_file);
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error removing file: " << e.what() << '\n';
    }
}

std::unique_ptr<Command> RemoveCommand::clone() const
{
    return std::make_unique<RemoveCommand>(*this);
}

std::tuple<bool, fs::path> RemoveCommand::parse_args(const std::string& args)
{
    std::istringstream iss{ args };
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);

    if (tokens.size() != 1)
    {
        std::cerr << "The syntax of the command is incorrect. Usage: DELETE <file>\n";
        return { false, {} };
    }

    return { true, fs::path{ tokens[0] } };
}