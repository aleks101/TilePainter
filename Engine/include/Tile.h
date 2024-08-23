#pragma once

#include "Object.h"

class Tile : public Object
{
private:
	static int streampos;
public:
	Tile() {}
	Tile(SDL_Renderer* ren, SDL_Texture* tex, SDL_Rect dest, SDL_Rect* player, bool isSolid);
	Tile(const Tile& obj);
	Tile(Tile&& other) noexcept
		: Object(std::move(other)) {
	}

	// Move Assignment Operator
	Tile& operator=(Tile&& other) noexcept {
		if (this != &other) {
			Object::operator=(std::move(other));  // Move the base class
		}
		return *this;
	}
	~Tile();
	Tile& operator=(const Tile& obj);
	SDL_Rect* GetDest() { return &m_dest; }
	SDL_Rect* GetSrc() { return &m_src; }
	SDL_Rect* GetScreen() { return &m_screen; }

	void Update();
	void Render();
};

