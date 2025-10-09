//
// Created by Loaf on 9/4/2025.
//

#include "FileSystemUtils.hxx"

#include <iostream>
#include <process.h>

#include "ANSI.hxx"
#include "ErrorPrinter.hxx"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#endif

namespace FileSystemUtils
{
    std::string get_home_directory()
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
        if (const char* home{ std::getenv("HOME") })
            return std::string(home);
    #endif
        return {};
    }

    bool is_executable(fs::path const& path)
    {
    #ifdef _WIN32
    std::string ext{ path.extension().string() };
    std::ranges::transform(ext, ext.begin(),
                           [](unsigned char const c){ return static_cast<char>(std::toupper(c)); });
    return ext == ".EXE";
    #else
        struct stat st;
        if (stat(path.c_str(), &st) == 0)
            return (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0;
        return false;
    #endif
    }

    bool execute_executable(fs::path const& path)
    {
    #ifdef _WIN32
        STARTUPINFOA si{};
        PROCESS_INFORMATION pi{};
        si.cb = sizeof(si);

        std::string cmd{ path.string() };

        if (!CreateProcessA(
            nullptr,
            cmd.data(),
            nullptr, nullptr,
            FALSE, 0, nullptr, nullptr,
            &si, &pi))
            return false;


        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;

    #else
        pid_t pid{ fork() };
        if (pid < 0)
        {
            print_formatted_error(ansi::withForeground("fork failed", ansi::Foreground::RED) + ": " + strerror(errno));
            return false;
        }

        if (pid == 0)
        {
            char* argv[]{ const_cast<char*>(command.c_str()), nullptr };
            execve(command.c_str(), argv, environ);
            _exit(127);
        }
        else
        {
            int status{};
            if (waitpid(pid, &status, 0) < 0)
            {
                print_formatted_error(ansi::withForeground("waitpid failed", ansi::Foreground::RED) + ": " + strerror(errno));
                return false;
            }

            if (WIFEXITED(status))
                return WEXITSTATUS(status) == 0 ? true : false;
            return false;
        }
    #endif
    }
}