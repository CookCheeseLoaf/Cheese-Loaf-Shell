#include "CopyCommand.hxx"
#include <iostream>
#include <algorithm>
#include <ranges>

bool
CopyCommand::isRecursiveOption(std::string_view option)
{
    if (option == "-r")
        return true;

    std::string upper(option);
    std::ranges::transform(upper, upper.begin(), ::toupper);
    return upper == "--RECURSIVE";
}

std::optional<std::pair<std::string_view, std::string_view>>
CopyCommand::parseArguments(arguments const& args, bool& recursive)
{
    recursive = false;

    if (args.size() == 2)
        return std::make_pair(args[0], args[1]);

    if (args.size() == 3)
    {
        if (!isRecursiveOption(args[0]))
        {
            std::cerr << "Unknown option: " << args[0] << '\n';
            return std::nullopt;
        }

        recursive = true;
        return std::make_pair(args[1], args[2]);
    }

    std::cerr << "Usage: copy [--recursive | -r] <source> <destination>\n";
    return std::nullopt;
}

CommandResult
CopyCommand::performCopy(fs::path const& source,
                                       fs::path const& destination,
                                       const bool recursive)
{
    std::error_code ec;

    fs::copy_options options = fs::copy_options::overwrite_existing;
    if (recursive)
        options |= fs::copy_options::recursive;

    fs::copy(source, destination, options, ec);

    if (ec)
    {
        std::cerr << "Error copying '" << source << "' to '" << destination
                  << "': " << ec.message() << '\n';
        return CommandResult::PathNotFound;
    }

    return CommandResult::Success;
}

CommandResult
CopyCommand::execute(arguments const& args)
{
    bool recursive = false;
    auto parsed = parseArguments(args, recursive);

    if (!parsed)
        return CommandResult::InvalidSyntax;

    auto [sourceArg, destinationArg] = *parsed;
    const fs::path source(sourceArg);
    const fs::path destination(destinationArg);

    if (!fs::exists(source))
    {
        std::cerr << "The system cannot find the path specified: " << source << '\n';
        return CommandResult::PathNotFound;
    }

    return performCopy(source, destination, recursive);
}

std::unique_ptr<Command>
CopyCommand::clone() const
{
    return std::make_unique<CopyCommand>(*this);
}
