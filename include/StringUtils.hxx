//
// Created by Loaf on 9/26/2025.
//

#ifndef LSHELL_UTILS_HXX
#define LSHELL_UTILS_HXX
#include <cctype>
#include <string>
#include <algorithm>
#include <ranges>

inline char to_upper_char(unsigned char const c)
{
    return static_cast<char>(std::toupper(c));
}

inline void make_upper(std::string& s)
{
    std::ranges::transform(s, s.begin(), to_upper_char);
}


#endif //LSHELL_UTILS_HXX