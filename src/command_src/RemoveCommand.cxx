#include "RemoveCommand.hxx"
#include "StringUtils.hxx"
#include <iostream>
#include <algorithm>

bool RemoveCommand::isRecursiveOption(std::string_view option)
{
    if (option == "-r")
        return true;

    std::string upper(option);
    std::ranges::transform(upper, upper.begin(), [](const unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); });
    return upper == "--RECURSIVE";
}

CommandResult RemoveCommand::removePath(fs::path const& target, const bool recursive)
{
    if (!fs::exists(target))
    {
        std::cerr << "The system cannot find the path specified: " << target << '\n';
        return CommandResult::PathNotFound;
    }

    try
    {
        if (fs::is_directory(target))
        {
            if (!recursive)
            {
                std::cerr << "The specified path is a directory. Use -r to remove recursively.\n";
                return CommandResult::InvalidSyntax;
            }
            fs::remove_all(target);
        }
        else
        {
            fs::remove(target);
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error removing '" << target << "': " << e.what() << '\n';
        return CommandResult::PathNotFound;
    }

    return CommandResult::Success;
}

CommandResult RemoveCommand::execute(arguments const& args)
{
    if (args.empty())
    {
        std::cerr << "Usage: remove [--recursive | -r] <path>\n";
        return CommandResult::InvalidSyntax;
    }

    fs::path target;
    bool recursive = false;

    if (args.size() == 1)
    {
        target = args[0];
    }
    else if (args.size() == 2)
    {
        if (!isRecursiveOption(args[0]))
        {
            std::cerr << "Unknown option: " << args[0] << '\n';
            return CommandResult::UnknownOption;
        }
        recursive = true;
        target = args[1];
    }
    else
    {
        std::cerr << "Usage: remove [--recursive | -r] <path>\n";
        return CommandResult::InvalidSyntax;
    }

    return removePath(target, recursive);
}

auto RemoveCommand::clone() const -> std::unique_ptr<Command>
{
    return std::make_unique<RemoveCommand>(*this);
}
