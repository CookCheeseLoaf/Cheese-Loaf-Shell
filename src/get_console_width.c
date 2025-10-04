//
// Created by Loaf on 10/4/2025.
//

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif // _WIN32


int get_console_width()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return 80;
    if (!GetConsoleScreenBufferInfo(h, &csbi)) return 80;
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return (width > 0) ? width : 80;
#else
    struct winsize ws{};
    if (ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return 80;
    }
    return ws.ws_col;
#endif
}
