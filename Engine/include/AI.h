#pragma once

#include <SDL.h>

#include "Vec2.h"
#include "Physics.h"
#include "Log.h"

class AI
{
protected:
	SDL_Rect* m_enemyDest;
private:
	SDL_Rect* m_entityDest;
public:
	AI(SDL_Rect* playerDest, SDL_Rect* dest);
	virtual Vec2 Follow(float speed, Vec2 destination);
};

