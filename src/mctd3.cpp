#include "mctd3.h"

Vec2 TileSize(128, 128);
std::map<std::string, TextureType*> TileTextures;
ColourType blankColour(128, 0, 128);

void initTextures() {
	SimpleUI_Log("Loading textures...", Level::DEBUG);
	for (const auto& entry : std::filesystem::directory_iterator("assets/blocks")) {
		if (std::filesystem::is_regular_file(entry)) {
			std::string filePath = entry.path();
			std::string fileName = std::filesystem::path(filePath).stem();
			std::string extension = std::filesystem::path(filePath).extension();
			if (extension.compare(".png") == 0) {
				SimpleUI_Log("Loading texture '" + fileName + ".png'.", Level::DEBUG);
				TileTextures[fileName] = new TextureType("assets/blocks/" + fileName + ".png");
			} else {
				SimpleUI_Log("Texture '" + fileName + "' must be a .png file.", Level::WARN);
			}
		}
	}
	SimpleUI_Log("Textures loaded.");
}

void updateTilePositions() {
	for (Tile* tile : Tiles) {
		tile->setPosition(tile->getPosition());
		SizeType tileSize;
		tileSize.setOffset(Vec2(TileSize.X, TileSize.Y));
		tile->getFrame()->setSize(tileSize);
	}
}

int cleanUpTiles() {
	int ret = 0;
	for (Tile* tile : Tiles) {
		delete tile;
		ret++;
	}
	return ret;
}

Tile::Tile(std::string id) {
	frame = new Frame();
	SizeType tileSize;
	tileSize.setScale(Vec2(0, 0));
	tileSize.setOffset(Vec2(TileSize.X, TileSize.Y));
	frame->setSize(tileSize);
	SizeType tilePos;
	tilePos.setScale(Vec2(0, 0));
	tilePos.setOffset(Vec2(0, 0));
	frame->setPosition(tilePos);
	if (&TileTextures[id]) {
		frame->setTexture(TileTextures[id]);
	} else {
		SimpleUI_Log("Texture '" + id + "' not found.", Level::WARN);
	}
	frame->setParent();
	Tiles.insert(this);
}

void Tile::setID(std::string id) {
	frame->setTexture(TileTextures[id]);
	ID = id;
}

void Tile::setPosition(Vec2 position) {
	SizeType pos;
	pos.setScale(Vec2(0, 0));
	pos.setOffset(Vec2(position.X * TileSize.X, position.Y * TileSize.Y));
	frame->setPosition(pos);
	Position = position;
}

void Tile::setBackground(bool background) {
	Background = background;
}

std::string Tile::getID() {
	return ID;
}

Vec2 Tile::getPosition() {
	return Position;
}

Frame* Tile::getFrame() {
	return frame;
}

bool Tile::getBackground() {
	return Background;
}

Tile Tile::operator=(const Tile& tile) {
	frame = tile.frame;
}

std::set<Tile*> Tiles;
