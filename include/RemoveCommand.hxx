#ifndef SHELL_REMOVE_COMMAND_HXX
#define SHELL_REMOVE_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include <tuple>
#include <memory>

class RemoveCommand final : public Command
{
public:
	void execute(const std::string& args) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;

private:
    static std::tuple<bool, fs::path> parse_args(const std::string& args);
};

#endif
