#include <iterator>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <random>
#include <chrono>
#include "mctd3.h"

int framerateCap = 60;
std::string Title = "Minecraft Tower Defense III";
int nextFrameTicks;
int lastTime = 0;
std::map<std::string, int> options;

std::function<void(void)> MCTD3_RawTick = std::function<void(void)>([](void){
	while (lastTime - SDL_GetTicks() < nextFrameTicks) {
		SDL_Delay(1);
	}
});

std::function<void(void)> MCTD3_Tick = std::function<void(void)>([](void){
	while (isPaused) {
		SDL_Delay(nextFrameTicks);
	}
	MCTD3_RawTick();
});

int RandomInt(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

bool MCTD3_getOptions() {
	options["resX"] = 1280;
	options["resY"] = 720;
	options["fullscreenDefault"] = 0;
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
			if (option == "res_x") {
				if (value > 0) {
				options["resX"] = value;
				loggerf("Window width set to " + std::to_string(value), Level::DEBUG);
				} else {
					loggerf("Window width must be greater than 0.", Level::WARN);
				}
			} else if (option == "res_y") {
				if (value > 0) {
					options["resY"] = value;
					loggerf("Window height set to " + std::to_string(value), Level::DEBUG);
				} else {
					loggerf("Window height must be greater than 0.", Level::WARN);
				}
			} else if (option == "fullscreen") {
				options["fullscreenDefault"] = (value == 1 ? 1 : 0);
			} else if (option == "key_toggle_fullscreen") {
				options["key_toggleFullscreen"] = value;
			} else if (option == "key_move_camera_up") {
				options["key_moveCameraUp"] = value;
			} else if (option == "key_move_camera_left") {
				options["key_moveCameraLeft"] = value;
			} else if (option == "key_move_camera_down") {
				options["key_moveCameraDown"] = value;
			} else if (option == "key_move_camera_right") {
				options["key_moveCameraRight"] = value;
			}
		}
	}
	return true;
}

void MCTD3_saveOptions() {

}

bool MCTD3_initSDL() {
	loggerf("Initialising SDL.", Level::DEBUG);
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		if (SDL_CreateWindowAndRenderer(options["resX"], options["resY"], SDL_WINDOW_RESIZABLE, &window, &renderer) == 0) {
			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
				loggerf("SDL Image failed to intialise: " + std::string(IMG_GetError()), Level::ERROR);
				return false;
			}
			if (TTF_Init() == -1) {
				loggerf("SDL TTF failed to intialise: " + std::string(TTF_GetError()), Level::ERROR);
				return false;
			}
			screenSurface = SDL_GetWindowSurface(window);
			loggerf("Screen surface got from window.", Level::DEBUG);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			loggerf("Renderer blend mode set.", Level::DEBUG);
			SDL_SetWindowTitle(window, Title.c_str());
			loggerf("Window title set.", Level::DEBUG);
			SDL_SetHint("_NET_WM_BYPASS_COMPOSITOR", "2");
			SDL_ClearError();
			SDL_Surface* iconSurface = IMG_Load("assets/icon.png");
			if (iconSurface == nullptr) {
				throw std::runtime_error("Icon failed to load: " + std::string(SDL_GetError()) + "\n\tAre you running from assets' parent directory?");
			}
			SDL_SetWindowIcon(window, iconSurface);
			loggerf("Window icon set.", Level::DEBUG);
			if (options["fullscreenDefault"] == 1) {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				loggerf("Entered fullscreen.");
			}
			SDL_DisplayMode dm;
			SDL_ClearError();
			if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
				throw std::runtime_error("Failed to get SDL display mode: " + std::string(SDL_GetError()));
			}
			screenSize.X = dm.w;
			screenSize.Y = dm.h;
			loggerf("Your screen size is " + std::to_string(screenSize.X) + "x" + std::to_string(screenSize.Y) + ".", Level::DEBUG);
			aspectRatio = screenSize.Y / screenSize.X;
			loggerf("Aspect ratio is " + std::to_string(aspectRatio) + ".", Level::DEBUG);
			TileSize.X = (options["resY"] * aspectRatio) / 10;
			TileSize.Y = (options["resX"] / aspectRatio) / 10;
			loggerf("Tile size is " + std::to_string(options["resY"] * aspectRatio) + "x" + std::to_string(options["resX"] / aspectRatio) + ".", Level::DEBUG);
			int windowX;
			int windowY;
			SDL_GetWindowSize(window, &windowX, &windowY);
			windowSize = Vec2(windowX, windowY);
			cameraBounds = Vec2(options["resX"] - TileSize.X * 3, options["resY"] - TileSize.Y * 3);
			loggerf("SDL initialised.");
			return true;
		} else {
			loggerf("SDL failed to create window and/or renderer: " + std::string(SDL_GetError()), Level::ERROR);
			return false;
		}
	} else {
		loggerf("SDL failed to initialise: " + std::string(SDL_GetError()), Level::ERROR);
		return false;
	}
}

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
Tile* ore;
Frame* label;
std::string defaultLabelText = "Left click 8 times to mine block.\nWhen you are done press ESC.";

