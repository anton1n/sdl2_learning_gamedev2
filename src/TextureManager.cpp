#include "TextureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
	FILE* pf = nullptr;
	pf = fopen(fileName, "r");
	if (!pf)
	{
		std::cout << "Texture not found! " << fileName << std::endl;
	}

	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}
void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);

}
