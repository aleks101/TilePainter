#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Log.h"

struct Texture {
	std::string imageFile;
	SDL_Texture* texture;
	Texture(std::string _imageFile, SDL_Texture* _texture) : imageFile(_imageFile), texture(_texture) {}
};
struct Font {
	std::string fontFile;
	TTF_Font* font;
	Font(std::string _fontFile, TTF_Font* _font) : fontFile(_fontFile), font(_font) {}
};
struct AudioSound {
	std::string audioFile;
	Mix_Chunk* chunk;
	AudioSound(std::string _audioFile, Mix_Chunk* _chunk) : audioFile(_audioFile), chunk(_chunk) {}
};
struct AudioMusic {
	std::string audioFile;
	Mix_Music* music;
	AudioMusic(std::string _audioFile, Mix_Music* _music) : audioFile(_audioFile), music(_music) {}
};

class Assets
{
private:
	static std::vector<Texture> textures;
	static std::vector<Font> fonts;

	static std::vector<AudioSound> sounds;
	static std::vector<AudioMusic> musics;
public:
	static void AddTexture(SDL_Renderer* ren, const char* getFile, int flag);
	static SDL_Texture* GetTexture(const char* imageFile);
	static void CleanTextures();

	static void AddFont(const char* fontFile, int size);
	static TTF_Font* GetFont(const char* fontFile);
	static void CleanFonts();

	static void AddSound(const char* soundFile);
	static Mix_Chunk* GetSound(const char* soundFile);
	static void CleanSounds();
	static void AddMusic(const char* musicFile);
	static Mix_Music* GetMusic(const char* musicFile);
	static void CleanMusic();
};