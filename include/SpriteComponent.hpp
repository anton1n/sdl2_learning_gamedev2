#pragma once

#include "Components.hpp"
#include <SDL2/SDL.h>
#include "TextureManager.hpp"
#include "Animation.hpp"
#include <map>
#include "AssetManager.hpp"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
    std::string texID;
	bool animated = false;
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE; 

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
        texID = id;
		setTex(id);
	}
	SpriteComponent(std::string id, bool isAnimated)
	{
        texID = id;
		animated = isAnimated;

        Animation idle = Animation(0,2,200);
		Animation walkDown = Animation(0, 4, 100);
        Animation sideways = Animation(1,4,100);
        Animation walkUp = Animation(2,4,100);


        animations.emplace("Idle", idle);
		animations.emplace("WalkUp", walkUp);
        animations.emplace("WalkSideways", sideways);
        animations.emplace("WalkDown", walkDown);

		Play("Idle");
		setTex(id);
	}
	void setTex(std::string id) {
		texture = Game::assets->GetTexture(id); //
	}
    std::string getTex()
    {
        return texID;
    }

	~SpriteComponent()
	{
		//SDL_DestroyTexture(texture);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();


		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		//destRect.w = destRect.h = 64;
	}
	void update() override
	{

		if(animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks()/speed)%frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};