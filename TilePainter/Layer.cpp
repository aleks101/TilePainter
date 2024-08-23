#include "Layer.h"

int Layer::id = 0;

Layer::Layer(std::string name, SDL_Renderer* ren, SDL_Event* ev, int x, int y, int rows, int columns, int Bwidth, int Bheight) :
	m_name(name), m_ren(ren), m_ev(ev), m_x(x), m_y(y), m_rows(rows), m_colums(columns), m_blockWidth(Bwidth), m_blockHeight(Bheight), m_2Dblocks(rows, columns) {
	m_textureLoaded = false;
	m_gridColor = { 255, 255,0,0 };
	m_prevRows = rows;
	m_prevColums = columns;
	m_isHidden = false;
	m_tex = nullptr;
	m_surface = nullptr;

	SDL_Rect rect = {x, y, Bwidth, Bheight};

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			rect.x = m_x + j * m_colums;
			rect.y = m_y + i * m_rows;
			*m_2Dblocks.AccessElement(i, j) = Block(m_ren, rect);
			m_2Dblocks.AccessElement(i, j)->tile.SetRenderer(m_ren);
			m_2Dblocks.AccessElement(i, j)->texID = 0;
			*m_2Dblocks.AccessElement(i, j)->tile.GetSrc() = { 0,0,0,0 };
		}
	}
	Reposition();
}
Layer::Layer(const Layer& obj) :
	m_name(obj.m_name), m_ren(obj.m_ren), m_tex(obj.m_tex), m_surface(obj.m_surface), m_ev(obj.m_ev), m_x(obj.m_x), m_y(obj.m_y), m_rows(obj.m_rows), m_colums(obj.m_colums), m_blockWidth(obj.m_blockWidth), m_blockHeight(obj.m_blockHeight), m_2Dblocks(obj.m_2Dblocks), m_isHidden(obj.m_isHidden) {
	m_textureLoaded = false;
	m_gridColor = { 255, 255,0,0 };
	m_prevRows = m_rows;
	m_prevColums = m_colums;

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			m_2Dblocks.AccessElement(i, j)->tile.SetRenderer(m_ren);
			*m_2Dblocks.AccessElement(i, j)->tile.GetSrc() = { 0,0,0,0 };
		}
	}
	Reposition();
}
Layer& Layer::operator=(const Layer& obj) {
	m_name = obj.m_name;
	m_ren = obj.m_ren;
	m_tex = obj.m_tex;
	m_surface = obj.m_surface;
	m_ev = obj.m_ev;
	m_x = obj.m_x;
	m_y = obj.m_y;
	m_rows = obj.m_rows;
	m_colums = obj.m_colums;
	m_blockWidth = obj.m_blockWidth;
	m_blockHeight = obj.m_blockHeight;
	m_2Dblocks = obj.m_2Dblocks;
	m_isHidden = obj.m_isHidden;
	m_textureLoaded = false;
	m_gridColor = { 255, 255,0,0 };
	m_prevRows = m_rows;
	m_prevColums = m_colums;

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			m_2Dblocks.AccessElement(i, j)->tile.SetRenderer(m_ren);
		}
	}
	Reposition();
	return *this;
}
Layer::~Layer() {
	m_ren = nullptr;
	m_ev = nullptr;
	m_tex = nullptr;
	if (m_surface != nullptr)
		SDL_FreeSurface(m_surface);
}
void Layer::Render() {
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			m_2Dblocks.AccessElement(i, j)->tile.Update();
		}
	}
}
void Layer::RenderOutline(int x) {
	if (x == 0) {
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_colums; j++) {
				SDL_SetRenderDrawColor(m_ren, m_gridColor.r, m_gridColor.g, m_gridColor.b, m_gridColor.a);
				SDL_RenderDrawRect(m_ren, m_2Dblocks.AccessElement(i, j)->tile.GetScreen());
				SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
			}
		}
	}
	else if(x == 1){
		SDL_Rect* sc = m_2Dblocks.AccessElement(0, 0)->tile.GetScreen();

		Vec2 upL, upR, downL, downR;
		upL = Vec2(sc->x, sc->y);
		sc = m_2Dblocks.AccessElement(0, m_colums - 1)->tile.GetScreen();
		upR = Vec2(sc->x + sc->w, sc->y);
		sc = m_2Dblocks.AccessElement(m_rows - 1, 0)->tile.GetScreen();
		downL = Vec2(sc->x, sc->y + sc->h);
		sc = m_2Dblocks.AccessElement(m_rows - 1, m_colums - 1)->tile.GetScreen();
		downR = Vec2(sc->x + sc->w, sc->y + sc->h);

		SDL_SetRenderDrawColor(m_ren, m_gridColor.r, m_gridColor.g, m_gridColor.b, m_gridColor.a);
		SDL_RenderDrawLine(m_ren, upL.x, upL.y, upR.x, upR.y);
		SDL_RenderDrawLine(m_ren, upL.x, upL.y, downL.x, downL.y);
		SDL_RenderDrawLine(m_ren, upR.x, upR.y, downR.x, downR.y);
		SDL_RenderDrawLine(m_ren, downL.x, downL.y, downR.x, downR.y);
		SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
	}

}
void Layer::Paint(bool canPaint) {
	if (m_textureLoaded && canPaint) {
		int mX, mY;
		SDL_GetMouseState(&mX, &mY);
		Vec2 rightUp = m_2Dblocks.AccessElement(0, m_colums - 1)->tile.GetScreenPos();
		rightUp.x += m_2Dblocks.AccessElement(0, m_colums - 1)->tile.GetScreen()->w;
		Vec2 leftDown = m_2Dblocks.AccessElement(m_rows - 1, 0)->tile.GetScreenPos();
		leftDown.y += m_2Dblocks.AccessElement(m_rows - 1, 0)->tile.GetScreen()->h;

		if (mX <= rightUp.x && mX >= leftDown.x) {
			if (mY >= rightUp.y && mY <= leftDown.y) {
				int column = -1;
				int row = -1;
				for (int i = 1; i <= m_colums; i++) {
					if (mX <= m_x + i * m_blockWidth) {
						column = i - 1;
						break;
					}
				}
				for (int i = 1; i <= m_rows; i++) {
					if (mY <= m_y + i * m_blockHeight) {
						row = i - 1;
						break;
					}
				}
				if (column == -1 || row == -1)
					return;

				*m_2Dblocks.AccessElement(row, column)->tile.GetSrc() = m_src;
				m_2Dblocks.AccessElement(row, column)->texID = m_tileID;
			}
		}
	}
}
void Layer::Erase(bool canPaint) {
	if (canPaint) {
		int mX, mY;
		SDL_GetMouseState(&mX, &mY);
		Vec2 rightUp = m_2Dblocks.AccessElement(0, m_colums - 1)->tile.GetScreenPos();
		rightUp.x += m_2Dblocks.AccessElement(0, m_colums - 1)->tile.GetScreen()->w;
		Vec2 leftDown = m_2Dblocks.AccessElement(m_rows-1, 0)->tile.GetScreenPos();
		leftDown.y += m_2Dblocks.AccessElement(m_rows - 1, 0)->tile.GetScreen()->h;

		if (mX <= rightUp.x && mX >= leftDown.x) {
			if (mY >= rightUp.y && mY <= leftDown.y) {
				int column = -1;
				int row = -1;
				for (int i = 1; i <= m_colums; i++) {
					if (mX <= m_x + i * m_blockWidth) {
						column = i - 1;
						break;
					}
				}
				for (int i = 1; i <= m_rows; i++) {
					if (mY <= m_y + i * m_blockHeight) {
						row = i - 1;
						break;
					}
				}
				if (column == -1 || row == -1)
					return;

				*m_2Dblocks.AccessElement(row, column)->tile.GetSrc() = { 0,0,0,0 };
				m_2Dblocks.AccessElement(row, column)->texID = 0;
			}
		}
	}
}
void Layer::SetPosition(int x, int y) {
	m_x = x;
	m_y = y;
	Reposition();
}
void Layer::SetPositionX(int x) {
	m_x = x;
	Reposition();
}
void Layer::SetPositionY(int y) {
	m_y = y;
	Reposition();
}
void Layer::ChangePositionFor(int x, int y) {
	m_x += x;
	m_y += y;
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			m_2Dblocks.AccessElement(i, j)->tile.ChangeScreenPosFor(Vec2(x, y));
		}
	}
}
void Layer::SetRows(int rows) {
	if (rows > MAX_WIDTH)
		m_rows = MAX_WIDTH;
	else
		m_rows = rows;

	if (m_rows > m_prevRows) {
		CreateBlocks();
	}
	else if (m_rows < m_prevRows) {
		DeleteBlocks();
	}
}
void Layer::SetColumns(int columns) {
	if (columns > MAX_HEIGHT)
		m_colums = MAX_HEIGHT;
	else
		m_colums = columns;

	if (m_colums > m_prevColums) {
		CreateBlocks();
	}
	else if (m_colums < m_prevColums) {
		DeleteBlocks();
	}
}
void Layer::SetBlockWidth(int width) {
	if (width > MAX_WIDTH)
		m_blockWidth = MAX_WIDTH;
	else
		m_blockWidth = width;
	Reposition();
}
void Layer::SetBlockHeight(int height) {
	if (height > MAX_HEIGHT)
		m_blockHeight = MAX_HEIGHT;
	else
		m_blockHeight = height;
	Reposition();
}
void Layer::LoadTexture(SDL_Texture* tex, SDL_Rect src, int ID) {
	if (tex != nullptr) {
		m_textureLoaded = true;
		m_src = src;
		m_tex = tex;
		m_tileID = ID;
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_colums; j++) {
				m_2Dblocks.AccessElement(i, j)->tile.SetTexture(m_tex);
			}
		}
	}
	else {
		m_tileID = 0;
		m_textureLoaded = false;
	}
}
void Layer::Reposition() {
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			Vec2 pos = Vec2(m_x + j * m_blockWidth, m_y + i * m_blockHeight);
			SDL_Rect* sc = m_2Dblocks.AccessElement(i, j)->tile.GetScreen();
			sc->x = pos.x;
			sc->y = pos.y;
			sc->w = m_blockWidth;
			sc->h = m_blockHeight;
		}
	}
}
void Layer::CreateBlocks() {
	if (m_prevColums < m_colums) {
		m_2Dblocks.ResizeColumns(m_colums);

		for (int i = 0; i < m_rows; i++) {
			for (int j = m_prevColums; j < m_colums; j++) {
				SDL_Rect screen;
				screen.w = m_blockWidth;
				screen.h = m_blockHeight;
				screen.x = m_x + j * m_blockWidth;
				screen.y = m_y + i * m_blockHeight;

				*m_2Dblocks.AccessElement(i, j) = Block(m_ren, screen);
				m_2Dblocks.AccessElement(i, j)->tile.SetScreen(screen);
				m_2Dblocks.AccessElement(i, j)->tile.SetRenderer(m_ren);
				m_2Dblocks.AccessElement(i, j)->tile.SetTexture(m_tex);
			}
		}
		m_prevColums = m_colums;
	}
	if (m_prevRows < m_rows) {
		m_prevColums = m_colums;
		m_2Dblocks.ResizeRows(m_rows);
		for (int i = m_prevRows; i < m_rows; i++) {
			for (int j = 0; j < m_colums; j++) {
				SDL_Rect screen;
				screen.w = m_blockWidth;
				screen.h = m_blockHeight;
				screen.x = m_x + j * m_blockWidth;
				screen.y = m_y + i * m_blockHeight;

				*m_2Dblocks.AccessElement(i, j) = Block(m_ren, screen);
				m_2Dblocks.AccessElement(i, j)->tile.SetScreen(screen);
				m_2Dblocks.AccessElement(i, j)->tile.SetTexture(m_tex);
				m_2Dblocks.AccessElement(i, j)->tile.SetRenderer(m_ren);
			}
		}
		m_prevRows = m_rows;
	}
}
void Layer::DeleteBlocks() {
	if (m_colums < m_prevColums) {
		m_prevColums = m_colums;
		m_2Dblocks.ResizeColumns(m_colums);
	}
	if (m_rows < m_prevRows) {
		m_2Dblocks.ResizeRows(m_rows);
		m_prevRows = m_rows;
	}
}
int Layer::GetXPos() const {
	return m_x;
}
int Layer::GetYPos() const {
	return m_y;
}
Block& Layer::GetBlock(int i, int j) {
	if (i >= 0 && i < m_rows) {
		if (j >= 0 && j < m_colums) {
			return *m_2Dblocks.AccessElement(i, j);
		}
	}
}
SDL_Surface* Layer::GetSurface(SDL_Window* window, SDL_Rect* dest) {
	id++;
	if (window != nullptr && dest!=nullptr) {
		SDL_Surface* winSurf = nullptr;
		winSurf = SDL_GetWindowSurface(window);
		if (winSurf != nullptr) {
			SDL_Rect layer;
			layer.x = m_x;
			layer.y = m_y;
			layer.w = m_colums * m_blockWidth;
			layer.h = m_rows * m_blockHeight;
			LOG("x: "); LOG(layer.x); LOG(" y: "); LOG(layer.y); LOG(" w: "); LOG(layer.w); LOG(" h: "); LOGln(layer.w);

			if (m_surface != nullptr) {
				SDL_FreeSurface(m_surface);
				m_surface = nullptr;
			}
			m_surface = SDL_CreateRGBSurface(0, layer.w, layer.h, winSurf->format->BitsPerPixel,
				winSurf->format->Rmask, winSurf->format->Gmask,
				winSurf->format->Bmask, winSurf->format->Amask);
	
			if (SDL_RenderReadPixels(m_ren, &layer, m_surface->format->format, m_surface->pixels, m_surface->pitch) < 0) {
				SDL_FreeSurface(m_surface);
				m_surface = nullptr;
				LOGln("SURFACE IS NULL!");
				return nullptr;
			}
			std::string str = "Files/Images/surface_" + std::to_string(id) + ".jpg";
			id++;
			IMG_SaveJPG(m_surface, str.c_str(), 50);
			return m_surface;
		}
	}
	return nullptr;
}

