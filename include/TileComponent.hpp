#pragma once

#include "ECS.hpp"
//#include "TransformComponent.hpp"
#include <SDL2/SDL.h>
//#include "SpriteComponent.hpp"

class TileComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
	{
		texture = Game::assets->GetTexture(id);

		position.x = xpos;
		position.y = ypos;
		
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = tsize * tscale;
	}

	void update() override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	/*TileComponent(int x, int y, int w, int h, int id)
	{
		tileRect.x = x;
		tileRect.y = y;
		tileRect.w = w;
		tileRect.h = h;
		tileID = id;

		const char *dirt = "res/dirt.png";
		const char* grass = "res/grass.png";
		const char* water ="res/water.png";

		switch (tileID)
		{
		case 0:
			path = new char[strlen(dirt)];
			strcpy(path, dirt);
			break;
		case 1:
			path = new char[strlen(grass)];
			strcpy(path, grass);
			break;
		case 2:
			path = new char[strlen(water)];
			strcpy(path, water);
			break;
		default:
			break;
		}
	}*/
/*
	void init() override
	{
		entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();

	}*/
};
