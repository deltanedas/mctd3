#include "main/main.h"

std::function<void(void)> MCTD3_RawTick = std::function<void(void)>([](void){
	while (lastTime - SDL_GetTicks() < nextFrameTicks) {
		SDL_Delay(1);
	}
});

std::function<void(void)> MCTD3_Tick = std::function<void(void)>([](void){
	while (MCTD3_paused) {
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

bool MCTD3_ProcessEvents() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (!MCTD3_paused) {
		if (state[key_moveCameraUp]) {
			camera.Move(0.0, TileSize.Y / 128);
		} else if (state[key_moveCameraDown]) {
			camera.Move(0.0, -(TileSize.Y / 128));
		}
		if (state[key_moveCameraLeft]) {
			camera.Move(TileSize.X / 128, 0.0);
		} else if (state[key_moveCameraRight]) {
			camera.Move(-(TileSize.X / 128), 0.0);
		}
	}

	while (SDL_PollEvent(&event)) {
		if (!MCTD3_paused) {
			updateEvents();
		}
		switch (event.type) {
			case SDL_QUIT:
				MCTD3_closing = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						MCTD3_closing = true;
						break;
					case SDLK_r:
						MCTD3_closing = true;
						MCTD3_restarting = true;
						break;
					case SDLK_p:
						if (MCTD3_paused) {
							loggerf("Unpaused");
							MCTD3_paused = false;
						} else {
							loggerf("Paused");
							MCTD3_paused = true;
						}
				}
				if (!MCTD3_paused) {
					int key = event.key.keysym.sym;
					switch (event.key.keysym.sym) {
						case SDLK_c:
							loggerf("Manually clearing frames on screen.");
							cleanUpFrames();
							break;
						case SDLK_t:
							loggerf("Toggling frames that are visible.");
							for (Frame* frame : Frames) {
								frame->toggleVisible();
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
					if (key == key_toggleFullscreen) {
						loggerf("Toggled fullscreen.");
						if (MCTD3_fullscreen) {
							MCTD3_fullscreen = false;
							SDL_SetWindowFullscreen(window, SDL_WINDOW_RESIZABLE);
						} else {
							MCTD3_fullscreen = true;
							SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						}
						break;
					}
				}
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						loggerf("Resized window to X: " + std::to_string(event.window.data1) + ", Y: " + std::to_string(event.window.data2) + ".", Level::DEBUG);
						windowSize = Vec2(event.window.data1, event.window.data2);
						TileSize.X = (windowSize.Y / aspectRatio) / 10;
						TileSize.Y = (windowSize.X * aspectRatio) / 10;
						loggerf("Tile Size: " + TileSize.to_string(), Level::DEBUG);
						Vec2 oldBounds = cameraBounds;
						cameraBounds = Vec2(windowSize.X - TileSize.X * 3, windowSize.Y - TileSize.Y * 3);
						updateTilePositions();
						updateFrames();
						camera.X = (camera.X / oldBounds.X) * cameraBounds.X;
						camera.Y = (camera.Y / oldBounds.Y) * cameraBounds.Y;
						break;
				}
				break;
		}
	}
	return true;
}

bool MCTD3_Init() {
	bool optionsCode = MCTD3_LoadOptions();
	if (optionsCode) {
		bool initSDLCode = MCTD3_InitSDL();
		if (initSDLCode) {
			bool initUICode = MCTD3_InitUI();
			if (initUICode) {
				bool loopCode = MCTD3_StartLoops();
				if (loopCode) {
					bool closeCode = MCTD3_Close();
					if (closeCode) {
						if (MCTD3_restarting) {
							MCTD3_restarting = false;
							MCTD3_closing = false;
							return MCTD3_Init();
						}
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool MCTD3_Close() {
	if (MCTD3_EventLoop_running) {
		loggerf("Waiting for event loop to finish...");
		while (MCTD3_EventLoop_running) {
			SDL_Delay(1);
		}
	}

	if (MCTD3_RenderLoop_running) {
		loggerf("Waiting for render loop to finish...");
		while (MCTD3_RenderLoop_running) {
			SDL_Delay(1);
		}
	}

	if (MCTD3_LogicLoop_running) {
		loggerf("Waiting for logic loop to finish...");
		while (MCTD3_LogicLoop_running) {
			SDL_Delay(1);
		}
	}


	loggerf("MCTD3 shutting down.", Level::DEBUG);
	loggerf("\tCleaning up " + std::to_string(Tiles.size()) + " tile(s).", Level::DEBUG);
	loggerf("\t" + std::to_string(cleanUpTiles()) + " tile(s) cleaned up.", Level::DEBUG);
	loggerf("\tCleaning up " + std::to_string(Frames.size()) + " frame(s).", Level::DEBUG);
	loggerf("\t" + std::to_string(cleanUpFrames()) + " frame(s) cleaned up.", Level::DEBUG);
	if (window != nullptr) {
		loggerf("\tDestroying window.", Level::DEBUG);
		SDL_DestroyWindow(window);
	}
	if (renderer != nullptr) {
		loggerf("\tDestroying renderer.", Level::DEBUG);
		SDL_DestroyRenderer(renderer);
	}
	loggerf("\tQuitting SDL.", Level::DEBUG);
	loggerf("\t\tQuitting SDL2-image.", Level::DEBUG);
	IMG_Quit();
	loggerf("\t\tQuitting SDL2-ttf.", Level::DEBUG);
	TTF_Quit();
	loggerf("\t\tQuitting SDL2.", Level::DEBUG);
	SDL_Quit();
	loggerf("\tSaving options.", Level::DEBUG);
	MCTD3_SaveOptions();
	loggerf("MCTD3 has shut down.");
	return true;
}