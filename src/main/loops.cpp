#include "main/main.h"

bool MCTD3_EventLoop() {
	loggerf("Event loop started.");

	MCTD3_EventLoop_running = true;

	while (!MCTD3_closing) {
		MCTD3_RawTick();
		lastTime = SDL_GetTicks();
		MCTD3_ProcessEvents();
	}

	MCTD3_EventLoop_running = false;

	loggerf("Event loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_RenderLoop() {
	loggerf("Render loop started.");
	loggerf("Took " + std::to_string(SDL_GetTicks()) + "ms to enter render loop.", Level::DEBUG);

	MCTD3_RenderLoop_running = true;

	while (!MCTD3_closing) {
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

	MCTD3_RenderLoop_running = false;

	loggerf("Render loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_LogicLoop() {
	loggerf("Logic loop started.");

	MCTD3_LogicLoop_running = true;

	srand(time(0));
	int counter = 0;
	while (!MCTD3_closing) {
		MCTD3_Tick();
		if (RandomInt(1, 600) == 69) {
			counter++;
		}
	}

	MCTD3_LogicLoop_running = false;

	loggerf("Logic loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_StartLoops() {
	loggerf("Starting loops.");

	if (framerateCap == 0) {
		loggerf("Framerate has no cap.", Level::DEBUG);
		nextFrameTicks = 0;
		MCTD3_Tick = MCTD3_RawTick;
	} else {
		nextFrameTicks = 1000 / framerateCap;
	}

	std::thread renderThread(MCTD3_RenderLoop); renderThread.detach();
	//std::thread logicThread(MCTD3_LogicLoop); logicThread.detach();
	return MCTD3_EventLoop();
}