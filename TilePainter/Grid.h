#pragma once

#include <Data.h>
#include "Layer.h"

#define MAX_LAYERS 15

class Grid : public Data
{
public:
	Grid(SDL_Renderer* ren, SDL_Event* ev, bool* erase, bool* paint);
	~Grid();
	void Update();
	void Input();

	bool AddLayer(std::string layerName, int rows, int columns, int Bwidth, int Bheight);
	void RemoveLayer(int i);
	Layer& AccessLayer();
	std::vector<Layer>& GetLayers() {
		return m_layers;
	}
	void SetActiveLayer(int i);
	void ShowOutline();

	void Save(std::string path);
	void Load(std::string path);
	SDL_Surface* GetLayerSurface(SDL_Window* window);
 
	int m_activeLayer;
	std::vector<Layer> m_layers;
private:
	void Save();
	void Load();
	std::string m_folderPath;
	SDL_Renderer* m_ren;
	SDL_Event* m_ev;

	int m_transparency = 100;
	bool m_mouseClick;
	bool* m_paint;
	bool* m_erase;
	int m_showOutline = 0;
};