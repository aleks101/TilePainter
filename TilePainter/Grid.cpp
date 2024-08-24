#include "Grid.h"

Grid::Grid(SDL_Renderer* ren, SDL_Event* ev, bool* erase, bool* paint) : m_ren(ren), m_ev(ev), m_erase(erase), m_paint(paint) {
	m_mouseClick = false;
	m_activeLayer = -1;
	m_layers.reserve(MAX_LAYERS);
}
Grid::~Grid() {
	m_layers.clear();
}
void Grid::Update() {
	if (!m_layers.empty()) {
		for (int i = 0; i < m_activeLayer; i++) {
			SDL_SetTextureAlphaMod(m_layers[i].GetTexture(), m_transparency);
			m_layers[i].Render();
			SDL_SetTextureAlphaMod(m_layers[i].GetTexture(), 255);

		}
		if (m_activeLayer >= 0 && m_activeLayer < m_layers.size()) {
			if (!m_layers[m_activeLayer].m_isHidden) {
				if (m_mouseClick) {
					m_layers[m_activeLayer].Paint(*m_paint);
					m_layers[m_activeLayer].Erase(*m_erase);
				}
				m_layers[m_activeLayer].Render();
				m_layers[m_activeLayer].RenderOutline(m_showOutline);
			}
		}
	}
}
void Grid::Input() {
	if (m_ev->type == SDL_MOUSEBUTTONDOWN)
		if (m_ev->button.button == SDL_BUTTON_LEFT)
			m_mouseClick = true;
	if (m_ev->type == SDL_MOUSEBUTTONUP)
		if (m_ev->button.button == SDL_BUTTON_LEFT)
			m_mouseClick = false;
}
bool Grid::AddLayer(std::string layerName, int rows, int columns, int Bwidth, int Bheight) {
	if (m_layers.size() < MAX_LAYERS) {
		m_layers.push_back(Layer(layerName, m_ren, m_ev, 50, 50, rows, columns, Bwidth, Bheight));
		m_activeLayer++;
		return true;
	}
	return false;
}
void Grid::RemoveLayer(int i) {
	if (m_layers.empty())
		return;
	if (i < 0 || i >= m_layers.size())
		return;
	LOGln(sizeof(m_layers[i + 1]));
	m_layers.erase(m_layers.begin() + i);
	if (m_activeLayer >= m_layers.size()) {
		m_activeLayer = m_layers.size() - 1;
	}
}
Layer& Grid::AccessLayer() {
	return m_layers[m_activeLayer];
}
void Grid::SetActiveLayer(int i) {
	if (i < 0 || i >= m_layers.size())
		return;
	m_activeLayer = i;
}
void Grid::ShowOutline() {
	m_showOutline++;
	if (m_showOutline >= 3)
		m_showOutline = 0;
}
void Grid::Save(std::string path) {
	m_folderPath = path;
	Save();
}
void Grid::Load(std::string path) {
	m_folderPath = path;
	Load();
}
void Grid::Save() {
	SetFile(m_folderPath + "/layers.bin");
	m_manager.Clear(true);
	SetFile(m_folderPath + "/grid.bin");
	if (!m_layers.empty()) {
		m_manager.WriteBinary(m_activeLayer);
		m_manager.WriteAppendBinary(m_showOutline);
		int s = m_layers.size();
		m_manager.WriteAppendBinary(s);
		for (int i = 0; i < m_layers.size(); i++) {
			m_layers[i].Save(m_folderPath);
		}
	}
}
void Grid::Load() {
	SetFile(m_folderPath + "/grid.bin");
	m_manager.SetStreamPos(0);
	m_manager.BinAutoMoveStreamPos(true);
	int s = 0;
	std::streampos pos = 0;

	m_manager.ReadBinary(&m_activeLayer);
	m_manager.ReadBinary(&m_showOutline);
	m_manager.ReadBinary(&s);

	m_layers.clear();
	for (int i = 0; i < s; i++) {
		m_layers.push_back(Layer("Layer" + std::to_string(m_layers.size()+1), m_ren, m_ev, 50, 50, 10, 10, 50, 50));
		pos = m_layers[i].Load(m_folderPath, pos);
	}
	m_activeLayer = m_layers.size() - 1;
}
SDL_Surface* Grid::GetLayerSurface(SDL_Window* window) {
	if (!m_layers.empty()) {
		SDL_Surface* surface = m_layers[0].GetSurface(window);

		SDL_Rect dest = { 0, 0, surface->w, surface->h };
		SDL_Surface* final = nullptr; 
		final = SDL_CreateRGBSurfaceWithFormat(0, dest.w, dest.h, 32, SDL_PIXELFORMAT_ARGB8888);

		if (final == nullptr) {
			LOGln("SURFACE CREATION FAILED");
			return nullptr;
		}
		for (int i = 0; i < m_layers.size(); i++) {
			SDL_Surface* surface = m_layers[i].GetSurface(window);
			if (surface != nullptr) {
				if (SDL_BlitSurface(surface, nullptr, final, &dest) < 0) {
					LOGln("BLITING FAILED");
					SDL_FreeSurface(surface);
					return nullptr;
				}
			}
			else {
				return nullptr;
			}
		}		
		LOGln("BLITING ALL LAYERS SUCCESSFUL");
		return final;
	}
	return nullptr;
}