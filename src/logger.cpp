#include "logger.h"

std::map<ColourEnums, const char*> Colours = {
	{RESET, "\033[0m"},
	{BOLD, "\033[1m"},
	{BLACK, "\033[30m"},
	{RED, "\033[31m"},
	{GREEN, "\033[32m"},
	{YELLOW, "\033[33m"},
	{BLUE, "\033[34m"},
	{MAGENTA, "\033[35m"},
	{CYAN, "\033[36m"},
	{WHITE, "\033[37m"}
};

std::string GetTime() {
	time_t t = time(0);
	tm* now = localtime(&t);
	return std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);
}

void loggerf(std::string str, Level level) {
	std::string currentTime = GetTime();
	const char* now = currentTime.c_str();
	const char* text = str.c_str();
	switch (level) {
		case Level::DEBUG:
			if (SimpleUI_debug) {
				printf("[%sDEBUG%s] [%stick #%d%s] [%s%s%s]: %s\n",
					Colours[GREEN], Colours[RESET],
					Colours[MAGENTA], tickCount, Colours[RESET],
					Colours[GREEN], now, Colours[RESET],
					text);
			} break;
		case Level::INFO:
			printf("[%sINFO%s] [%stick #%d%s] [%s%s%s]: %s\n",
				Colours[CYAN], Colours[RESET],
				Colours[MAGENTA], tickCount, Colours[RESET],
				Colours[GREEN], now, Colours[RESET],
				text); break;
		case Level::WARN:
			printf("[%sWARN%s] [%stick #%d%s] [%s%s%s]: %s%s%s\n",
				Colours[YELLOW], Colours[RESET],
				Colours[MAGENTA], tickCount, Colours[RESET],
				Colours[GREEN], now, Colours[RESET],
				Colours[YELLOW], text, Colours[RESET]); break;
		case Level::ERROR:
			fprintf(stderr, "[%sERROR%s] [%stick #%d%s] [%s%s%s]: %s%s%s\n",
				Colours[RED], Colours[RESET],
				Colours[MAGENTA], tickCount, Colours[RESET],
				Colours[GREEN], now, Colours[RESET],
				Colours[RED], text, Colours[RESET]); break;
	}
}

void loggerf(int str, Level level) {
	loggerf(std::to_string(str), level);
}

void loggerf(double str, Level level) {
	loggerf(std::to_string(str), level);
}

void loggerf(float str, Level level) {
	loggerf(std::to_string(str), level);
}
