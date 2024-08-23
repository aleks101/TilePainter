#pragma once

#include <App.h>
#include <Assets.h>
#include <Log.h>
#include <Button.h>
#include <Sound.h>
#include <TextInput.h>
#include <FileManager.h>

#include "Grid.h"
#include "Tileset.h"

class Application
{
public:
	Application();
	~Application();

	void Start();
private:
	void MainLoop();
	void GameLoop();
	void ExportMap();
	void ExportImage();
	void LoadTileset();
	void Save();
	void Load();
	void FetchData();
private:
	App* app;
	SDL_Event* ev;
	SDL_Texture* sceneTexture;
	bool isRunning;

	Button saveB, loadB, gridB, exportMapB, exportImageB;
	Button addLayer;
	Button hideButton;
	Button deleteButton;

	Button upB, downB, rightB, leftB;
	Button clearB, fillB;
	Button showGridB;
	Button selectTilesetB;

	Button eraseB;
	bool erase;
	bool paint;

	//false - hidden
	//true - shown
	std::vector<std::pair<Button, bool>> layerList;
	int selectedList;

	//GRID
	bool gridSettingShown;
	Text tileSize, gridSize, gridPos;
	Text row, column;
	TextInput tileW, tileH;
	TextInput gridW, gridH;
	TextInput gridPosX, gridPosY;
	TextInput rowInput, columnInput;

	Grid* grid;
	Tileset* tileset;
	std::string currTilesetPath = "noPath";
	std::string currSavePath = "noPath";

	Button tiles;

	Sound<Mix_Chunk>* clickSound;
	//SDL_Rect right, left, up, down;
	//OFFSETS
	int offsetX = 10, offsetY = 10;
};

