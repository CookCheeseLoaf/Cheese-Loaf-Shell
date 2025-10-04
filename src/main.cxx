#include <iostream>
#include <sstream>
#include <replxx.hxx>
#include <string>

#include "ANSI.hxx"
#include "FileSystemUtils.hxx"
#include "REPL.hxx"

int main()
{
	try
	{
		const std::string home = FileSystemUtils::get_home_directory();
		fs::current_path(home);
	}
	catch (const fs::filesystem_error& e)
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
		const std::string home = FileSystemUtils::get_home_directory();
		std::string directory = REPL::get_dir() + ansi::withForeground(">>> ", ansi::Foreground::MAGENTA);
		if (directory.find(home) == 0)
			directory = ansi::withForeground(home, ansi::Foreground::GREEN) +
						directory.substr(home.size());

		const char* cinput = rexx.input(directory);
		if (!cinput)
			break;

		std::string input = cinput;
		if (input.empty())
			continue;

		rexx.history_add(input);

		if (!repl(input))
		{
			std::cout.flush();
		}
		else
		{
			std::cout.flush();
		}
	}

	return 0;
}