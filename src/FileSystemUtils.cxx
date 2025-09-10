//
// Created by Loaf on 9/4/2025.
//

#include "FileSystemUtils.hxx"

#ifndef _WIN32
#include <unistd.h>
#include <sys/stat.h>
#endif

std::string FileSystemUtils::get_home_directory()
{
#ifdef _WIN32
    char* buffer{ nullptr };
    size_t size{};
    if (_dupenv_s(&buffer, &size, "USERPROFILE") == 0 && buffer != nullptr)
    {
        std::string home{ buffer };
        free(buffer);
        return home;
    }
#else
    if (const char* home{ std::getenv("HOME") })
        return home;
#endif
        return ".";
}

bool FileSystemUtils::is_executable(const fs::path &path)
{
#ifdef _WIN32
        return path.extension() == ".exe";
#else
        if (path.extension() == ".sh")
        {
            struct stat st;
            if (stat(path.c_str(), &st) == 0)
                return (st.st_mode & S_IXUSR) != 0;
        }
        return false;
#endif
}
