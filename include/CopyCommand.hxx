#ifndef SHELL_COPY_COMMAND_HPP
#define SHELL_COPY_COMMAND_HPP

#include "CommandShell.hxx"
#include <memory>
#include "FileSystemUtils.hxx"
#include "using_arguments.h"

class CopyCommand final : public Command
{
public:
	CommandResult execute(arguments const& args) override;
	[[nodiscard]] auto clone() const -> std::unique_ptr<Command> override;

private:
	static bool isRecursiveOption(std::string_view option);
	static std::optional<std::pair<std::string_view, std::string_view>>
		parseArguments(arguments const& args, bool& recursive);
	static CommandResult performCopy(fs::path const& source,
									fs::path const& destination,
									bool recursive);
};

#endif