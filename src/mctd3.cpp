#include "mctd3.h"

Vec2 TileSize(128, 128);
std::map<std::string, TextureType*> TileTextures;
ColourType blankColour(128, 0, 128);

void initTextures() {
	loggerf("Loading textures...", Level::DEBUG);
	for (const auto& entry : std::filesystem::directory_iterator("assets/blocks")) {
		if (std::filesystem::is_regular_file(entry)) {
			std::string filePath = entry.path();
			std::string fileName = std::filesystem::path(filePath).stem();
			std::string extension = std::filesystem::path(filePath).extension();
			if (extension.compare(".png") == 0) {
				loggerf("Loading texture '" + fileName + ".png'.", Level::DEBUG);
				TileTextures[fileName] = new TextureType("assets/blocks/" + fileName + ".png");
			} else {
				loggerf("Texture '" + fileName + "' must be a .png file.", Level::WARN);
			}
		}
	}
	loggerf("Textures loaded.");
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
	tileSize.setOffset(Vec2(TileSize.X, TileSize.Y));
	frame->setSize(tileSize);
	if (&TileTextures[id]) {
		frame->setTexture(TileTextures[id]);
	} else {
		loggerf("Texture '" + id + "' not found.", Level::WARN);
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
	pos.setOffset(Vec2(position.X * TileSize.X, position.Y * TileSize.Y));
	frame->setPosition(pos);
	Position = position;
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

std::set<Tile*> Tiles;
