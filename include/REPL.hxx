#ifndef REPL_HXX
#define REPL_HXX
#include "ReservedWords.hxx"
#include <tuple>
#include <map>
#include <functional>
#include <optional>

#include "ParsedCmd.hxx"

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

class REPL {
public:
    REPL();
    bool operator()(const std::string& str);

private:
    std::map<ReservedWords, std::function<void(std::string const&)>> m_commands;
    static std::optional<ParsedCmd> parse_args_opt(std::string const& input);
public:
    static std::string get_dir();
    static constexpr int MAJOR = 0, MINOR = 1, PATCH = 0;
};

#endif