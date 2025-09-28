//
// Created by Loaf on 8/31/2025.
//

#ifndef SHELL_RESERVED_WORD_H
#define SHELL_RESERVED_WORD_H

#include <string>
#include <map>
#include <stdexcept>

enum class ReservedWords : unsigned char {
    DIR,
    CD,
    CHDIR,
    CLEAR,
    CLS,
    COPY,
    DEL,
    DELETE,
    ERASE,
    PRINT,
    EXIT,
    HELP,
    MKDIR,
    MD,
    PAUSE,
    RMDIR,
    RD,
    RENAME,
    REN,
    SHOW,
    TOUCH,
    WHEREAMI,
    VER,
    UNKNOWN
};


inline ReservedWords stringToReservedWord(const std::string& str)
{
    static const std::map<std::string, ReservedWords> stringToEnum =
    {
        {"DIR",     ReservedWords::DIR},
        {"CD",      ReservedWords::CD},
        {"CHDIR",   ReservedWords::CHDIR},
        {"CLEAR",   ReservedWords::CLEAR},
        {"CLS",     ReservedWords::CLS},
        {"COPY",    ReservedWords::COPY},
        {"DEL",     ReservedWords::DEL},
        {"DELETE",  ReservedWords::DELETE},
        {"ERASE",   ReservedWords::ERASE},
        {"PRINT",   ReservedWords::PRINT},
        {"EXIT",    ReservedWords::EXIT},
        {"HELP",    ReservedWords::HELP},
        {"MKDIR",   ReservedWords::MKDIR},
        {"MD",      ReservedWords::MD},
        {"PAUSE",   ReservedWords::PAUSE},
        {"RMDIR",   ReservedWords::RMDIR},
        {"RD",      ReservedWords::RD},
        {"RENAME",  ReservedWords::RENAME},
        {"REN",     ReservedWords::REN},
        {"SHOW",    ReservedWords::SHOW},
        {"WHEREAMI",ReservedWords::WHEREAMI},
        {"TOUCH",   ReservedWords::TOUCH},
        {"VER",     ReservedWords::VER}
    };

    if (const auto it = stringToEnum.find(str); it != stringToEnum.end())
        return it->second;
    return ReservedWords::UNKNOWN;
}

constexpr const char* reservedWordToString(const ReservedWords word)
{
    switch (word)
    {
        case ReservedWords::UNKNOWN:   return "UNKNOWN";
        case ReservedWords::DIR:       return "DIR";
        case ReservedWords::CD:        return "CD";
        case ReservedWords::CHDIR:     return "CHDIR";
        case ReservedWords::CLEAR:     return "CLEAR";
        case ReservedWords::CLS:       return "CLS";
        case ReservedWords::COPY:      return "COPY";
        case ReservedWords::DEL:       return "DEL";
        case ReservedWords::DELETE:    return "DELETE";
        case ReservedWords::ERASE:     return "ERASE";
        case ReservedWords::PRINT:     return "PRINT";
        case ReservedWords::EXIT:      return "EXIT";
        case ReservedWords::HELP:      return "HELP";
        case ReservedWords::MKDIR:     return "MKDIR";
        case ReservedWords::MD:        return "MD";
        case ReservedWords::PAUSE:     return "PAUSE";
        case ReservedWords::RMDIR:     return "RMDIR";
        case ReservedWords::RD:        return "RD";
        case ReservedWords::RENAME:    return "RENAME";
        case ReservedWords::REN:       return "REN";
        case ReservedWords::SHOW:      return "SHOW";
        case ReservedWords::WHEREAMI: return "WHEREAMI";
        case ReservedWords::TOUCH:     return "TOUCH";
        case ReservedWords::VER:       return "VER";
        default:                       return "UNKNOWN";
    }
}

constexpr const char* informationAboutReservedWords(const ReservedWords word)
{
    switch (word)
    {
        case ReservedWords::EXIT: return "Exit from the shell.";
        case ReservedWords::CLEAR:
        case ReservedWords::CLS: return "Clear the screen.";
        case ReservedWords::DIR: return "List files and directories.";
        case ReservedWords::VER: return "Cheese Loaf Shell version.";
        case ReservedWords::PRINT: return "Display messages.";
        case ReservedWords::HELP: return "Show help.";
        case ReservedWords::MKDIR:
        case ReservedWords::MD: return "Make Directories.";
        case ReservedWords::RMDIR:
        case ReservedWords::RD: return "Remove Directories.";
        case ReservedWords::RENAME:
        case ReservedWords::REN: return "Replace a file or files.";
        case ReservedWords::COPY: return "Copy files.";
        case ReservedWords::TOUCH: return "Create a file.";
        case ReservedWords::SHOW: return "Outputs the file.";
        case ReservedWords::DELETE:
        case ReservedWords::DEL:
        case ReservedWords::ERASE: return "Removes the file.";
        case ReservedWords::CHDIR:
        case ReservedWords::CD: return "Change current directories.";
        case ReservedWords::WHEREAMI: return "Show the current directories.";
        case ReservedWords::PAUSE: return "Delays the shell until you press any keys.";
        default: return "UNKNOWN.";
    }
}

#endif //SHELL_RESERVED_WORD_H