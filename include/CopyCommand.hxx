#ifndef SHELL_COPY_COMMAND_HPP
#define SHELL_COPY_COMMAND_HPP

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"
#include <tuple>
#include <memory>

class CopyCommand final : public Command
{
public:
	void execute(const std::string& args) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;
private:
    static std::tuple<bool, fs::path, fs::path> parse_args(const std::string& args);
};

#endif