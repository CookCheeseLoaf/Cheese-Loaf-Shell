//
// Created by Loaf on 9/22/2025.
//

#ifndef CLSHELL_PARSEDCMD_HXX
#define CLSHELL_PARSEDCMD_HXX
#include <string>

struct ParsedCmd
{
    std::string command;
    std::vector<std::string> args;
};

#endif //CLSHELL_PARSEDCMD_HXX