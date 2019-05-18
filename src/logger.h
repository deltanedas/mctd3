#ifndef FILE_LOGGER_INCLUDED
#define FILE_LOGGER_INCLUDED

#include <string>
#include <stdio.h>
#include <map>
#include "simpleui.h"

enum ColourEnums {
	RESET,
	BOLD,
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
};

extern std::map<ColourEnums, const char*> Colours;

std::string GetTime();

enum class Level {
	DEBUG,
	INFO,
	WARN,
	ERROR
};

void loggerf(std::string str, Level level = Level::INFO);

#endif
