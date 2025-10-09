#include "RenameCommand.hxx"
#include <iostream>
#include <vector>

#include "ANSI.hxx"
#include "ErrorPrinter.hxx"

std::optional<std::pair<std::string, std::string>>
RenameCommand::parseArguments(arguments const& args, std::string& err)
{
    if (args.size() != 2)
    {
        err = ansi::withForeground("Usage", ansi::Foreground::RED)+ ": RENAME <old> <new>";
        return std::nullopt;
    }

    return std::make_pair(args[0], args[1]);
}

CommandResult RenameCommand::renamePath(fs::path const& source, fs::path const& destination)
{
    if (!fs::exists(source))
    {
        ErrorPrinter::setLastError("The system cannot find the path specified: " + source.string());
        return CommandResult::PathNotFound;
    }

    std::error_code ec{};
    fs::rename(source, destination, ec);

    if (ec)
    {
        ErrorPrinter::setLastError("Error renaming file: " + ec.message());
        return CommandResult::Failure;
    }

    return CommandResult::Success;
}

CommandResult RenameCommand::execute(arguments const& args)
{
    std::string err{};
    auto parsed{ parseArguments(args, err) };

    if (!parsed)
    {
        ErrorPrinter::setLastError(err);
        return CommandResult::InvalidSyntax;
    }

    auto const& [source, destination]{ *parsed };
    return renamePath(source, destination);
}

auto RenameCommand::clone() const -> std::unique_ptr<Command>
{
    return std::make_unique<RenameCommand>(*this);
}
