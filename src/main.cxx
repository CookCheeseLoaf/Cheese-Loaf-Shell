#include <iostream>
#include <replxx.hxx>
#include <string>

#include "ANSI.hxx"
#include "FileSystemUtils.hxx"
#include "REPL.hxx"
#include "StartupDirectory.hxx"

int main()
{
	try
	{
		ensure_startup_directory();
	}
	catch (fs::filesystem_error const& e)
	{
		std::cerr << "Warning: Could not set initial directory to home: " << e.what() << '\n';
	}

	replxx::Replxx rexx{};
	REPL repl{};
	std::ios::sync_with_stdio(false);
	repl("ver");
	std::cout.flush();

	while (true)
	{
		std::string const home{ FileSystemUtils::get_home_directory() };
		std::string directory{ REPL::get_dir() + ansi::withForeground(">>> ", ansi::Foreground::MAGENTA) };

		if (directory.find(home) == 0)
		{
			directory = ansi::withForeground(home, ansi::Foreground::GREEN)
					  + directory.substr(home.size());
		}

		char const* cinput{ rexx.input(directory) };
		if (!cinput)
			break;

		std::string const input{ cinput };
		if (input.empty())
			continue;

		rexx.history_add(input);

		bool const result{ repl(input) };
		std::cout.flush();

		if (!result) {}
	}

	return 0;
}
