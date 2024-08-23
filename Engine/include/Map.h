#pragma once

#include <fstream>

#include "Tile.h"
#include "Collision.h"
#include "Log.h"

struct TexID {
	TexID(SDL_Texture* _tex, int _id) : tex(_tex), id(_id) {}
	TexID(const TexID& texID) : tex(texID.tex), id(texID.id) {}
	SDL_Texture* tex;
	int id;
};
struct TileID {
	TileID(SDL_Rect pos, int _id) : tilePosition(pos), id(_id) {}
	TileID(const TileID& tileID) : tilePosition(tileID.tilePosition), id(tileID.id){}
	SDL_Rect tilePosition;
	int id;
};
class Map
{
private:
	std::vector<Tile> m_tiles;
	Object* m_camObj;
public:
	Map(Object* camObj);
	~Map();
	void Update();
	void Render();
	void AddLayer(SDL_Renderer* ren, std::string filePath, std::vector<TexID> textures, Vec2 startPos, int size, bool isSolid);
	void AddLayer(SDL_Renderer* ren, std::string mapFile, std::string tilesetFile, SDL_Texture* tex, Vec2 startPos, int size, bool isSolid);
	void RemoveTile(int index);
	void MoveTilesFor(Vec2 newPos);
	void MoveTilesTo(Vec2 newPos);
	void MoveDisplayTilesFor(Vec2 newPos);
	void MoveDisplayTilesTo(Vec2 newPos);

	bool CheckCollisionDest(Object* object);
	bool CheckCollisionDest(SDL_Rect* dest);
	bool CheckCollisionDest(Vec2 point);
	bool CheckCollisionScreen(Object* object);
	bool CheckCollision(Object* object, bool destOrScreen, bool XorY);
};

