//
// Created by Loaf on 10/4/2025.
//

#ifndef LSHELL_ERRORPRINTER_HXX
#define LSHELL_ERRORPRINTER_HXX

#include <optional>
#include <string>


namespace ErrorPrinter
{
    void setLastError(std::string /* msg */);

    std::optional<std::string> getLastError();

    void clearLastError();
}

void print_formatted_error(std::string const& /* message */);

#endif //LSHELL_ERRORPRINTER_HXX