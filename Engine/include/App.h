#pragma once

#include <SDL.h>

class App
{
private:
	static App* instance;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event* ev;
	App();
public:
	App(const App& obj) = delete;

	static App* GetInstance();
	void Init(const char* title, const int width, const int height);
	void Render();
	void Quit();
	
	SDL_Renderer* GetRenderer();
	SDL_Event* GetEvent();
	SDL_Window* GetWindow();
};