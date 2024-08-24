#include "Application.h"

#include <windows.h>
#include <shobjidl.h> 
#include <string>
#include <locale>

Application::Application() {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	app = App::GetInstance();
	app->Init("Tile painter", 1280, 720);
	IMG_Init(0);
	Assets::AddFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf", 25);
	Assets::AddSound("Files/Audio/click.mp3");
	Assets::AddTexture(app->GetRenderer(), "Files/Images/tileset.png", IMG_INIT_PNG);

	sceneTexture = SDL_CreateTexture(app->GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		1280, 720);
}
Application::~Application() {
	SDL_DestroyTexture(sceneTexture);
	layerList.clear();

	saveB.CleanTexture();
	loadB.CleanTexture();
	gridB.CleanTexture();
	addLayer.CleanTexture();

	hideButton.CleanTexture();
	deleteButton.CleanTexture();
	clearB.CleanTexture();
	fillB.CleanTexture();
	eraseB.CleanTexture();
	selectTilesetB.CleanTexture();

	downB.CleanTexture();
	rightB.CleanTexture();
	leftB.CleanTexture();
	upB.CleanTexture();

	tileSize.CleanTexture();
	gridSize.CleanTexture();
	gridPos.CleanTexture();
	row.CleanTexture();
	column.CleanTexture();
	
	tileW.CleanTexture();
	tileH.CleanTexture();
	gridW.CleanTexture();
	gridH.CleanTexture();
	gridPosX.CleanTexture();
	gridPosY.CleanTexture();
	rowInput.CleanTexture();
	columnInput.CleanTexture();
	showGridB.CleanTexture();

	tiles.CleanTexture();
	exportMapB.CleanTexture();
	exportImageB.CleanTexture();

	delete grid;
	delete tileset;
	delete clickSound;

	Assets::CleanFonts();
	Assets::CleanTextures();
	Assets::CleanSounds();
	app->Quit();
}
void Application::Start() {
	isRunning = true;
	ev = app->GetEvent();

	saveB = Button(app->GetRenderer(), ev, Vec2(0,0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Save", {255,255,255,0}, {255,0,0,0});
	loadB = Button(app->GetRenderer(), ev, Vec2(saveB.GetDest()->w+offsetX, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Load", { 255,255,255,0 }, { 255,0,0,0 });
	gridB = Button(app->GetRenderer(), ev, Vec2(loadB.GetDest()->w + loadB.GetDestPos().x + offsetX, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Grid", {255,255,255,0}, {255,0,0,0});
	exportMapB = Button(app->GetRenderer(), ev, Vec2(1110, 700), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Export Map", { 255,255,255,0 }, { 255,0,0,0 });;
	exportImageB = Button(app->GetRenderer(), ev, Vec2(1110, 680), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Export Image", { 255,255,255,0 }, { 255,0,0,0 });;

	upB = Button(app->GetRenderer(), ev, Vec2(620, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "UP", { 255,255,255,0 }, { 255,0,0,0 });
	downB = Button(app->GetRenderer(), ev, Vec2(620, 700), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "DOWN", { 255,255,255,0 }, { 255,0,0,0 });
	rightB = Button(app->GetRenderer(), ev, Vec2(1210, 360), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "RIGHT", { 255,255,255,0 }, { 255,0,0,0 });
	leftB = Button(app->GetRenderer(), ev, Vec2(0, 360), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "LEFT", { 255,255,255,0 }, { 255,0,0,0 });

	hideButton = Button(app->GetRenderer(), app->GetEvent(), Vec2(100, 700), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Hide", { 255,255,255,0 }, { 255,0,0,0 });
	deleteButton = Button(app->GetRenderer(), app->GetEvent(), Vec2(hideButton.GetDestPos().x + hideButton.GetDest()->w+offsetX, 700), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Delete", {255,255,255,0}, {255,0,0,0});
	clearB = Button(app->GetRenderer(), app->GetEvent(), Vec2(deleteButton.GetDestPos().x + deleteButton.GetDest()->w + offsetX, 700), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Clear", { 255,255,255,0 }, { 255,0,0,0 });
	fillB = Button(app->GetRenderer(), app->GetEvent(), Vec2(clearB.GetDestPos().x + clearB.GetDest()->w + offsetX, 700), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Fill", { 255,255,255,0 }, { 255,0,0,0 });

	addLayer = Button(app->GetRenderer(), app->GetEvent(), Vec2((int)gridB.GetDestPos().x + (int)gridB.GetDest()->w + offsetX, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Add layer", { 255,255,255,0 }, { 255,0,0,0 });
	selectTilesetB = Button(app->GetRenderer(), app->GetEvent(), Vec2(1090, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Select tileset", { 255,255,255,0 }, { 255,0,0,0 });

	eraseB = Button(app->GetRenderer(), app->GetEvent(), Vec2(1150, 220), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Rubber", { 255,255,255,0 }, { 255,0,0,0 });
	erase = false;
	paint = false;

	showGridB = Button(app->GetRenderer(), app->GetEvent(), Vec2(addLayer.GetDestPos().x + addLayer.GetDest()->w + offsetX, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Show grid", {255,255,255,0}, {255,0,0,0});
	showGridB.SetNoChangeText("Show");

	tileSize = Text(app->GetRenderer(), Vec2(0, saveB.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Tile size: ", {255,255,255,0});
	gridSize = Text(app->GetRenderer(), Vec2(0, saveB.GetDest()->h + tileSize.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Grid size: ", { 255,255,255,0 });
	gridPos = Text(app->GetRenderer(), Vec2(0, gridSize.GetDestPos().y + gridSize.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Position : x : y", { 255,255,255,0 });

	tileW = TextInput(app->GetRenderer(), Vec2(tileSize.GetDest()->w+3, saveB.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", {255,255,255,0},{255,0,0,0}, ev);
	tileH = TextInput(app->GetRenderer(), Vec2(0, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", {255,255,255,0}, {255,0,0,0}, ev);
	
	gridW = TextInput(app->GetRenderer(), Vec2(gridSize.GetDest()->w, saveB.GetDest()->h + tileSize.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", {255,255,255,0}, {255,0,0,0}, ev);
	gridH = TextInput(app->GetRenderer(), Vec2(0, saveB.GetDest()->h + tileSize.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", { 255,255,255,0 }, { 255,0,0,0 }, ev);

	gridPosX = TextInput(app->GetRenderer(), Vec2(gridPos.GetDest()->w + offsetX, gridSize.GetDestPos().y + gridSize.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", { 255,255,255,0 }, { 255,0,0,0 }, ev);
	gridPosY = TextInput(app->GetRenderer(), Vec2(0, gridSize.GetDestPos().y + gridSize.GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", { 255,255,255,0 }, { 255,0,0,0 }, ev);

	row = Text(app->GetRenderer(), Vec2(selectTilesetB.GetDestPos().x - 200, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Rows: ", { 255,255,255,0 });
	column = Text(app->GetRenderer(), Vec2(selectTilesetB.GetDestPos().x - 200, 20), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Columns: ", { 255,255,255,0 });
	rowInput = TextInput(app->GetRenderer(), Vec2(selectTilesetB.GetDestPos().x - 80, 0), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", { 255,255,255,0 }, { 255,0,0,0 }, ev);
	columnInput = TextInput(app->GetRenderer(), Vec2(selectTilesetB.GetDestPos().x - 80, 20), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "1", { 255,255,255,0 }, { 255,0,0,0 }, ev);

	grid = new Grid(app->GetRenderer(), app->GetEvent(), &erase, &paint);
	grid->AddLayer("Layer1", 10, 10, 50, 50);

	layerList.push_back(std::make_pair(Button(app->GetRenderer(), app->GetEvent(), Vec2(0, 700), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), grid->AccessLayer().m_name, {255,255,255,0}, {255,0,0,0}), true));

	tileset = new Tileset(app->GetRenderer(), app->GetEvent(), Vec2(1080, 20));

	clickSound = new Sound<Mix_Chunk>(Assets::GetSound("Files/Audio/click.mp3"), 20);

	gridSettingShown = false;

	tileW.AcceptNumbers(true);
	tileW.AcceptNegativeNumbers(false);
	tileH.AcceptNumbers(true);
	tileH.AcceptNegativeNumbers(false);
	gridW.AcceptNumbers(true);
	gridW.AcceptNegativeNumbers(false);
	gridH.AcceptNumbers(true);
	gridH.AcceptNegativeNumbers(false);
	gridPosX.AcceptNumbers(true);
	gridPosY.AcceptNumbers(true);
	columnInput.AcceptNegativeNumbers(false);
	columnInput.AcceptNumbers(true);
	rowInput.AcceptNegativeNumbers(false);
	rowInput.AcceptNumbers(true);

	tileW.ChangeText(50);
	tileH.ChangeText(50);
	gridW.ChangeText(10);
	gridH.ChangeText(10);
	gridPosX.ChangeText(50);
	gridPosY.ChangeText(50);

	tileH.SetDestPos(Vec2(tileW.GetDestPos().x + tileW.GetDest()->w + offsetX, saveB.GetDest()->h));
	gridH.SetDestPos(Vec2(gridW.GetDestPos().x + gridW.GetDest()->w + offsetX, saveB.GetDest()->h + tileSize.GetDest()->h));
	gridPosY.SetDestPos(Vec2(gridPos.GetDest()->w + gridPosX.GetDest()->w + offsetX, gridSize.GetDestPos().y + gridSize.GetDest()->h));

	selectedList = 0;

	MainLoop();
}
void Application::MainLoop() {
	while (isRunning) {
		SDL_SetRenderTarget(app->GetRenderer(), sceneTexture);
		SDL_SetRenderDrawColor(app->GetRenderer(), 0, 0, 0, 0);
		SDL_RenderClear(app->GetRenderer());

		GameLoop();

		//RENDER
		SDL_SetRenderTarget(app->GetRenderer(), nullptr);
		SDL_RenderClear(app->GetRenderer());
		SDL_RenderCopy(app->GetRenderer(), sceneTexture, nullptr, nullptr);
		app->Render();
	}
}
void Application::GameLoop() {
	//CHECK INPUT
	if (SDL_WaitEvent(ev)) {
		if (ev->type == SDL_QUIT)
			isRunning = false;
		if (ev->type == SDL_MOUSEWHEEL) {
			if (ev->wheel.y > 0) {
				if (!layerList.empty()) {
					grid->AccessLayer().ChangePositionFor(0, 40);
					gridPosY.ChangeText(grid->AccessLayer().GetYPos());
				}
			}
			else if (ev->wheel.y < 0) {
				if (!layerList.empty()) {
					grid->AccessLayer().ChangePositionFor(0, -40);
					gridPosY.ChangeText(grid->AccessLayer().GetYPos());
				}
			}
		}

		grid->Input();

		if (saveB.CheckMouseClick() || loadB.CheckMouseClick() || gridB.CheckMouseClick() || addLayer.CheckMouseClick() || exportMapB.CheckMouseClick()
			|| exportImageB.CheckMouseClick() || deleteButton.CheckMouseClick() || hideButton.CheckMouseClick() || clearB.CheckMouseClick() || fillB.CheckMouseClick()
			|| upB.CheckMouseClick() || downB.CheckMouseClick() || rightB.CheckMouseClick() || leftB.CheckMouseClick()
			|| selectTilesetB.CheckMouseClick())
			clickSound->Play();
		if (saveB.CheckMouseClick()) {
			if (!layerList.empty())
				Save();
		}
		if (loadB.CheckMouseClick()) {
			Load();
		}
		if (exportMapB.CheckMouseClick()) {
			if (!layerList.empty())
				ExportMap();
		}
		if (exportImageB.CheckMouseClick()) {
			if (!layerList.empty())
				exportImage = true;
		}
		if (gridB.CheckMouseClick()) {
			gridSettingShown = !gridSettingShown;
		}

		if (addLayer.CheckMouseClick()) {
			if (grid->AddLayer("Layer" + std::to_string(grid->GetLayers().size() + 1), 10, 10, 50, 50)) {
				layerList.push_back(std::make_pair(Button(app->GetRenderer(), app->GetEvent(), Vec2(0, 700 - 20 * layerList.size()), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), grid->AccessLayer().m_name, { 255,255,255,0 }, { 255,0,0,0 }), true));
				gridW.ChangeText(10);
				gridH.ChangeText(10);
				tileW.ChangeText(50);
				tileH.ChangeText(50);
				gridPosX.ChangeText(50);
				gridPosY.ChangeText(50);
				hideButton.ChangeText("Hide");
				selectedList = layerList.size() - 1;
				FetchData();
			}
		}
		if (selectTilesetB.CheckMouseClick()) {
			LoadTileset();
		}

		rowInput.Input();
		if (rowInput.GetNum() == 0) {
			rowInput.ChangeText(1);
		}
		else if (rowInput.HasInputEnded())
			tileset->SetParams(rowInput.GetNum(), columnInput.GetNum());

		columnInput.Input();
		if (columnInput.GetNum() == 0) {
			columnInput.ChangeText(1);
		}
		else if (columnInput.HasInputEnded())
			tileset->SetParams(rowInput.GetNum(), columnInput.GetNum());

		if (deleteButton.CheckMouseClick()) {
			if (!layerList.empty()) {
				if (selectedList >= 0 && selectedList < layerList.size()) {
					LOGln("DELETING ELEMENT AT INDEX " + std::to_string(selectedList));
					layerList.erase(layerList.begin() + selectedList);
					grid->RemoveLayer(selectedList);
					selectedList = layerList.size() - 1;

					Vec2 pos(0, 700);
					for (int i = 0; i < layerList.size(); i++) {
						grid->GetLayers()[i].m_name = "Layer" + std::to_string(i + 1);
						layerList[i].first.SetDestPos(pos);
						layerList[i].first.ChangeText(grid->GetLayers()[i].m_name);
						pos.y -= 20;
					}
					FetchData();
				}
				else {
					LOGln("Invalid selectedList index");
				}
			}
		}
		if (hideButton.CheckMouseClick()) {
			if (!layerList.empty()) {
				if (layerList[selectedList].second)
					hideButton.ChangeText("Show");
				else
					hideButton.ChangeText("Hide");
				layerList[selectedList].second = !layerList[selectedList].second;
				grid->AccessLayer().m_isHidden = !layerList[selectedList].second;
			}
		}
		if (clearB.CheckMouseClick()) {
			if (!layerList.empty())
				grid->AccessLayer().Clear();
		}
		if (fillB.CheckMouseClick()) {
			if (!layerList.empty() && tileset->IsTileSelected())
				grid->AccessLayer().Fill();
		}

		if (upB.CheckMouseClick()) {
			if (!layerList.empty()) {
				grid->AccessLayer().ChangePositionFor(0, 50);
				gridPosY.ChangeText(grid->AccessLayer().GetYPos());
			}
		}
		if (downB.CheckMouseClick()) {
			if (!layerList.empty()) {
				grid->AccessLayer().ChangePositionFor(0, -50);
				gridPosY.ChangeText(grid->AccessLayer().GetYPos());
			}
		}
		if (rightB.CheckMouseClick()) {
			if (!layerList.empty()) {
				grid->AccessLayer().ChangePositionFor(-50, 0);
				gridPosX.ChangeText(grid->AccessLayer().GetXPos());
			}
		}
		if (leftB.CheckMouseClick()) {
			if (!layerList.empty()) {
				grid->AccessLayer().ChangePositionFor(50, 0);
				gridPosX.ChangeText(grid->AccessLayer().GetXPos());
			}
		}


		if (showGridB.CheckMouseClick()) {
			grid->ShowOutline();
		}

		tileset->SelectTile();
		if (tileset->IsTileSelected()) {
			if (!layerList.empty()) {
				grid->AccessLayer().LoadTexture(tileset->GetTex(), tileset->GetTileSrc(), tileset->GetTileID());
				paint = true;
				erase = false;
			}
		}
		if (eraseB.CheckMouseClick()) {
			erase = !erase;
			if (erase) {
				paint = false;
				tileset->UnselectTle();
			}
		}

		if (!layerList.empty()) {
			for (int i = 0; i < layerList.size(); i++) {
				layerList[i].first.Update();
				if (layerList[i].first.CheckMouseClick()) {
					selectedList = i;
					grid->SetActiveLayer(selectedList);
					FetchData();
					if (layerList[selectedList].second) {
						hideButton.ChangeText("Hide");
					}
					else {
						hideButton.ChangeText("Show");
					}
				}
			}
		}

		if (gridSettingShown && !layerList.empty()) {
			if (tileW.CheckMouseClick() || tileH.CheckMouseClick() || gridW.CheckMouseClick() || gridH.CheckMouseClick() || gridPosX.CheckMouseClick() || gridPosY.CheckMouseClick())
				clickSound->Play();

			tileW.Input();
			if (tileW.HasInputEnded()) {
				int n = tileW.GetNum();
				grid->AccessLayer().SetBlockWidth(n);
			}
			else if (tileW.GetNum() == 0) {
				tileW.ChangeText(1);
			}

			tileH.Input();
			if (tileH.HasInputEnded()) {
				int n = tileH.GetNum();
				grid->AccessLayer().SetBlockHeight(n);
			}
			else if (tileH.GetNum() == 0) {
				tileH.ChangeText(1);
			}

			tileH.SetDestPos(Vec2(tileW.GetDestPos().x + tileW.GetDest()->w + offsetX, tileH.GetDestPos().y));

			gridW.Input();
			if (gridW.HasInputEnded()) {
				int n = gridW.GetNum();
				grid->AccessLayer().SetRows(n);
			}
			else if (gridW.GetNum() == 0) {
				gridW.ChangeText(1);
			}

			gridH.Input();
			if (gridH.HasInputEnded()) {
				int n = gridH.GetNum();
				grid->AccessLayer().SetColumns(n);
			}
			else if (gridH.GetNum() == 0) {
				gridH.ChangeText(1);
			}


			gridH.SetDestPos(Vec2(gridW.GetDestPos().x + gridW.GetDest()->w + offsetX, saveB.GetDest()->h + tileSize.GetDest()->h));

			gridPosX.Input();
			if (gridPosX.HasInputEnded()) {
				int n = gridPosX.GetNum();
				grid->AccessLayer().SetPositionX(n);
				gridPosX.ChangeText(n);

			}
			gridPosY.Input();
			if (gridPosY.HasInputEnded()) {
				int n = gridPosY.GetNum();
				grid->AccessLayer().SetPositionY(n);
				gridPosY.ChangeText(n);
			}

			gridPosY.SetDestPos(Vec2(gridPos.GetDest()->w + gridPosX.GetDest()->w + offsetX * 2, gridSize.GetDestPos().y + gridSize.GetDest()->h));
		}
	}
	//UPDATE
	grid->Update();
	if (exportImage) {
		ExportImage();
		exportImage = false;
	}

	saveB.Update();
	loadB.Update();
	gridB.Update();
	addLayer.Update();
	showGridB.Update();
	exportMapB.Update();
	exportImageB.Update();
	selectTilesetB.Update();

	row.Update();
	column.Update();
	rowInput.Update();
	columnInput.Update();

	deleteButton.Update();
	hideButton.Update();
	clearB.Update();
	fillB.Update();

	upB.Update();
	downB.Update();
	rightB.Update();
	leftB.Update();

	tileset->Update();
	eraseB.Update();
	if (erase) {
		SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 0, 0);
		SDL_RenderDrawRect(app->GetRenderer(), eraseB.GetDest());
		SDL_SetRenderDrawColor(app->GetRenderer(), 0, 0, 0, 0);
	}
	if (gridSettingShown && !layerList.empty()) {
		tileSize.Update();
		gridSize.Update();
		gridPos.Update();

		tileW.Update();
		tileH.Update();
		gridW.Update();
		gridH.Update();
		gridPosX.Update();
		gridPosY.Update();
	}
	if (!layerList.empty()) {
		SDL_SetRenderDrawColor(app->GetRenderer(), 255, 255, 0, 0);
		SDL_RenderDrawRect(app->GetRenderer(), layerList[selectedList].first.GetDest());
		SDL_SetRenderDrawColor(app->GetRenderer(), 0, 0, 0, 0);
	}

}
void Application::FetchData() {
	if (!layerList.empty()) {
		Layer* layer = &grid->AccessLayer();
		tileW.ChangeText(layer->GetBlockWidth());
		tileH.ChangeText(layer->GetBlockHeight());
		gridW.ChangeText(layer->GetColumns());
		gridH.ChangeText(layer->GetRows());
		gridPosX.ChangeText(layer->GetXPos());
		gridPosY.ChangeText(layer->GetYPos());
	}
}
HRESULT SelectFolderDialog(HWND owner, std::wstring& selectedFolder) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
		IFileDialog* pfd = NULL;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

		if (SUCCEEDED(hr)) {
			// Set the options to pick folders instead of files.
			DWORD options;
			pfd->GetOptions(&options);
			pfd->SetOptions(options | FOS_PICKFOLDERS);

			// Show the folder selection dialog.
			hr = pfd->Show(owner);

			// Get the selected folder path.
			if (SUCCEEDED(hr)) {
				IShellItem* psi = NULL;
				hr = pfd->GetResult(&psi);
				if (SUCCEEDED(hr)) {
					PWSTR pszFolderPath = NULL;
					hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);

					// Save the selected folder path in the output parameter.
					if (SUCCEEDED(hr)) {
						selectedFolder = pszFolderPath;
						CoTaskMemFree(pszFolderPath);
					}
					psi->Release();
				}
			}
			pfd->Release();
		}
		CoUninitialize();
	}
	return hr;
}
HRESULT SelectImageFileDialog(HWND owner, std::wstring& selectedFile) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
		IFileDialog* pfd = NULL;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

		if (SUCCEEDED(hr)) {
			// Set the options for the file dialog.
			DWORD options;
			pfd->GetOptions(&options);
			pfd->SetOptions(options | FOS_FORCEFILESYSTEM);  // Ensure only file system items are shown

			// Set file type filters (for image files).
			COMDLG_FILTERSPEC fileTypes[] = {
				{L"Image Files (*.png;*.jpg;*.jpeg;*.bmp;*.gif)", L"*.png;*.jpg;*.jpeg;*.bmp;*.gif"},
				{L"All Files (*.*)", L"*.*"}
			};
			pfd->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
			pfd->SetFileTypeIndex(1);  // Choose the first filter as default
			pfd->SetDefaultExtension(L"png");  // Default extension if user doesn't provide one

			// Show the dialog.
			hr = pfd->Show(owner);

			// Check if the user canceled the dialog.
			if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
				// User canceled the dialog; no need to process further.
				pfd->Release();
				CoUninitialize();
				return hr;
			}

			// Get the selected file path if the dialog wasn't canceled.
			if (SUCCEEDED(hr)) {
				IShellItem* psi = NULL;
				hr = pfd->GetResult(&psi);
				if (SUCCEEDED(hr)) {
					PWSTR pszFilePath = NULL;
					hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Save the selected file path in the output parameter.
					if (SUCCEEDED(hr)) {
						selectedFile = pszFilePath;
						CoTaskMemFree(pszFilePath);
					}
					psi->Release();
				}
			}
			pfd->Release();
		}
		CoUninitialize();
	}
	return hr;
}

int GetFlag(std::string str) {
	if (str == "jpg")
		return IMG_INIT_JPG;
	if (str == "png")
		return IMG_INIT_PNG;
	if (str == "webp")
		return IMG_INIT_WEBP;
	return IMG_INIT_PNG;
}
void Application::ExportMap() {
	if (!grid->m_layers.empty()) {
		using namespace std;

		std::wstring filePath;

		// Call the file selection dialog function
		HRESULT hr = SelectFolderDialog(NULL, filePath);
		if (SUCCEEDED(hr)) {
			if (!filePath.empty()) {
				std::string str(filePath.begin(), filePath.end());

				string name = "";
				for (int k = 0; k < grid->m_layers.size(); k++) {
					name = str + "/Layer" + to_string(k + 1) + ".txt";
					FileManager manager(name);
					std::string data = "";
					grid->m_layers;
					data += to_string(grid->m_layers[k].GetColumns()) + "\n";
					data += to_string(grid->m_layers[k].GetRows()) + "\n";

					for (int i = 0; i < grid->m_layers[k].GetRows(); i++) {
						for (int j = 0; j < grid->m_layers[k].GetColumns(); j++) {
							data += to_string(grid->m_layers[k].GetBlock(i, j).texID) + " ";
						}
						data += "\n";
					}
					manager.Write(data);
				}
			}
			else {
				wprintf(L"No file was selected.\n");
			}
		}
		else {
			wprintf(L"File selection was canceled or failed.\n");
		}
	}
}
void Application::ExportImage() {
	if (!grid->m_layers.empty()) {
		using namespace std;

		std::wstring filePath;

		// Call the file selection dialog function
		HRESULT hr = SelectFolderDialog(NULL, filePath);
		if (SUCCEEDED(hr)) {
			if (!filePath.empty()) {
				std::string str(filePath.begin(), filePath.end());
				string name = str + "/Image.jpg";

				SDL_Surface* image = nullptr;
				image = grid->GetLayerSurface(app->GetWindow());

				if (image != nullptr) {
					IMG_SaveJPG(image, name.c_str(), 50);
					SDL_FreeSurface(image);
					LOGln("IMAGE EXPORTED");
				}
				else {
					LOGln("MAJOR FAIL");
				}
			}
			else {
				wprintf(L"No file was selected.\n");
			}
		}
		else {
			wprintf(L"File selection was canceled or failed.\n");
		}
	}
}
void Application::LoadTileset() {
	std::wstring filePath;

	// Call the file selection dialog function
	HRESULT hr = SelectImageFileDialog(NULL, filePath);
	if (SUCCEEDED(hr)) {
		if (!filePath.empty()) {
			wprintf(L"Selected File: %s\n", filePath.c_str());
			std::string str(filePath.begin(), filePath.end());
			std::string endFix=".";
			for (int i = 0; i < str.length(); i++) {
				if (str[i] == '.') {
					endFix = str.substr(i + 1, str.length() - 1);
					break;
				}
			}
			int flag = 2;
			if (endFix != ".") {
				flag = GetFlag(endFix);
			}
			Assets::AddTexture(app->GetRenderer(), str.c_str(), flag);
			tileset->LoadTileset(Assets::GetTexture(str.c_str()));
			currTilesetPath = str;
		}
		else {
			wprintf(L"No file was selected.\n");
		}
	}
	else {
		wprintf(L"File selection was canceled or failed.\n");
	}
}
void Application::Save() {
	if (tileset->IsTilesetLoaded()) {
		using namespace std;

		std::wstring filePath;

		// Call the file selection dialog function
		HRESULT hr = SelectFolderDialog(NULL, filePath);
		if (SUCCEEDED(hr)) {
			if (!filePath.empty()) {
				
				std::string str(filePath.begin(), filePath.end());
				currSavePath = str;
				grid->Save(str);
				tileset->Save(currTilesetPath);
			}
			else {
				wprintf(L"No file was selected.\n");
			}
		}
		else {
			wprintf(L"File selection was canceled or failed.\n");
		}
	}
}
void Application::Load() {
	using namespace std;

	std::wstring filePath;

	// Call the file selection dialog function
	HRESULT hr = SelectFolderDialog(NULL, filePath);
	if (SUCCEEDED(hr)) {
		if (!filePath.empty()) {
			std::string str(filePath.begin(), filePath.end());
			currSavePath = str;
				
			tileset->Load();

			rowInput.ChangeText(tileset->GetRows());
			columnInput.ChangeText(tileset->GetColumns());

			grid->Load(str);
			for (int i = 0; i < grid->GetLayers().size(); i++) {
				grid->GetLayers()[i].LoadTexture(tileset->GetTex(), tileset->GetTileSrc(), tileset->GetTileID());
			}
			if (!layerList.empty()) {
				for (int i = 0; i < layerList.size(); i++)
					layerList[i].first.CleanTexture();
				layerList.clear();
			}
			for (int i = 0; i < grid->GetLayers().size(); i++) {
				layerList.push_back(std::make_pair(Button(app->GetRenderer(), app->GetEvent(), Vec2(0, 700 - 20 * layerList.size()), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), grid->GetLayers()[i].m_name, { 255,255,255,0 }, { 255,0,0,0 }), true));
			}
			selectedList = layerList.size() - 1;

			FetchData();
		}
		else {
			wprintf(L"No file was selected.\n");
		}
	}
	else {
		wprintf(L"File selection was canceled or failed.\n");
	}
}