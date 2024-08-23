#pragma once

#include <Box.h>
#include <SDL.h>
#include <Tile.h>
#include <Collision.h>
#include <Data.h>

#define MAX_WIDTH 1000
#define MAX_HEIGHT 1000

#define MAX_BLOCK_WIDTH 500
#define MAX_BLOCK_HEIGHT 500

struct Block {
	Tile tile;
	int texID;
	Block(SDL_Renderer* ren, SDL_Rect destination) : tile(ren, nullptr, destination, nullptr, true), texID(0) {

	}
	Block() : texID(0), tile(nullptr, nullptr, { 0,0,0,0 }, nullptr, false) {

	}
	Block(const Block& obj) : tile(obj.tile){
		tile = obj.tile;
		texID = obj.texID;
	}
	Block& operator=(const Block& obj) {
		tile = obj.tile;
		texID = obj.texID;
		return *this;
	}
	// Move constructor
	Block(Block&& obj) noexcept
		: tile(std::move(obj.tile)), texID(obj.texID) {
		obj.texID = 0;
	}

	// Move assignment operator
	Block& operator=(Block&& obj) noexcept {
		if (this != &obj) {
			tile = std::move(obj.tile);
			texID = obj.texID;
			obj.texID = 0;
		}
		return *this;
	}
	~Block() {

	}
};

class Layer : public Data {
public:
	Layer(std::string name, SDL_Renderer* ren, SDL_Event* ev, int x, int y, int rows, int columns, int Bwidth, int Bheight);
	Layer(const Layer& obj);
	Layer& operator=(const Layer& obj);
	~Layer();

	void Render();
	void RenderOutline(int x);
	void Paint(bool canPaint);
	void Erase(bool canPaint);

	void Reposition();
	void CreateBlocks();
	void DeleteBlocks();

	void SetPosition(int x, int y);
	void SetPositionX(int x);
	void SetPositionY(int y);
	void ChangePositionFor(int x, int y);

	void SetRows(int rows);
	void SetColumns(int columns);
	void SetBlockWidth(int width);
	void SetBlockHeight(int height);

	int GetRows() const {
		return m_rows;
	}
	int GetColumns() const {
		return m_colums;
	}
	int GetBlockWidth() const {
		return m_blockWidth;
	}
	int GetBlockHeight() const {
		return m_blockHeight;
	}

	Block& GetBlock(int i, int j);

	void LoadTexture(SDL_Texture* tex, SDL_Rect src, int ID);
	void Clear();
	void Fill();

	int GetXPos() const;
	int GetYPos() const;

	SDL_Texture* GetTexture() {
		return m_tex;
	}
	SDL_Surface* GetSurface(SDL_Window* window, SDL_Rect* dest);

	std::streampos Load(std::string folderPath, std::streampos pos);
	void Save(std::string folderPath);

	std::string m_name;
	bool m_isHidden;
private:
	void Save();
	void Load();
	std::string m_folderPath;
	SDL_Renderer* m_ren;
	SDL_Event* m_ev;
	SDL_Texture* m_tex;
	SDL_Surface* m_surface;

	Box<Block> m_2Dblocks;
	SDL_Rect m_src;
	int m_tileID;

	SDL_Color m_gridColor;

	int m_x, m_y;
	int m_rows, m_colums;
	int m_prevRows, m_prevColums;
	int m_blockWidth, m_blockHeight;

	bool m_textureLoaded;

	static int id;
};