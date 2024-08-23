#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Vec2.h"

class Light
{
private:
	SDL_Renderer* m_ren;
	Vec2 m_pos;
	float m_radius, m_intensity;
	SDL_Color m_color;
	SDL_BlendMode m_blendType;
public:
	Light();
	Light(SDL_Renderer* ren, Vec2 pos, float radius, float intensity, SDL_Color color, SDL_BlendMode mode);
	Light(const Light& light);
	void DrawLight();
	void ChangePos(Vec2 pos);
	void ChangeRadius(float radius);
	void ChangeIntensity(float intensity);
	void ChangeColor(SDL_Color color);
};

