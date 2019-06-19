#include "main/main.h"
#include "luawrapper.h"

FC_Font* CreateFont(std::string name) {
	FC_Font* ret = FC_CreateFont();
	if (name.compare("") != 0) {
		fonts[name] = ret;
	}
	return ret;
}

bool MCTD3_InitSDL() {
	SimpleUI_Log("Initialising SDL.", Level::DEBUG);
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		if (SDL_CreateWindowAndRenderer(windowSize.X, windowSize.Y, SDL_WINDOW_RESIZABLE, &window, &renderer) == 0) {
			if (MCTD3_fullscreen) {
				SimpleUI_Log("Entering fullscreen.", Level::DEBUG);
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
				SimpleUI_Log("SDL Image failed to intialise: " + std::string(IMG_GetError()), Level::ERROR);
				return false;
			}
			if (TTF_Init() == -1) {
				SimpleUI_Log("SDL TTF failed to intialise: " + std::string(TTF_GetError()), Level::ERROR);
				return false;
			}
			screenSurface = SDL_GetWindowSurface(window);
			SimpleUI_Log("Screen surface got from window.", Level::DEBUG);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SimpleUI_Log("Renderer blend mode set.", Level::DEBUG);
			SDL_SetWindowTitle(window, Title.c_str());
			SimpleUI_Log("Window title set.", Level::DEBUG);
			SDL_SetHint("_NET_WM_BYPASS_COMPOSITOR", "2");
			SDL_ClearError();
			SDL_Surface* iconSurface = IMG_Load("assets/icon.png");
			if (iconSurface == nullptr) {
				throw std::runtime_error("Icon failed to load: " + std::string(SDL_GetError()) + "\n\tAre you running from assets' parent directory?");
			}
			SDL_SetWindowIcon(window, iconSurface);
			SimpleUI_Log("Window icon set.", Level::DEBUG);
			SDL_DisplayMode dm;
			SDL_ClearError();
			if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
				throw std::runtime_error("Failed to get SDL display mode: " + std::string(SDL_GetError()));
			}
			screenSize.X = dm.w;
			screenSize.Y = dm.h;

			aspectRatio = screenSize.Y / screenSize.X;

			TileSize.X = (options["resY"] / aspectRatio) / 10;
			TileSize.Y = (options["resX"] * aspectRatio) / 10;
			int windowX;
			int windowY;
			SDL_GetWindowSize(window, &windowX, &windowY);
			windowSize = Vec2(windowX, windowY);
			cameraBounds = Vec2(options["resX"] - TileSize.X * 3, options["resY"] - TileSize.Y * 3);
			SimpleUI_Log("SDL initialised.");
			return true;
		} else {
			SimpleUI_Log("SDL failed to create window and/or renderer: " + std::string(SDL_GetError()), Level::ERROR);
			return false;
		}
	} else {
		SimpleUI_Log("SDL failed to initialise: " + std::string(SDL_GetError()), Level::ERROR);
		return false;
	}
}

Frame* label;

void oreDigCallback(EventType eventType);

bool MCTD3_InitUI() {
	SimpleUI_Log("Initialising UI.", Level::DEBUG);
	initTextures();
	FC_Font* minecraftFont = CreateFont("minecraftia");
	SDL_Color color = FC_MakeColor(0, 255, 0, 255);
	Uint8 res = FC_LoadFont(minecraftFont, renderer, "assets/minecraftia.ttf", 24, color, TTF_STYLE_NORMAL);
	SimpleUI_Log("Loaded font: " + std::to_string(res));
	Tile* stone = new Tile("stone");
	Tile* stone2 = new Tile("stone");
	stone2->setPosition(Vec2(1, 0));
	Tile* stone3 = new Tile("stone");
	stone3->setPosition(Vec2(2, 0));
	Tile* stone4 = new Tile("stone");
	stone4->setPosition(Vec2(0, 1));
	Tile* ore = new Tile("diamond_ore");
	ore->setPosition(Vec2(1, 1));
	Tile* stone5 = new Tile("stone");
	stone5->setPosition(Vec2(2, 1));
	Tile* stone6 = new Tile("stone");
	stone6->setPosition(Vec2(0, 2));
	Tile* stone7 = new Tile("stone");
	stone7->setPosition(Vec2(1, 2));
	Tile* stone8 = new Tile("stone");
	
	stone8->setPosition(Vec2(2, 2));

	std::vector<TextureType*> textures = {};
	textures.push_back(new TextureType());
	for (int i = 0; i < 10; i++) {
		textures.push_back(new TextureType("assets/blocks/block_break/" + std::to_string(i) + ".png"));
	}
	AnimationType* breakAnim = new AnimationType(textures);

	for (Tile* tile : Tiles) {
		Frame* frame = tile->getFrame();
		frame->setVisible(true);
		frame->setEventCallback(EventEnum::MOUSE_DOWN, oreDigCallback);
		frame->setAnimation(breakAnim);
	}
/*
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
		TextureType* labelTexture = new TextureType();
		ColourType* labelColour = new ColourType(128, 255, 128, 128);
		labelTexture->setColour(labelColour);
	label->setTexture(labelTexture);
	label->setVisible(true);
*/


	MCTD3_WrapSimpleUI();

	SimpleUI_Log("UI initialised.");
	return true;
}
