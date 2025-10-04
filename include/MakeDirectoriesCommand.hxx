//
// Created by Loaf on 8/31/2025.
//

#ifndef SHELL_MAKE_DIRECTORIES_COMMAND_HXX
#define SHELL_MAKE_DIRECTORIES_COMMAND_HXX

#include "CommandShell.hxx"
#include <memory>
#include <vector>
#include "using_arguments.h"
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

class MakeDirectoriesCommand final : public Command
{
public:
    CommandResult execute(arguments const& args) override;
    [[nodiscard]] auto clone() const -> std::unique_ptr<Command> override;

private:
    static bool validateArguments(arguments const& args);
    static CommandResult createDirectory(std::string_view arg);
    static void reportError(std::error_code ec);
};

#endif // SHELL_MAKE_DIRECTORIES_COMMAND_HXX
