#include "RenameCommand.hxx"
#include <iostream>
#include <vector>

std::optional<std::pair<std::string_view, std::string_view>>
RenameCommand::parseArguments(arguments const& args, std::string& err)
{
    if (args.size() != 2)
    {
        err = "The syntax of the command is incorrect. Usage: rename <old> <new>";
        return std::nullopt;
    }

    return std::make_pair(args[0], args[1]);
}

CommandResult RenameCommand::renamePath(fs::path const& source, fs::path const& destination)
{
    if (!fs::exists(source))
    {
        std::cerr << "The system cannot find the path specified: " << source << '\n';
        return CommandResult::PathNotFound;
    }

    std::error_code ec;
    fs::rename(source, destination, ec);

    if (ec)
    {
        std::cerr << "Error renaming file: " << ec.message() << '\n';
        return CommandResult::CommandFailed;
    }

    return CommandResult::Success;
}

CommandResult RenameCommand::execute(arguments const& args)
{
    std::string err;
    auto parsed = parseArguments(args, err);

    if (!parsed)
    {
        std::cerr << err << '\n';
        return CommandResult::InvalidSyntax;
    }

    auto [source, destination] = *parsed;
    return renamePath(source, destination);
}

std::unique_ptr<Command> RenameCommand::clone() const
{
    return std::make_unique<RenameCommand>(*this);
}