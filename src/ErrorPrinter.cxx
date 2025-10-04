//
// Created by Loaf on 10/4/2025.
//

#include "ErrorPrinter.hxx"
#include "get_console_width.h"
#include "ANSI.hxx"
#include <iostream>
#include <mutex>

namespace
{
    std::mutex g_lastErrMutex;
    std::optional<std::string> g_lastErr;
}


void ErrorPrinter::setLastError(std::string msg)
{
    std::lock_guard guard{ g_lastErrMutex };
    g_lastErr = std::move(msg);
}

std::optional<std::string> ErrorPrinter::getLastError()
{
    std::lock_guard lg{ g_lastErrMutex };
    return g_lastErr;
}

void ErrorPrinter::clearLastError()
{
    std::lock_guard lg{ g_lastErrMutex };
    g_lastErr.reset();
}

static void print_formatted_error(const std::string& message)
{
    bool in_escape{};
    int col{};
    int last_line_col{};
    std::string cleaned{};

    for (size_t i = 0; i < message.size(); ++i)
    {
        const char c{ message[i] };

        if (in_escape)
        {
            if (c >= '@' && c <= '~')
            {
                in_escape = false;
            }
            continue;
        }

        if (c == '\033')
        {
            if (i + 1 < message.size() && message[i + 1] == '[')
            {
                in_escape = true;
                ++i;
            }
            continue;
        }

        cleaned.push_back(c);

        if (c == '\n')
        {
            last_line_col = 0;
            col = 0;
        }
        else if (c == '\r')
        {
            col = 0;
            last_line_col = col;
        }
        else if (c == '\t')
        {
            constexpr int TABSTOP{ 8 };
            int advance{ TABSTOP - (col % TABSTOP) };
            col += advance;
            last_line_col = col;
        }
        else
        {
            ++col;
            last_line_col = col;
        }
    }

    int console_width{ get_console_width() };
    if (console_width <= 0)
    {
        console_width = 80;
    }
    int cursor_col{ last_line_col % console_width };
    int emoticon_width{ 2 };
    int spaces_needed{ console_width - cursor_col - emoticon_width };

    std::string padding;
    if (spaces_needed > 0)
    {
        padding.assign(spaces_needed, ' ');
    }
    else
    {
        padding = "\n";
        padding.append(std::max(0, console_width - emoticon_width), ' ');
    }

    std::cerr << message << padding << ansi::withForeground(":(", ansi::Foreground::RED) << '\n';
}