//
// Created by Loaf on 8/31/2025.
//
#ifndef SHELL_MAKE_DIRECTORIES_COMMAND_HXX
#define SHELL_MAKE_DIRECTORIES_COMMAND_HXX
#include "CommandShell.hxx"
#include <memory>

class MakeDirectoriesCommand final : public Command
{
public:
    void execute(std::string const& /* args */) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;
};



#endif //SHELL_MAKE_DIRECTORIES_COMMAND_HXX