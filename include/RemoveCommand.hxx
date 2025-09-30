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
    using two_paths = std::pair<fs::path, fs::path>;
    static std::optional<two_paths> parse_args(const std::string& /* args */, std::string& /* err */);
    static std::vector<std::string> split_quoted_args(const std::string& /* args */);
};

#endif
