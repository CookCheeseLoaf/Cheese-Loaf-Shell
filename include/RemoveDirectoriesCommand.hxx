#ifndef SHELL_REMOVE_DIRECTORIES_COMMAND_HXX
#define SHELL_REMOVE_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include <memory>
#include "FileSystemUtils.hxx"
#include "using_arguments.h"

class RemoveDirectoriesCommand final : public Command
{
public:
	CommandResult execute(arguments const& /* args */) override;
	[[nodiscard]] auto clone() const -> std::unique_ptr<Command> override;

private:
	static bool validateArguments(arguments const& /* args */);
	static CommandResult removeDirectory(fs::path const& /* dir */);
	static void reportError(std::error_code const& /* ec */);
};

#endif