#ifndef SHELL_RENAME_COMMAND_HXX
#define SHELL_RENAME_COMMAND_HXX

#include "CommandShell.hxx"
#include <tuple>
#include "FileSystemUtils.hxx"
#include <memory>

class RenameCommand final: public Command
{
public:
    void execute(const std::string& args) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;

private:
    static std::tuple<bool, fs::path, fs::path> parse_args(const std::string& args);
};

#endif
