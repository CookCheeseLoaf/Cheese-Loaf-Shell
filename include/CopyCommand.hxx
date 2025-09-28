#ifndef SHELL_COPY_COMMAND_HPP
#define SHELL_COPY_COMMAND_HPP

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include <memory>
#include <vector>
#include <tuple>
#include <optional>

class CopyCommand final : public Command {
public:
	void execute(const std::string& args) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;
private:
    static std::optional<std::tuple<std::string, fs::path, fs::path>> parse_args(const std::string& args, std::string& err);
    static std::vector<std::string> split_quoted_args(const std::string&);
};

#endif