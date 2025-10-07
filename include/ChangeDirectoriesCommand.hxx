#ifndef SHELL_CHANGE_DIRECTORIES_COMMAND_HXX
#define SHELL_CHANGE_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include "using_arguments.h"

class ChangeDirectoriesCommand final : public Command
{
public:
	ChangeDirectoriesCommand() = default;
    CommandResult execute(arguments const& /* args */) override;
	[[nodiscard]] auto clone() const -> std::unique_ptr<Command> override;
	[[nodiscard]] static fs::path get_current_directory();

private:
	static bool validateArguments(arguments const& /* args */);
};

#endif