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

class REPL final
{
public:
    REPL();
    bool operator()(std::string_view /* input */);
    [[nodiscard]] static std::optional<ParsedCmd> parse_args(std::string_view /* input */);
    static std::string get_dir();
    static constexpr int MAJOR = 0;
    static constexpr int MINOR = 1;
    static constexpr int PATCH = 0;

private:
    using CommandFn = std::function<void(std::string_view)>;
    std::map<ReservedWords, CommandFn> m_commands;
};


#endif