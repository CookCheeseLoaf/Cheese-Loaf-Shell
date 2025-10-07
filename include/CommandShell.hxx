//
// Created by Loaf on 8/31/2025.
//

#ifndef SHELL_COMMAND_SHELL_HXX
#define SHELL_COMMAND_SHELL_HXX
#include <memory>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

enum class CommandResult : int
{
    Success = 0,
    Exit,
    Failure,
    InvalidSyntax,
    AlreadyExists,
    PermissionDenied,
    AccessDenied,
    PathNotFound,
    UnknownOption,
    UnknownError
};

std::string_view constexpr to_string(CommandResult const result)
{
    switch (result)
    {
        case CommandResult::Success:       return "Success";
        case CommandResult::Exit:          return "Exit";
        case CommandResult::Failure:       return "Failure";
        case CommandResult::InvalidSyntax: return "Invalid syntax";
        case CommandResult::AlreadyExists: return "Already exists";
        case CommandResult::PermissionDenied: return "Permission denied";
        case CommandResult::AccessDenied:  return "Access denied";
        case CommandResult::PathNotFound:  return "Path not found";
        case CommandResult::UnknownOption: return "Unknown option";
        case CommandResult::UnknownError:  return "Unknown error";
    }
    return "Unknown CommandResult";
}

struct Command
{
    virtual ~Command() = default;
    [[nodiscard]] virtual CommandResult execute(std::vector<std::string> const& /* args */) = 0;
	[[nodiscard]] virtual auto clone() const -> std::unique_ptr<Command> = 0;
};

struct CommandCallable
{
    std::unique_ptr<Command> ptr;

    CommandCallable() = default;
    explicit CommandCallable(std::unique_ptr<Command> p) : ptr{ std::move(p) } {}

    CommandCallable(CommandCallable&&) noexcept = default;
    CommandCallable& operator=(CommandCallable&&) noexcept = default;
    CommandCallable(CommandCallable const& other) :
        ptr{ other.ptr ? other.ptr->clone() : nullptr } {}

    CommandCallable& operator=(CommandCallable const& other)
    {
        if (this != &other)
            ptr = other.ptr ? other.ptr->clone() : nullptr;
        return *this;
    }

    std::optional<CommandResult> operator()(std::vector<std::string> const& args) const
    {
        if (ptr)
            return ptr->execute(args);

        std::cerr << "Command object empty.\n";
        return std::nullopt;
    }

    Command* operator->() noexcept { return ptr.get(); }
    Command const* operator->() const noexcept { return ptr.get(); }
    explicit operator bool() const noexcept { return static_cast<bool>(ptr); }
};

#endif //SHELL_COMMAND_SHELL_HXX