#include <iostream>
#include <sstream>
#include <replxx.hxx>
#include <string>
#include "REPL.hxx"

int main()
{
	std::cout << R"(
   _____ _                           _                  __    _____ _          _ _
  / ____| |                         | |                / _|  / ____| |        | | |
 | |    | |__   ___  ___  ___  ___  | |     ___   __ _| |_  | (___ | |__   ___| | |
 | |    | '_ \ / _ \/ _ \/ __|/ _ \ | |    / _ \ / _` |  _|  \___ \| '_ \ / _ \ | |
 | |____| | | |  __/  __/\__ \  __/ | |___| (_) | (_| | |    ____) | | | |  __/ | |
  \_____|_| |_|\___|\___||___/\___| |______\___/ \__,_|_|   |_____/|_| |_|\___|_|_|)" << '\n';

	replxx::Replxx rexx{};
	REPL repl{};
	std::ios::sync_with_stdio(false);
	repl("ver");
	std::cout.flush();

	while (true)
	{
		const char* cinput{ rexx.input("cls " + REPL::get_dir() + "> ") };
		if (!cinput)
			break;

		std::string input{ cinput };
		if (input.empty())
			continue;

		rexx.history_add(input);

		if (!repl(input))
		{
			std::string cmd;
			std::istringstream{ input } >> cmd;
			std::cerr << "Illegal command: " << cmd << '\n';
		}
		else
			std::cout.flush();
	}
}