#ifndef SHELL_RENAME_COMMAND_HXX
#define SHELL_RENAME_COMMAND_HXX

#include "CommandShell.hxx"
#include <memory>
#include "FileSystemUtils.hxx"
#include "using_arguments.h"

class RenameCommand final : public Command
{
public:
    CommandResult execute(arguments const& args) override;
    [[nodiscard]] auto clone() const -> std::unique_ptr<Command> override;

private:
    static std::optional<std::pair<std::string, std::string>>
        parseArguments(arguments const& args, std::string& err);
    static CommandResult renamePath(fs::path const& source, fs::path const& destination);
};


#endif
