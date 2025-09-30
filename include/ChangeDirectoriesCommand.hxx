#ifndef SHELL_CHANGE_DIRECTORIES_COMMAND_HXX
#define SHELL_CHANGE_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"

class ChangeDirectoriesCommand final : public Command
{
public:
	ChangeDirectoriesCommand();
	void execute(std::string const& /* args */) override;
	[[nodiscard]] std::unique_ptr<Command> clone() const override;
	[[nodiscard]] fs::path get_current_directory() const;

private:
	fs::path current_directory;
};

#endif