#ifndef SHELL_REMOVE_DIRECTORIES_COMMAND_HXX
#define SHELL_REMOVE_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include <memory>

class RemoveDirectoriesCommand final : public Command
{
public:
	void execute(std::string const& /* args */) override;
	[[nodiscard]] std::unique_ptr<Command> clone() const override;
};

#endif