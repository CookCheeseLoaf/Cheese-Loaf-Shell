#include <iostream>
#include <sstream>
#include <replxx.hxx>
#include <string>
#include "REPL.hxx"

int main()
{
	std::cout << R"(
	___   _____             _      _____ _                           _                  __   ___
   |  _| /  __ \           | |    /  __ \ |                         | |                / _| |_  |
   | |   | /  \/ ___   ___ | | __ | /  \/ |__   ___  ___  ___  ___  | |     ___   __ _| |_    | |
   | |   | |    / _ \ / _ \| |/ / | |   | '_ \ / _ \/ _ \/ __|/ _ \ | |    / _ \ / _` |  _|   | |
   | |   | \__/\ (_) | (_) |   <  | \__/\ | | |  __/  __/\__ \  __/ | |___| (_) | (_| | |     | |
   | |_   \____/\___/ \___/|_|\_\  \____/_| |_|\___|\___||___/\___| \_____/\___/ \__,_|_|    _| |
   |___|                                                                                    |___|)" << '\n';

	replxx::Replxx rexx{};
	REPL repl{};
	std::ios::sync_with_stdio(false);
	repl("ver");
	std::cout.flush();

	while (true)
	{
		const char* cinput{ rexx.input(REPL::get_dir() + "> ") };
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