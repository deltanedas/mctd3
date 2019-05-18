#include "main/main.h"

bool MCTD3_LoadOptions() {
	options["resX"] = 1280;
	options["resY"] = 720;
	options["fullscreen"] = 0;
	options["key_toggleFullscreen"] = SDLK_F11;
	options["key_moveCameraUp"] = SDL_SCANCODE_W;
	options["key_moveCameraLeft"] = SDL_SCANCODE_A;
	options["key_moveCameraDown"] = SDL_SCANCODE_S;
	options["key_moveCameraRight"] = SDL_SCANCODE_D;
	if (std::filesystem::exists("options.txt")) {
		std::ifstream optionsFile("options.txt");
		if (!optionsFile) {
			throw std::runtime_error("Failed to read existing options.txt file.");
			return false;
		}
		std::string line;
		while (std::getline(optionsFile, line)) {
			std::istringstream iss(line);
			std::string option;
			int value;
			if (!(iss >> option >> value)) {
				loggerf("Failed to read option line '" + line + "'.", Level::WARN);
				continue;
			}
			if (option == "resX") {
				if (value > 0) {
					options["resX"] = value;
					windowSize.X = value;
					loggerf("Window width set to " + std::to_string(value), Level::DEBUG);
				} else {
					loggerf("Window width must be greater than 0.", Level::WARN);
				}
			} else if (option == "resY") {
				if (value > 0) {
					options["resY"] = value;
					windowSize.Y = value;
					loggerf("Window height set to " + std::to_string(value), Level::DEBUG);
				} else {
					loggerf("Window height must be greater than 0.", Level::WARN);
				}
			} else if (option == "fullscreen") {
				MCTD3_fullscreen = (value == 1 ? 1 : 0);
				options["fullscreen"] = MCTD3_fullscreen;
			} else if (option == "key_toggleFullscreen") {
				options["key_toggleFullscreen"] = value;
				key_toggleFullscreen = value;
			} else if (option == "key_moveCameraUp") {
				options["key_moveCameraUp"] = value;
				key_moveCameraUp = value;
			} else if (option == "key_moveCameraLeft") {
				options["key_moveCameraLeft"] = value;
				key_moveCameraLeft = value;
			} else if (option == "key_moveCameraDown") {
				options["key_moveCameraDown"] = value;
				key_moveCameraDown = value;
			} else if (option == "key_moveCameraRight") {
				options["key_moveCameraRight"] = value;
				key_moveCameraRight = value;
			} else {
				loggerf("Unknown option '" + option + "'.", Level::WARN);
			}
		}
	}
	return true;
}

bool MCTD3_SaveOptions() {
	std::ofstream optionsFile("options.txt");
	if (optionsFile) {
		loggerf("Writing options to options.txt.", Level::DEBUG);
		for (auto const &it : options) {
			loggerf("\tOption '" + it.first + "' = " + std::to_string(it.second), Level::DEBUG);
			optionsFile << it.first + " " + std::to_string(it.second) << std::endl ;
		}
	} else {
		loggerf("Failed to write options to options.txt: " + std::to_string(errno), Level::ERROR);
	}
	return true;
}