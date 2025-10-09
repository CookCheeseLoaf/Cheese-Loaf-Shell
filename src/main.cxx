#include <iostream>
#include <ranges>
#include <replxx.hxx>
#include <string>

#include "ANSI.hxx"
#include "ErrorPrinter.hxx"
#include "FileSystemUtils.hxx"
#include "REPL.hxx"
#include "StartupDirectory.hxx"
#include "ReservedWords.hxx"

#ifdef _WIN32
#include <windows.h>
#endif

static std::map<ReservedWords, std::function<CommandResult(const arguments&)>> g_commands;

static std::vector<replxx::Replxx::Completion> completionFunction(
	std::string const& input, int& contextLen)
{
	std::vector<replxx::Replxx::Completion> completions;
	contextLen = static_cast<int>(input.length());

	if (input.empty()) return completions;

	auto const transformFunc{ std::islower(input[0])
							 ? [](unsigned char const c) { return static_cast<unsigned char>(std::tolower(c)); }
	: [](unsigned char const c) { return static_cast<unsigned char>(std::toupper(c)); } };

	std::string finalInput{ input };
	std::ranges::transform(finalInput, finalInput.begin(), transformFunc);

	for (auto const& key : g_commands | std::views::keys)
	{
		std::string name{ reservedWordToString(key) };
		std::string transformedName{ name };
		std::ranges::transform(transformedName.begin(), transformedName.end(), transformedName.begin(), transformFunc);

		if (transformedName.rfind(finalInput, 0) == 0)
			completions.emplace_back(name.c_str());
	}
	return completions;
}

int main()
{
#ifdef _WIN32
	HANDLE hOut{ GetStdHandle(STD_OUTPUT_HANDLE) };
	DWORD dwMode{};
	GetConsoleMode(hOut, &dwMode);
	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
	try
	{
		ensure_startup_directory();
	}
	catch (fs::filesystem_error const& e)
	{
		print_formatted_error(ansi::withForeground("Warning", ansi::Foreground::YELLOW) + ": Could not set initial directory to home: " + e.what());
	}

	replxx::Replxx rexx{};
	REPL repl{};
	g_commands = repl.getCommands();
	rexx.set_completion_callback(completionFunction);

	std::ios::sync_with_stdio(false);
	repl("ver");
	std::cout.flush();

	while (true)
	{
		std::string const home{ FileSystemUtils::get_home_directory() };
		std::string directory{ REPL::get_dir() + ansi::withForeground(">>> ", ansi::Foreground::CYAN) };

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
