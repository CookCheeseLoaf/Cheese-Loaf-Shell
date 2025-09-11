#include <iostream>
#include <sstream>
#include <replxx.hxx>
#include <string>
#include "REPL.hxx"

int main()
{
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