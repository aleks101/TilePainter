#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Object.h"
#include "Log.h"

class Text : public Object
{

private:
	void CreateText();
public:
	Text(){}
	Text(SDL_Renderer* ren, Vec2 pos, TTF_Font* _font, std::string _text, SDL_Color _color);
	Text(const Text& text);
	virtual ~Text();
	Text& operator=(const Text& obj);

	void Render();
	virtual void Update();

	SDL_Rect* GetDest();
	SDL_Rect* GetSrc();
	SDL_Rect* GetScreen();

	virtual void CleanTexture();

	virtual void SetNoChangeText(int indexTo);
	virtual void SetNoChangeText(std::string newText);
	virtual void ChangeText(std::string _text);
	virtual void ChangeText(int num);
	void ChangeColor(SDL_Color color);
	void ChangeSize(int num);
	void ChangeSizeFor(int num);

	std::string GetText() const { return m_text; }
protected:
	std::string m_text;
	std::string m_permaText;
	SDL_Color m_color;
	TTF_Font* m_font;

	int m_origWidth, m_origHeight;
	int m_sizePercent;
};

