#pragma once

#include "Object.h"
#include "Text.h"
#include "Collision.h"

class Button : public Text
{
public:
	Button(){}
	Button(SDL_Renderer* ren, SDL_Event* ev, Vec2 pos, TTF_Font* font, std::string text, SDL_Color color1, SDL_Color color2);
	Button(const Button& obj);
	~Button();
	Button& operator=(const Button& obj);
	bool CheckMouseOver();
	bool CheckMouseClick();
	bool CheckMouseClickOutside();
	void Update();
private:
	int x, y;
	bool isMouseOver;
protected:
	SDL_Event* m_ev;
	SDL_Color m_normalColor;
	SDL_Color m_highlightColor;
};

