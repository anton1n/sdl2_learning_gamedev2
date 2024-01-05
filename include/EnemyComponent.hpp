#pragma once

#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Game.hpp"
//#include <cmath>


class EnemyComponent: public Component
{
private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    //Vector2D* playerPos;
    int detectionRadius;
    Vector2D velocity;
public:
    EnemyComponent( int dr) : detectionRadius(dr)
    {
        velocity = Vector2D();
        //playerPos = nullptr;
    }
    ~EnemyComponent()
    {}

    void init() override
    {
        sprite = &entity->getComponent<SpriteComponent>();
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
    }

    void update() override
    {
        transform->velocity = velocity;

    }
    void rangeCheck(Vector2D &playerPos){

        if(!entity->isActive())
            return;
        int distance = static_cast<int>(
                sqrt( (transform->position.x - playerPos.x)*(transform->position.x - playerPos.x) +
                      (transform->position.y - playerPos.y)*(transform->position.y - playerPos.y))
        );
        //std::cout<<distance<<std::endl;
        if (distance <= detectionRadius) {
            double angle = atan2(playerPos.y - transform->position.y, playerPos.x - transform->position.x);

            velocity = Vector2D(cos(angle), sin(angle));
            if( velocity.x < 0)
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
            else
                sprite->spriteFlip = SDL_FLIP_NONE;
            sprite->Play("Walk");
        }
        else {
            velocity = Vector2D(0, 0);
            sprite->Play("Idle");
            if(sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
                sprite->spriteFlip = SDL_FLIP_NONE;
        }
    }
};