//
// Created by Loaf on 10/7/2025.
//

#include "StartupDirectory.hxx"

#include <iostream>

#include "FileSystemUtils.hxx"

void ensure_startup_directory()
{
    try
    {
        if (fs::path const cwd{ fs::current_path() }, root{ cwd.root_path() }; cwd == root)
        {
            if (std::string const home{ FileSystemUtils::get_home_directory() }; !home.empty() && home != ".")
            {
                fs::path home_path{ home };
                if (home_path.is_relative())
                    home_path = fs::absolute(home_path);

                home_path = fs::weakly_canonical(home_path);

                if (fs::exists(home_path) && fs::is_directory(home_path))
                    fs::current_path(home_path);
            }
        }
    }
    catch (fs::filesystem_error const& e)
    {
        std::cerr << "Startup: filesystem error while initializing working directory: " << e.what() << '\n';
    }
    catch (std::exception const& e)
    {
        std::cerr << "Startup: error while initializing working directory: " << e.what() << '\n';
    }
}
