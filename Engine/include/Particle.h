#pragma once

#include "Object.h"
#include "Timer.h"
#include "Vec2.h"
#include "Physics.h"
//#include "Light.h"

class Particle : public Object
{
public:
	Particle(){}
	Particle(SDL_Renderer* ren, SDL_Texture* tex, SDL_Rect dest, SDL_Rect screen, SDL_Rect* player, Vec2 velocity, float lifeTime);
	Particle(const Particle& obj);
	~Particle();
	Particle& operator=(const Particle& obj);
	SDL_Rect* GetDest() { return &m_dest; }
	SDL_Rect* GetScreen() { return &m_screen; }
	SDL_Rect* GetSrc() { return &m_src; }
	void Render();
	void Update();
	void Move();
	bool m_isDead;
private:
	int m_opacity;
	float m_lifetime;
	Timer m_timer;
	//Light m_light;
};

