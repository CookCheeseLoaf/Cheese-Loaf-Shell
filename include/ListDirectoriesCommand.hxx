#ifndef SHELL_LIST_DIRECTORIES_COMMAND_HXX
#define SHELL_LIST_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include "FileSystemUtils.hxx"

class ListDirectoriesCommand final : public Command
{
public:
    void execute(const std::string& /* args */) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;

private:
    static void PrintEntry(fs::directory_entry const& /* entry */);
    static std::string DetermineColor(fs::directory_entry const& /* entry */);
};

#endif
