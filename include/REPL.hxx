#ifndef REPL_HXX
#define REPL_HXX
#include "ReservedWords.hxx"
#include <map>
#include <functional>

#include "ParsedCmd.hxx"
#include "using_arguments.h"

template <typename MapType>
bool executeCommandIfExists(ReservedWords word, const std::string& args, MapType& commands)
{
    if (const auto it = commands.find(word); it != commands.end() && it->second)
    {
        it->second(args);
        return true;
    }
    return false;
}

class REPL final
{
public:
    REPL();
    bool operator()(std::string_view /* input */);
    static ParsedCmd parse_args(std::string_view /* input */);
    static std::vector<std::string_view> split_quoted_args(std::string_view /* args */);
    static std::string get_dir();
    static constexpr int MAJOR = 0;
    static constexpr int MINOR = 1;
    static constexpr int PATCH = 0;
    ParsedCmd parsed_command;

private:
    std::map<ReservedWords, std::function<void(arguments)>> m_commands;
};


#endif