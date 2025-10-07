#include "CopyCommand.hxx"
#include <algorithm>
#include <ranges>

#include "ANSI.hxx"
#include "ErrorPrinter.hxx"

bool
CopyCommand::isRecursiveOption(std::string const& option)
{
    if (option == "-r")
        return true;

    std::string upper{ option };
    std::ranges::transform(
        upper, upper.begin(),
        [](unsigned char const c) { return static_cast<unsigned char>(std::toupper(c)); });
    return upper == "--RECURSIVE";
}

std::optional<std::pair<std::string, std::string>>
CopyCommand::parseArguments(arguments const& args, bool& recursive)
{
    recursive = false;

    if (args.size() == 2)
        return std::make_pair(args[0], args[1]);

    if (args.size() == 3)
    {
        if (!isRecursiveOption(args[0]))
        {
            ErrorPrinter::setLastError(
                ansi::withForeground("Unknown option", ansi::Foreground::RED)
                + ": " + args[0]);
            return std::nullopt;
        }

        recursive = true;
        return std::make_pair(args[1], args[2]);
    }

    ErrorPrinter::setLastError(
        ansi::withForeground("Usage", ansi::Foreground::RED)
        + ": copy [--recursive | -r] <source> <destination>");
    return std::nullopt;
}

CommandResult
CopyCommand::performCopy(fs::path const& source,
                         fs::path const& destination,
                         bool const recursive)
{
    std::error_code ec{};

    fs::copy_options options{ fs::copy_options::overwrite_existing };
    if (recursive)
        options |= fs::copy_options::recursive;

    fs::copy(source, destination, options, ec);

    if (ec)
    {
        ErrorPrinter::setLastError(
            ansi::withForeground("Error", ansi::Foreground::RED)
            + " copying '" + source.string() + "' to '" + destination.string() + "': "
            + ec.message());
        return CommandResult::PathNotFound;
    }

    return CommandResult::Success;
}

CommandResult
CopyCommand::execute(arguments const& args)
{
    bool recursive{ false };
    auto parsed{ parseArguments(args, recursive) };

    if (!parsed)
        return CommandResult::InvalidSyntax;

    auto const& [sourceArg, destinationArg]{ *parsed };
    fs::path const source{ sourceArg };
    fs::path const destination{ destinationArg };

    if (!fs::exists(source))
    {
        ErrorPrinter::setLastError(
            "The system cannot find the path specified: " + source.string());
        return CommandResult::PathNotFound;
    }

    return performCopy(source, destination, recursive);
}

auto CopyCommand::clone() const -> std::unique_ptr<Command>
{
    return std::make_unique<CopyCommand>(*this);
}
