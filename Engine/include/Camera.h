#pragma once

#include <SDL.h>

#include "Vec2.h"
#include "Log.h"

class Camera {
private:
	//player dest
	const SDL_Rect* cameraPos;
	SDL_Rect prevCamera;
	int prevXoffset, prevYoffset;
	Vec2 change;
public:
	Camera();
	bool CheckForChange();
	Vec2 CalculateChange();
	void SetCamera();
	void Init(const SDL_Rect* _cameraPos);
};