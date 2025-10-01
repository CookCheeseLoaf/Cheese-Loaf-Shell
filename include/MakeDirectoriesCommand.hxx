//
// Created by Loaf on 8/31/2025.
//
#ifndef SHELL_MAKE_DIRECTORIES_COMMAND_HXX
#define SHELL_MAKE_DIRECTORIES_COMMAND_HXX
#include "CommandShell.hxx"
#include <memory>
#include <optional>
#include <vector>

class MakeDirectoriesCommand final : public Command
{
public:
    void execute(std::string const& /* args */) override;
    [[nodiscard]] std::unique_ptr<Command> clone() const override;

private:
    using two_paths = std::pair<fs::path, fs::path>;
    static std::optional<two_paths> parse_args(std::string const& /* args */, std::string& /* err */);
    static std::vector<std::string> split_quoted_args(std::string const& /* args */);
};



#endif //SHELL_MAKE_DIRECTORIES_COMMAND_HXX