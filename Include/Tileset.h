#pragma once

#include <Object.h>
#include <Data.h>
#include <Collision.h>

class Tileset : public Object, public Data
{
public:
	Tileset(SDL_Renderer* ren, SDL_Event* ev, Vec2 pos);
	~Tileset();

	void Render();
	void Update();
	SDL_Rect* GetDest() {
		return &m_dest;
	}
	SDL_Rect* GetSrc() {
		return &m_src;
	}
	SDL_Rect* GetScreen() {
		return &m_screen;
	}

	void LoadTileset(SDL_Texture* tex);
	bool IsTilesetLoaded() { return m_isTilesetLoaded; }
	void SetParams(int rows, int columns);
	void SelectTile();
	void UnselectTle();

	SDL_Rect GetTileSrc() const {
		return m_srcTile;
	}
	bool IsTileSelected() const {
		return m_isTileSelected;
	}
	int GetTileID() const {
		return m_selectedTileID;
	}

	int GetRows() { return m_rows; }
	int GetColumns() { return m_columns; }

	void Load();
	void Save(std::string texPath);
private:
	void Save();
	int GetFlag(std::string str) {
		if (str == "jpg")
			return IMG_INIT_JPG;
		if (str == "png")
			return IMG_INIT_PNG;
		if (str == "webp")
			return IMG_INIT_WEBP;
		return IMG_INIT_PNG;
	}

	SDL_Event* m_ev;
	Vec2 m_position;

	int m_rows, m_columns;
	float m_destTileSizeW, m_destTileSizeH;
	bool m_isTileSelected;
	bool m_isTilesetLoaded=false;
	int m_selectedTileID;

	SDL_Rect m_selectedTile;
	SDL_Rect m_srcTile;

	std::string m_texPath;
};

