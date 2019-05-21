#include <iostream>
#include "main/main.h"

bool ricardoFocused = false;

void ricardoFunc(EventType eventType) {
	if (ricardoFocused) {
		SDL_KeyboardEvent e = eventType.KeyboardEvent;
		loggerf(std::to_string(e.keysym.sym));
	}
}

void ricardoFocus(EventType eventType) {
	SDL_MouseButtonEvent e = eventType.MouseButtonEvent;
	loggerf("Mouse Up.", Level::DEBUG);
	if (e.button == 1) {
		ricardoFocused = false;
		if (eventType.Caller->isPointInBounds(e.x, e.y, true)) {
			ricardoFocused = true;
		}
	}
}

std::map<Frame*, int> mineCount;
std::string defaultLabelText = "Left click 8 times to mine block.\nWhen you are done press ESC.";

void setText(std::string text, int count) {
	std::this_thread::sleep_for(std::chrono::milliseconds(count * 1000));
	label->getText()->setText(text);
}

void oreDigCallback(EventType eventType) {
	SDL_MouseButtonEvent e = eventType.MouseButtonEvent;
	if (e.button == 1) {
		if (eventType.Caller->isPointInBounds(e.x, e.y, true)) {
			mineCount[eventType.Caller]++;
			if (mineCount[eventType.Caller] == 8) {
				label->getText()->setText("You are get diamonde!");
				eventType.Caller->setEventCallback(EventEnum::MOUSE_DOWN);
				eventType.Caller->setVisible(false);
				std::thread resetThread(setText, defaultLabelText, 5); resetThread.detach();
			}
		}
	}
}

void printHelp() {
	printf("Command Line Options:");
	printf("\t-d [--debug] - Print extra debug information to the log.\n");
	printf("\t-h [--help] - Print this help message.\n");
}

int main(int argc, char* argv[]) {
	loggerf(Title);
	if (argc > 1) {
		bool failed = true;
		for (int i = 1; i < argc; i++) {
			std::string param = std::string(argv[i]);
			if ((param.compare("-d") == 0) || (param.compare("--debug") == 0)) {
				MCTD3_debug = true;
				continue;
			}
			if ((param.compare("-h") == 0) || (param.compare("--help") == 0)) {
				printHelp();
				return 0;
			}
			if (failed) {
				failed = false;
				loggerf("Use '" + std::string(argv[0]) + " --help' for help with parameters.", Level::WARN);
			}
			loggerf("Unknown parameter: " + param, Level::WARN);
		}
	}
	loggerf("SDL_FontCache version: " + FC_GetVersion(), Level::DEBUG);
	try {
		bool initCode = MCTD3_Init();
		if (initCode == false) {
			loggerf("MCTD3 abnormally shut down.", Level::WARN);
			return 1;
		}
		return 0;
	} catch (std::exception& e) {
		loggerf("Exception caught: " + std::string(e.what()), Level::ERROR);
		return 1;
	}
}