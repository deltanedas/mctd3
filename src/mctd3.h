#pragma once

#include <filesystem>
#include <simpleui.h>

using namespace SimpleUI; //sorry not sorry.

extern Vec2 TileSize;
extern std::map<std::string, TextureType*> TileTextures;
extern ColourType blankColour;

void initTextures();
void updateTilePositions();
int cleanUpTiles();

class Tile {
	public:
		Tile(std::string id = "dirt");

		void setID(std::string id);
		void setPosition(Vec2 position);
		void setBackground(bool background);

		std::string getID();
		Vec2 getPosition();
		Frame* getFrame();
		bool getBackground();

		Tile operator=(const Tile& tile);

	private:
		std::string ID = "";
		Frame* frame = nullptr;
		Vec2 Position = Vec2(0, 0);
		bool Background = false;
};

extern std::set<Tile*> Tiles;

class Wave {
	public:
		Wave() {}

		std::map<std::string, int> Mobs = {};
};

class Map {
	public:
		Map() {}

		std::vector<Tile> MapTiles = {};
		std::vector<Wave> Waves = {};
		Vec2 Bounds = Vec2(0, 0);
};
