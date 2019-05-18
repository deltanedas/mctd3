#pragma once

#include <filesystem>
#include "simpleui.h"

extern Vec2 TileSize;
extern std::map<std::string, TextureType*> TileTextures;
extern ColourType blankColour;

void initTextures();
void updateTilePositions();
int cleanUpTiles();

class Tile {
	public:
		Tile(std::string id = "dirt");
		~Tile();

		void setID(std::string id);
		void setPosition(Vec2 position);
		void setBackground(bool background);

		std::string getID();
		Vec2 getPosition();
		Frame* getFrame();
		bool getBackground();
	private:
		std::string ID;
		Frame* frame;
		Vec2 Position;
		bool Background = false;
};

extern std::set<Tile*> Tiles;

class Wave {
public:
	Wave() {}

	std::map<std::string, int> Mobs;
};

class Map {
	public:
		Map() {}

		std::set<Tile> MapTiles;
		std::set<Wave> Waves;
		Vec2 Bounds;
};