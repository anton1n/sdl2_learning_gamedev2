#pragma once
//#pragma warning (disable: 4996)
#include "Game.hpp"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName);

	static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};