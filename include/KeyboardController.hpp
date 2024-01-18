#pragma once

#include "Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->Play("WalkUp");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->Play("WalkSideways");

				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->Play("WalkSideways");
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->Play("WalkDown");
                break;
            case SDLK_RCTRL: {
                int a = transform->velocity.x;
                int b = transform->velocity.y;
                if (!a && !b) {
                    b = -1;
                }
                int c = entity->getComponent<TransformComponent>().position.x;
                int d = entity->getComponent<TransformComponent>().position.y;
//                Game::assets->CreateProjectile(
//                        Vector2D(entity->getComponent<TransformComponent>().position.x + 45,
//                                 transform->velocity.y == 1 ? entity->getComponent<TransformComponent>().position.y +
//                                                              150 :
//                                 entity->getComponent<TransformComponent>().position.y - 50),
//                        Vector2D(a, b),
//                        200, 1, "projectile");
                //transform->velocity.y == 1 ? d+=150 : d-=50;
                if(transform->velocity.y == 1 && transform->velocity.x !=transform->velocity.y)
                {
                    d+=100;
                    c+=20;
                }
                else if(transform->velocity.y == -1 && transform->velocity.x !=transform->velocity.y)
                {
                    d-=75;
                    c+=20;
                }
                else{
                    if(transform->velocity.x == 1)
                    {
                        c+=100;
                    }
                    else if(transform->velocity.x == -1)
                    {
                        c-=75;
                    }
                    else{
                        d-=50;
                        c+=20 ;
                    }
                }
                Game::assets->CreateProjectile(
                        Vector2D(c,d),Vector2D(a, b),200, 1, "projectile");
                //Sound::playSound("../res/sample-3s.wav");
                break;
            }
			default:
				break;
			}
		}
		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->Play("Idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->Play("Idle");

				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->Play("Idle");
                sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->Play("Idle");
				break;
			case SDLK_ESCAPE:
				if(Game::gameState == START_MENU)
					Game::isRunning = false;
				else
					Game::gameState = START_MENU;
				
				break;
			default:
				break;
			}
		}
	}
};