void resetText(int count) {
	std::this_thread::sleep_for(std::chrono::milliseconds(count * 1000));
	label->getText()->setText(defaultLabelText);
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
				std::thread resetThread(resetText, 5); resetThread.detach();
			}
		}
	}
}


bool MCTD3_initUI() {
	loggerf("Initialising UI.", Level::DEBUG);
	initTextures();
	FC_Font* minecraftFont = FC_CreateFont();
	SDL_Color color = FC_MakeColor(0, 0, 0, 255);
	FC_LoadFont(minecraftFont, renderer, "assets/minecraftia.ttf", 24, color, TTF_STYLE_NORMAL);
	Tile* stone = new Tile("stone");
	Tile* stone2 = new Tile("stone");
	stone2->setPosition(Vec2(1, 0));
	Tile* stone3 = new Tile("stone");
	stone3->setPosition(Vec2(2, 0));
	Tile* stone4 = new Tile("stone");
	stone4->setPosition(Vec2(0, 1));
	ore = new Tile("diamond_ore");
	ore->setPosition(Vec2(1, 1));
	Tile* stone5 = new Tile("stone");
	stone5->setPosition(Vec2(2, 1));
	Tile* stone6 = new Tile("stone");
	stone6->setPosition(Vec2(0, 2));
	Tile* stone7 = new Tile("stone");
	stone7->setPosition(Vec2(1, 2));
	Tile* stone8 = new Tile("stone");
	stone8->setPosition(Vec2(2, 2));

	for (Tile* tile : Tiles) {
		tile->getFrame()->setVisible(true);
		tile->getFrame()->setEventCallback(EventEnum::MOUSE_DOWN, oreDigCallback);
	}

	label = new Frame();
		SizeType labelSize;
		labelSize.setScale(Vec2(0.4, 0.1));
	label->setSize(labelSize);
		SizeType labelPos;
		labelPos.setScale(Vec2(0.3, 0.45));
	label->setPosition(labelPos);
		TextType* labelText = new TextType(minecraftFont, defaultLabelText);
		labelText->setScaleX(true);
		labelText->setScaleY(true);
	label->setText(labelText);
		ColourType* labelColour = new ColourType(128, 255, 128, 128);
	label->setColour(labelColour);
	label->setVisible(true);

	loggerf("UI initialised.");
	return true;
}

