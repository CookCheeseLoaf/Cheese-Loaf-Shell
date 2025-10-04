//
// Created by Loaf on 9/4/2025.
//

#ifndef SHELL_FILESYSTEMUTILS_HXX
#define SHELL_FILESYSTEMUTILS_HXX

#include <filesystem>
#include <string>


namespace fs = std::filesystem;

namespace FileSystemUtils
{
    std::string get_home_directory();
    bool is_executable(fs::path const& /* path */);
}

#endif //SHELL_FILESYSTEMUTILS_HXX