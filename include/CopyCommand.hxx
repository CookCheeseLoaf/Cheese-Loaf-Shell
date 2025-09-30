#ifndef SHELL_COPY_COMMAND_HPP
#define SHELL_COPY_COMMAND_HPP

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include <memory>
#include <vector>
#include <tuple>
#include <optional>

class CopyCommand final : public Command
{
public:
	void execute(std::string const& /* args */) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;
private:
    static std::optional<std::tuple<std::string, fs::path, fs::path>> parse_args(std::string const& /* args */, std::string& /* err */);
    static std::vector<std::string> split_quoted_args(std::string const& /* args */);
};

#endif