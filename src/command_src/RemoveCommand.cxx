#include "RemoveCommand.hxx"
#include "StringUtils.hxx"
#include <iostream>
#include <algorithm>

#include "ANSI.hxx"
#include "ErrorPrinter.hxx"

bool RemoveCommand::isRecursiveOption(std::string_view const option)
{
    if (option == "-r")
        return true;

    std::string upper{ option };
    std::ranges::transform(upper, upper.begin(), [](unsigned char const c) { return static_cast<unsigned char>(std::toupper(c)); });
    return upper == "--RECURSIVE";
}

CommandResult RemoveCommand::removePath(fs::path const& target, bool const recursive)
{
    if (!fs::exists(target))
    {
        ErrorPrinter::setLastError("The system cannot find the path specified: " + target.string());
        return CommandResult::PathNotFound;
    }

    try
    {
        if (fs::is_directory(target))
        {
            if (!recursive)
            {
                ErrorPrinter::setLastError("The specified path is a directory. Use -r to remove recursively.");
                return CommandResult::InvalidSyntax;
            }
            fs::remove_all(target);
        }
        else
            fs::remove(target);
    }
    catch (fs::filesystem_error const& e)
    {
        ErrorPrinter::setLastError(ansi::withForeground("Error", ansi::Foreground::RED) + " removing '" + target.string() + "': " + e.what());
        return CommandResult::PathNotFound;
    }

    return CommandResult::Success;
}

CommandResult RemoveCommand::execute(arguments const& args)
{
    if (args.empty())
    {
        ErrorPrinter::setLastError(ansi::withForeground("Usage", ansi::Foreground::RED)
        + ": REMOVE [--RECURSIVE | -r] <path>");
        return CommandResult::InvalidSyntax;
    }

    fs::path target{};
    bool recursive{ false };

    if (args.size() == 1)
        target = args[0];
    else if (args.size() == 2)
    {
        if (!isRecursiveOption(args[0]))
        {
            ErrorPrinter::setLastError(ansi::withForeground("Unknown option", ansi::Foreground::RED)
            + ": " + args[0]);
            return CommandResult::UnknownOption;
        }
        recursive = true;
        target = args[1];
    }
    else
    {
        ErrorPrinter::setLastError(ansi::withForeground("Usage", ansi::Foreground::RED)
        + ": REMOVE [--RECURSIVE | -r] <path>");
        return CommandResult::InvalidSyntax;
    }

    return removePath(target, recursive);
}

auto RemoveCommand::clone() const -> std::unique_ptr<Command>
{
    return std::make_unique<RemoveCommand>(*this);
}
