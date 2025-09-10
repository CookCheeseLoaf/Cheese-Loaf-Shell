//
// Created by Loaf on 8/31/2025.
//

#ifndef SHELL_COMMAND_SHELL_HXX
#define SHELL_COMMAND_SHELL_HXX
#include <string>
#include <memory>
#include <iostream>

struct Command
{
    virtual ~Command() = default;
    virtual void execute(const std::string& args) = 0;
    [[nodiscard]] virtual std::unique_ptr<Command> clone() const = 0;
};

struct CommandCallable
{
    std::unique_ptr<Command> ptr;

    CommandCallable() = default;
    explicit CommandCallable(std::unique_ptr<Command> p) : ptr{ std::move(p) } {}

    CommandCallable(CommandCallable&&) noexcept = default;
    CommandCallable& operator=(CommandCallable&&) noexcept = default;
    CommandCallable(const CommandCallable& other) :
        ptr{ other.ptr ? other.ptr->clone() : nullptr } {}

    CommandCallable& operator=(const CommandCallable& other)
    {
        if (this != &other) {
            ptr = other.ptr ? other.ptr->clone() : nullptr;
        }
        return *this;
    }

    void operator()(const std::string& args) const
    {
        if (ptr) ptr->execute(args);
        else std::cerr << "Command object empty.\n";
    }

    Command* operator->() noexcept { return ptr.get(); }
    const Command* operator->() const noexcept { return ptr.get(); }
    explicit operator bool() const noexcept { return static_cast<bool>(ptr); }
};

#endif //SHELL_COMMAND_SHELL_HXX