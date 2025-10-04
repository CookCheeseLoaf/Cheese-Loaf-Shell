//
// Created by Loaf on 9/22/2025.
//

#ifndef CLSHELL_PARSEDCMD_HXX
#define CLSHELL_PARSEDCMD_HXX
#include <string>
#include <utility>

struct ParsedCmd
{
    std::string command;
    std::vector<std::string_view> args;

    ParsedCmd() = default;

    ParsedCmd(std::string_view command, std::vector<std::string_view> args)
    {
        this->command = command;
        this->args = std::move(args);
    }
};

#endif //CLSHELL_PARSEDCMD_HXX