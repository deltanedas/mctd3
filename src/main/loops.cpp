#include "main/main.h"

bool MCTD3_EventLoop() {
	SimpleUI_Log("Event loop started.");

	MCTD3_EventLoop_running = true;

	while (!MCTD3_closing) {
		MCTD3_RawTick();
		lastTime = SDL_GetTicks();
		MCTD3_ProcessEvents();
	}

	MCTD3_EventLoop_running = false;

	SimpleUI_Log("Event loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_RenderLoop() {
	SimpleUI_Log("Render loop started.");
	SimpleUI_Log("Took " + std::to_string(SDL_GetTicks()) + "ms to enter render loop.", Level::DEBUG);

	MCTD3_RenderLoop_running = true;

	while (!MCTD3_closing) {
		MCTD3_Tick();
		tickCount++;

		SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
		SDL_RenderClear(renderer);
		for (Frame* frame : VisibleFrameInstances) {
			SimpleUI_Log("Seg");
			Vec2 size = frame->getAbsoluteSize();
			Vec2 pos = frame->getAbsolutePosition();
			double rotation = frame->getRotation();
			SDL_Point pivot_p = {(int) frame->getAbsolutePivot().X, (int) frame->getAbsolutePivot().Y};
			const SDL_Point* pivot = const_cast<SDL_Point*>(&pivot_p);
			if (!frame->getAnchored()) {
				pos -= Vec2(camera.X, camera.Y);
			}
			if (size.X < 1) size.X = 1;
			if (size.Y < 1) size.Y = 1;
			SimpleUI_Log("fault");

			if (frame->getTexture()) {
				SimpleUI_Log("yup");
				SDL_ClearError();
				SDL_Rect* textureRect = new SDL_Rect();
				TextureType* texture = frame->getTexture();
				SimpleUI_Log("thats me");
				size.X *= texture->getScale().X;
				size.Y *= texture->getScale().Y;
				if (size.X < 1) size.X = 1;
				if (size.Y < 1) size.Y = 1;
				textureRect->x = pos.X;
				textureRect->y = pos.Y;
				textureRect->w = size.X;
				textureRect->h = size.Y;
				SimpleUI_Log("You might");
				SDL_ClearError();
				if (SDL_RenderCopyEx(renderer, texture->getTexture(), NULL, textureRect, rotation, pivot, SDL_FLIP_NONE) != 0) {
					SimpleUI_Log("Failed to render texture: " + std::string(SDL_GetError()), Level::WARN);
				}
				delete textureRect;
			}
			SimpleUI_Log("actually its not");

			if (frame->getText() != nullptr) {
				SimpleUI_Log("dddactually its not");
				if (frame->getText()->getFont() != nullptr) {
					SimpleUI_Log("eeeactually its not");
					SDL_ClearError();
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
			SimpleUI_Log("kek");

			if (frame->getAnimation() != nullptr) {
				TextureType* texture = frame->getAnimation()->getFrames()[frame->getAnimationFrame()];
				if (texture != nullptr) {
					SDL_ClearError();
					SDL_Rect* textureRect = new SDL_Rect();
					size.X *= texture->getScale().X;
					size.Y *= texture->getScale().Y;
					if (size.X < 1) size.X = 1;
					if (size.Y < 1) size.Y = 1;
					textureRect->x = pos.X;
					textureRect->y = pos.Y;
					textureRect->w = size.X;
					textureRect->h = size.Y;
					SDL_ClearError();
					if (SDL_RenderCopyEx(renderer, texture->getTexture(), NULL, textureRect, rotation, pivot, SDL_FLIP_NONE) != 0) {
						SimpleUI_Log("Failed to render animation frame: " + std::string(SDL_GetError()), Level::WARN);
					}
					delete textureRect;
				}
			}
		}

		SDL_RenderPresent(renderer);
	}

	MCTD3_RenderLoop_running = false;

	SimpleUI_Log("Render loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_LogicLoop() {
	SimpleUI_Log("Logic loop started.");

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

	SimpleUI_Log("Logic loop finished.", Level::DEBUG);
	return true;
}

bool MCTD3_StartLoops() {
	SimpleUI_Log("Starting loops.");

	if (framerateCap == 0) {
		SimpleUI_Log("Framerate has no cap.", Level::DEBUG);
		nextFrameTicks = 0;
		MCTD3_Tick = MCTD3_RawTick;
	} else {
		nextFrameTicks = 1000 / framerateCap;
	}

	std::thread renderThread(MCTD3_RenderLoop); renderThread.detach();
	//std::thread logicThread(MCTD3_LogicLoop); logicThread.detach();
	return MCTD3_EventLoop();
}
