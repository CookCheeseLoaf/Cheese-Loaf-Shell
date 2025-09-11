#ifndef ANSI_HXX
#define ANSI_HXX

#include <string>
#include <string_view>

namespace ansi
{
	enum class Foreground : unsigned char
	{
		BLACK = 30,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
		BRIGHT_BLACK = 90,
		BRIGHT_RED,
		BRIGHT_GREEN,
		BRIGHT_YELLOW,
		BRIGHT_BLUE,
		BRIGHT_MAGENTA,
		BRIGHT_CYAN,
		BRIGHT_WHITE
	};

	enum class Background : unsigned char
	{
		BLACK = 40,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
		BRIGHT_BLACK = 100,
		BRIGHT_RED,
		BRIGHT_GREEN,
		BRIGHT_YELLOW,
		BRIGHT_BLUE,
		BRIGHT_MAGENTA,
		BRIGHT_CYAN,
		BRIGHT_WHITE
	};

	inline constexpr std::string_view RESET{ "\033[0m" };
	inline constexpr std::string_view CLEAR_SCREEN{ "\033[2J\033[H" };

	inline std::string foreground(Foreground fg)
	{
		return "\033[" + std::to_string(static_cast<unsigned char>(fg)) + "m";
	}

	inline std::string background(Background bg)
	{
		return "\033[" + std::to_string(static_cast<unsigned char>(bg)) + "m";
	}
}
#endif