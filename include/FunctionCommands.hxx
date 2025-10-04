//
// Created by Loaf on 9/5/2025.
//

#ifndef SHELL_FUNCTIONCOMMANDS_HXX
#define SHELL_FUNCTIONCOMMANDS_HXX
#include "CommandShell.hxx"
#include "using_arguments.h"

CommandResult help_command(arguments const& /* args */);
CommandResult touch_command(arguments const& /* args */);
CommandResult show_command(arguments const& /* args */);
CommandResult version_command(arguments const& /* args */);
CommandResult print_command(arguments const& /* args */);

#endif //SHELL_FUNCTIONCOMMANDS_HXX