void MCTD3_processEvents() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (!isPaused) {
		if (state[options["key_moveCameraUp"]]) {
			camera.Move(0.0, TileSize.Y / 128);
		} else if (state[options["key_moveCameraDown"]]) {
			camera.Move(0.0, -(TileSize.Y / 128));
		}
		if (state[options["key_moveCameraLeft"]]) {
			camera.Move(TileSize.X / 128, 0.0);
		} else if (state[options["key_moveCameraRight"]]) {
			camera.Move(-(TileSize.X / 128), 0.0);
		}
	}

	while (SDL_PollEvent(&event)) {
		if (!isPaused) {
			updateEvents();
		}
		switch (event.type) {
			case SDL_QUIT:
				closing = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						closing = true;
						break;
					case SDLK_r:
						closing = true;
						restarting = true;
						break;
					case SDLK_p:
						if (isPaused) {
							loggerf("Unpaused");
							isPaused = false;
						} else {
							loggerf("Paused");
							isPaused = true;
						}
				}
				if (!isPaused) {
					switch (event.key.keysym.sym) {
						case SDLK_c:
							loggerf("Manually clearing frames on screen.");
							cleanUpFrames();
							break;
						case SDLK_t:
							loggerf("Toggling frames that are visible.");
							for (Frame* frame : Frames) {
								frame->toggleVisible();
								loggerf("john", Level::DEBUG);
							}
							break;
						case SDLK_y:
							loggerf("Toggling frames that are anchored.");
							for (Frame* frame : Frames) {
								frame->toggleAnchored();
							}
							break;
						case SDLK_h:
							loggerf("Resetting camera.");
							camera.X = 0;
							camera.Y = 0;
							break;
					}
				}
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						loggerf("Resized window to X: " + std::to_string(event.window.data1) + ", Y: " + std::to_string(event.window.data2) + ".", Level::DEBUG);
						windowSize = Vec2(event.window.data1, event.window.data2);
						TileSize.X = (windowSize.X * aspectRatio) / 10;
						TileSize.Y = (windowSize.Y / aspectRatio) / 10;
						Vec2 oldBounds = cameraBounds;
						cameraBounds = Vec2(windowSize.X - TileSize.X * 3, windowSize.Y - TileSize.Y * 3);
						loggerf(cameraBounds.to_string());
						updateTilePositions();
						updateFrames();
						camera.X = (camera.X / oldBounds.X) * cameraBounds.X;
						camera.Y = (camera.Y / oldBounds.Y) * cameraBounds.Y;
						break;
					/*case SDL_WINDOWEVENT_MINIMIZED:
						loggerf("Minimised window.", Level::DEBUG);
						updateFrames();
						break;
					case SDL_WINDOWEVENT_RESTORED:
						loggerf("Restored window.", Level::DEBUG);
						updateFrames();
						break;
					case SDL_WINDOWEVENT_MAXIMIZED:
						loggerf("Maximised window.", Level::DEBUG);
						TileSize = event.window.data1 / 10;
						cameraBounds = Vec2(options["resX"] - TileSize, options["resY"] - TileSize);
						updateTilePositions();
						updateFrames();
						break;*/
				}
				break;
			}
	}
}

