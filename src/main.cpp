#include <iostream>
#include "main/main.h"

bool ricardoFocused = false;

void ricardoFunc(EventType eventType) {
	if (ricardoFocused) {
		SDL_KeyboardEvent e = eventType.KeyboardEvent;
		SimpleUI_Log(std::to_string(e.keysym.sym));
	}
}

void ricardoFocus(EventType eventType) {
	SDL_MouseButtonEvent e = eventType.MouseButtonEvent;
	SimpleUI_Log("Mouse Up.", Level::DEBUG);
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
	SimpleUI_Log("Yo ive been clicked.");
	if (e.button == 1) {
		if (eventType.Caller->isPointInBounds(e.x, e.y, true)) {
			mineCount[eventType.Caller]++;
			eventType.Caller->nextAnimationFrame();
			if (mineCount[eventType.Caller] == 10) {
				label->getText()->setText("Good job!");
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
	printf("\t-u [--uiscript] <file> - Sets UI Lua script to the file, is 'ui.lua' by default.\n");
	printf("\t-h [--help] - Print this help message.\n");
}

int main(int argc, char* argv[]) {
	SimpleUI_Log(Title);
	if (argc > 1) {
		bool failed = false;
		int skip = 0;
		for (int i = 1; i < argc; i++) {
			std::string param = std::string(argv[i]);
			if ((param.compare("-d") == 0) || (param.compare("--debug") == 0)) {
				SimpleUI_debug = true;
				continue;
			}
			if ((param.compare("-u") == 0) || (param.compare("--uiscript") == 0)) {
				skip++;
				if (argc > i) {
					uiScriptPath = std::string(argv[i + 1]);
				} else {
					printHelp();
					return 1;
				}
				continue;
			}
			if ((param.compare("-h") == 0) || (param.compare("--help") == 0)) {
				printHelp();
				return 0;
			}
			if (failed) {
				failed = false;
				SimpleUI_Log("Use '" + std::string(argv[0]) + " --help' for help with parameters.", Level::WARN);
			}
			if (skip > 0) {
				skip--;
				continue;
			}
			SimpleUI_Log("Unknown parameter: " + param, Level::WARN);
			failed = true;
		}
	}
	SimpleUI_Log("SDL_FontCache version: " + FC_GetVersion(), Level::DEBUG);
	try {
		bool initCode = MCTD3_Init();
		if (initCode == false) {
			SimpleUI_Log("MCTD3 abnormally shut down.", Level::WARN);
			return 1;
		}
		return 0;
	} catch (std::exception& e) {
		SimpleUI_Log("Exception caught: " + std::string(e.what()), Level::ERROR);
		return 1;
	}
}
