#ifndef SHELL_LIST_DIRECTORIES_COMMAND_HXX
#define SHELL_LIST_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include "using_arguments.h"

class ListDirectoriesCommand final : public Command
{
public:
    CommandResult execute(arguments const& /* args */) override;
    [[nodiscard]] auto clone() const -> std::unique_ptr<Command> override;

private:
    static void PrintEntry(fs::directory_entry const& /* entry */);
    static std::string DetermineColor(fs::directory_entry const& /* entry */);
};

#endif
