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
    [[nodiscard]] std::unique_ptr<Command> clone() const override;

private:
    static std::optional<std::pair<std::string_view, std::string_view>>
        parseArguments(arguments const& args, std::string& err);
    static CommandResult renamePath(fs::path const& source, fs::path const& destination);
};


#endif
