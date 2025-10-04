#ifndef SHELL_CHANGE_DIRECTORIES_COMMAND_HXX
#define SHELL_CHANGE_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include "using_arguments.h"

class ChangeDirectoriesCommand final : public Command
{
public:
	ChangeDirectoriesCommand();
    CommandResult execute(arguments const& /* args */) override;
	[[nodiscard]] std::unique_ptr<Command> clone() const override;
	[[nodiscard]] fs::path get_current_directory() const;

private:
	static bool validateArguments(arguments const& args);
	fs::path current_directory;
};

#endif