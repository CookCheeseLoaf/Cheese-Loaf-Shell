//
// Created by Loaf on 9/28/2025.
//

#include "portable_getch.h"
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>

int portable_getch()
{
    printf("Press any key to continue...");
    fflush(stdout);
    const int ch = _getch();
    puts("\r\n");
    return ch;
}

#else
#include <unistd.h>
#include <termios.h>

int portable_getch()
{
    printf("Press any key to continue...");
    fflush(stdout);

    struct termios oldt, newt;
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) return EOF;

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt) != 0) return EOF;

    unsigned char ch{};
    ssize_t n = read(STDIN_FILENO, &ch, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (n <= 0) return EOF;

    puts("\n");
    return (int)ch;
}
#endif