void Layer::Clear() {
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			SDL_Rect* src = m_2Dblocks.AccessElement(i, j)->tile.GetSrc();
			src->w = 0;
			src->h = 0;
			src->x = 0;
			src->y = 0;
		}
	}
}
void Layer::Fill() {
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			SDL_Rect* src = m_2Dblocks.AccessElement(i, j)->tile.GetSrc();
			src->w = m_src.w;
			src->h = m_src.h;
			src->x = m_src.x;
			src->y = m_src.y;
		}
	}
}
void Layer::Save(std::string folderPath) {
	m_folderPath = folderPath;
	Save();
}
void Layer::Save() {
	SetFile(m_folderPath + "/layers.bin");
	m_manager.WriteAppendBinary(m_isHidden);
	m_manager.WriteAppendBinary(m_src);
	m_manager.WriteAppendBinary(m_tileID);
	m_manager.WriteAppendBinary(m_x);
	m_manager.WriteAppendBinary(m_y);
	m_manager.WriteAppendBinary(m_rows);
	m_manager.WriteAppendBinary(m_colums);
	m_manager.WriteAppendBinary(m_prevRows);
	m_manager.WriteAppendBinary(m_prevColums);
	m_manager.WriteAppendBinary(m_blockWidth);
	m_manager.WriteAppendBinary(m_blockHeight);
	m_manager.WriteAppendBinary(m_textureLoaded);

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			m_manager.WriteAppendBinary(m_2Dblocks.AccessElement(i, j)->texID);
			m_manager.WriteAppendBinary(*m_2Dblocks.AccessElement(i, j)->tile.GetScreen());
			m_manager.WriteAppendBinary(*m_2Dblocks.AccessElement(i, j)->tile.GetSrc());
		}
	}
}
std::streampos Layer::Load(std::string folderPath, std::streampos pos) {
	m_folderPath = folderPath;
	SetFile(folderPath + "/layers.bin");
	m_manager.BinAutoMoveStreamPos(true);
	m_manager.SetStreamPos(pos);
	Load();
	return m_manager.GetStreamPos();
}
void Layer::Load() {
	m_manager.ReadBinary(&m_isHidden);
	m_manager.ReadBinary(&m_src);
	m_manager.ReadBinary(&m_tileID);
	m_manager.ReadBinary(&m_x);
	m_manager.ReadBinary(&m_y);
	m_manager.ReadBinary(&m_rows);
	m_manager.ReadBinary(&m_colums);
	m_manager.ReadBinary(&m_prevRows);
	m_manager.ReadBinary(&m_prevColums);
	m_manager.ReadBinary(&m_blockWidth);
	m_manager.ReadBinary(&m_blockHeight);
	m_manager.ReadBinary(&m_textureLoaded);

	m_2Dblocks.Clear();
	m_2Dblocks.ResizeColumns(m_colums);
	m_2Dblocks.ResizeRows(m_rows);

	int ID = 0;
	SDL_Rect rect = { 0,0,0,0 };
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_colums; j++) {
			*m_2Dblocks.AccessElement(i, j) = Block(m_ren, { 0,0,0,0 });

			m_manager.ReadBinary(&ID);
			m_2Dblocks.AccessElement(i, j)->texID = ID;

			m_manager.ReadBinary(&rect);
			*m_2Dblocks.AccessElement(i, j)->tile.GetScreen() = rect;

			m_manager.ReadBinary(&rect);
			*m_2Dblocks.AccessElement(i, j)->tile.GetSrc() = rect;
		}
	}
}