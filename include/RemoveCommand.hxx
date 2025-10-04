#ifndef SHELL_REMOVE_COMMAND_HXX
#define SHELL_REMOVE_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include <memory>

#include "using_arguments.h"

class RemoveCommand final : public Command
{
public:
    CommandResult execute(arguments const& args) override;
    [[nodiscard]] auto clone() const -> std::unique_ptr<Command> override;

private:
    static bool isRecursiveOption(std::string_view option);
    static CommandResult removePath(fs::path const& target, bool recursive);
};

#endif
