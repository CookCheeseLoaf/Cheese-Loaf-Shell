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
    if (_dupenv_s(&buffer, &size, "USERPROFILE") == 0 && buffer)
    {
        std::string home{ buffer };
        free(buffer);
        return home;
    }
    char* drive{ nullptr }, *path{ nullptr };
    size_t dsize{}, psize{};
    if (_dupenv_s(&drive, &dsize, "HOMEDRIVE") == 0 && drive &&
        _dupenv_s(&path, &psize, "HOMEPATH") == 0 && path)
    {
        std::string home{ std::string(drive) + std::string(path) };
        free(drive); free(path);
        return home;
    }
#else
    if (const char* home = std::getenv("HOME"))
        return std::string(home);
#endif
    return {};
}

bool FileSystemUtils::is_executable(fs::path const& path)
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
