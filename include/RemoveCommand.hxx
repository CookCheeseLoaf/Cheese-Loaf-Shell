#ifndef SHELL_REMOVE_COMMAND_HXX
#define SHELL_REMOVE_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include <optional>
#include <vector>
#include <utility>
#include <memory>

class RemoveCommand final : public Command
{
public:
	void execute(const std::string& args) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;

private:
    static std::optional<std::pair<std::string, fs::path>> parse_args(const std::string& args, std::string& err);
    static std::vector<std::string> split_quoted_args(const std::string&);
};

#endif