bool MCTD3_eventLoop() {
	loggerf("Event loop started.");
	while (!closing) {
		MCTD3_RawTick();
		lastTime = SDL_GetTicks();
		MCTD3_processEvents();
	}
	loggerf("Event loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_renderLoop() {
	loggerf("Render loop started.");

	loggerf("Took " + std::to_string(SDL_GetTicks()) + "ms to enter render loop.", Level::DEBUG);

	while (!closing) {
		MCTD3_Tick();
		tickCount++;

		SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
		SDL_RenderClear(renderer);
		for (Frame* frame : VisibleFrames) {
			Vec2 size = frame->getAbsoluteSize();
			Vec2 pos = frame->getAbsolutePosition();
			ColourType* colour = frame->getColour();
			if (!frame->getAnchored()) {
				pos -= Vec2(camera.X, camera.Y);
			}
			SDL_Rect* rect = new SDL_Rect();
			if (size.X < 1) size.X = 1;
			if (size.Y < 1) size.Y = 1;
			rect->x = pos.X;
			rect->y = pos.Y;
			rect->w = size.X;
			rect->h = size.Y;

			SDL_SetRenderDrawColor(renderer, colour->getR(), colour->getG(), colour->getB(), colour->getA());
			SDL_RenderFillRect(renderer, rect);

			if (frame->getTexture() != nullptr) {
				SDL_Rect* textureRect = new SDL_Rect();
				TextureType* texture = frame->getTexture();
				size.X *= texture->getScale().X;
				size.Y *= texture->getScale().Y;
				if (size.X < 1) size.X = 1;
				if (size.Y < 1) size.Y = 1;
				textureRect->x = pos.X;
				textureRect->y = pos.Y;
				textureRect->w = size.X;
				textureRect->h = size.Y;
				SDL_ClearError();
				if (SDL_RenderCopy(renderer, texture->getTexture(), NULL, textureRect) != 0) {
					loggerf("Failed to render texture: " + std::string(SDL_GetError()), Level::WARN);
				}
			}

			if (frame->getText() != nullptr) {
				if (frame->getText()->getFont() != nullptr) {
					TextType* text = frame->getText();
					FC_Font* font = text->getFont();
					Vec2 textSize = text->getSize();
					if (size.X < 1) size.X = 1;
					if (size.Y < 1) size.Y = 1;
					FC_Scale scale;
					scale.x = 1;
					scale.y = 1;
					if (text->getScaleX() == true) {
						scale.x = size.X / textSize.X;
					}
					if (text->getScaleY() == true) {
						scale.y = size.Y / textSize.Y;
					}
					FC_DrawScale(font, renderer, pos.X, pos.Y, scale, text->getText());
				}
			}
		}

		SDL_RenderPresent(renderer);
	}
	loggerf("Render loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_logicLoop() {
	loggerf("Logic loop started.");
	srand(time(0));
	int counter = 0;
	while (!closing) {
		MCTD3_Tick();
		if (RandomInt(1, 600) == 69) {
			counter++;
		}
	}
	loggerf("Logic loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_loop() {
	loggerf("Starting loops.");

	if (framerateCap == 0) {
		loggerf("Framerate has no cap.", Level::DEBUG);
		nextFrameTicks = 0;
		MCTD3_Tick = MCTD3_RawTick;
	} else {
		nextFrameTicks = 1000 / framerateCap;
	}

	std::thread renderThread(MCTD3_renderLoop); renderThread.detach();
	//std::thread logicThread(MCTD3_logicLoop); logicThread.detach();
	return MCTD3_eventLoop();
}

bool MCTD3_close() {
	loggerf("MCTD3 shutting down.", Level::DEBUG);
	loggerf("\tCleaning up " + std::to_string(Tiles.size()) + " tile(s).", Level::DEBUG);
	loggerf("\t" + std::to_string(cleanUpTiles()) + " tile(s) cleaned up.", Level::DEBUG);
	loggerf("\tCleaning up " + std::to_string(Frames.size()) + " frame(s).", Level::DEBUG);
	loggerf("\t" + std::to_string(cleanUpFrames()) + " frame(s) cleaned up.", Level::DEBUG);
	if (window) {
		loggerf("\tDestroying window.", Level::DEBUG);
		SDL_DestroyWindow(window);
	}
	if (renderer) {
		loggerf("\tDestroying renderer.", Level::DEBUG);
		SDL_DestroyRenderer(renderer);
	}
	loggerf("\tQuitting SDL.", Level::DEBUG);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	loggerf("\tSaving options.", Level::DEBUG);
	MCTD3_saveOptions();
	loggerf("MCTD3 has shut down.");
	return true;
}

bool MCTD3_init() {
	bool optionsCode = MCTD3_getOptions();
	if (optionsCode) {
		bool initSDLCode = MCTD3_initSDL();
		if (initSDLCode) {
			bool initUICode = MCTD3_initUI();
			if (initUICode) {
				bool loopCode = MCTD3_loop();
				if (loopCode) {
					bool closeCode = MCTD3_close();
					if (closeCode) {
						if (restarting) {
							restarting = false;
							closing = false;
							return MCTD3_init();
						}
						return true;
					}
				}
			}
		}
	}
	return false;
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
				isDebug = true;
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
		bool initCode = MCTD3_init();
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
