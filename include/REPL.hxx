#ifndef REPL_HXX
#define REPL_HXX
#include "ReservedWords.hxx"
#include <tuple>

template <typename MapType>
bool executeCommandIfExists(ReservedWords word, const std::string& args, MapType& commands)
{
    if (const auto it{ commands.find(word) }; it != commands.end() && it->second)
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
    static std::tuple<bool, std::string, std::string> parse_args(std::string const&);
public:
    static std::string get_dir();
};

#endif