#include "Tileset.h"
#include <string>
#include <Assets.h>

Tileset::Tileset(SDL_Renderer* ren, SDL_Event* ev, Vec2 pos) :
	Object({ (int)pos.x, (int)pos.y,0,0 }), m_ev(ev), m_position(pos), m_selectedTile({0,0,50,50}), m_destTileSizeW(50), m_destTileSizeH(50) {
	m_ren = ren;
	m_rows = 5;
	m_columns = 5;

	m_dest.x = pos.x;
	m_dest.y = pos.y;
	m_dest.w = 200;
	m_dest.h = 200;

	m_srcTile = { 0,0,0,0 };
	m_isTileSelected = false;

	m_selectedTileID = 0;
	m_tex = nullptr;
}
Tileset::~Tileset() {

}
void Tileset::Render() {
	if (m_tex != nullptr && m_ren != nullptr) {
		SDL_RenderCopy(m_ren, m_tex, nullptr, &m_dest);

		if (m_isTileSelected) {
			SDL_SetRenderDrawColor(m_ren, 255, 255, 0, 0);
			SDL_RenderDrawRect(m_ren, &m_selectedTile);
			SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
		}
	}
}
void Tileset::Update() {
	Render();
}
void Tileset::LoadTileset(SDL_Texture* tex) {
	m_tex = tex;
	m_isTilesetLoaded = true;
	int w, h;
	SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);

	m_srcTile.w = w / m_columns;
	m_srcTile.h = h / m_rows;
	
	m_destTileSizeW = m_dest.w / m_columns;
	m_destTileSizeH = m_dest.h / m_rows;

	m_selectedTile.w = static_cast<int>(m_destTileSizeW);
	m_selectedTile.h = static_cast<int>(m_destTileSizeH);

}
void Tileset::SetParams(int rows, int columns) {
	if (m_tex != nullptr && (m_rows != rows || m_columns != columns)) {
		m_rows = rows;
		m_columns = columns;

		int w, h;
		SDL_QueryTexture(m_tex, nullptr, nullptr, &w, &h);

		m_srcTile.w = w / m_columns;
		m_srcTile.h = h / m_rows;

		m_destTileSizeW = m_dest.w / m_columns;
		m_destTileSizeH = m_dest.h / m_rows;

		m_selectedTile.w = static_cast<int>(m_destTileSizeW);
		m_selectedTile.h = static_cast<int>(m_destTileSizeH);
	}
}
void Tileset::SelectTile() {
	if (m_tex != nullptr) {
		int mX, mY;
		SDL_GetMouseState(&mX, &mY);
		SDL_Rect tile;
		if (m_ev->type == SDL_MOUSEBUTTONDOWN) {
			if (m_ev->button.button == SDL_BUTTON_LEFT) {
				for (int i = 0; i < m_rows; i++) {
					for (int j = 0; j < m_columns; j++) {
						tile = { m_dest.x + j * static_cast<int>(m_destTileSizeW), m_dest.y + i * static_cast<int>(m_destTileSizeH),static_cast<int>(m_destTileSizeW), static_cast<int>(m_destTileSizeH)};
						if (coll::CheckCollisionPoint(tile, Vec2(mX, mY))) {
							m_selectedTile.x = m_dest.x + j * m_destTileSizeW;
							m_selectedTile.y = m_dest.y + i * m_destTileSizeH;

							m_srcTile.x = j * m_srcTile.w;
							m_srcTile.y = i * m_srcTile.h;
							m_isTileSelected = !m_isTileSelected;

							m_selectedTileID = i * m_columns + (j + 1);
							break;
						}
					}
				}
			}
		}
	}
}
void Tileset::UnselectTle() {
	m_isTileSelected = false;
}
void Tileset::Save(std::string texPath) {
	m_texPath = texPath;
	Save();
}
void Tileset::Save() {
	m_manager.ChangeFile("Files/Save/tileset.txt");

	m_manager.Write(m_texPath);
	m_manager.WriteAppend("\n");
	m_manager.WriteAppend(m_rows);
	m_manager.WriteAppend("\n");
	m_manager.WriteAppend(m_columns);
}
void Tileset::Load() {
	m_manager.ChangeFile("Files/Save/tileset.txt");

	m_texPath = m_manager.Read(0);
	m_rows = std::stoi(m_manager.Read(1));
	m_columns = std::stoi(m_manager.Read(2));

	Assets::AddTexture(m_ren, m_texPath.c_str(), GetFlag(m_texPath));
	LoadTileset(Assets::GetTexture(m_texPath.c_str()));
}