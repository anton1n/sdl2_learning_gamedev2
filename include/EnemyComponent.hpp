#pragma once

#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Game.hpp"
#include <stdlib.h>
#include <time.h>


class EnemyComponent: public Component
{
protected:
    TransformComponent* transform;
    SpriteComponent* sprite;
    //Vector2D* playerPos;
    int detectionRadius;
    Vector2D velocity;

public:
    //Vector2D previousPosition;
    EnemyComponent( int dr) : detectionRadius(dr)
    {
        velocity = Vector2D();
        //playerPos = nullptr;
    }
    ~EnemyComponent()
    {
        std::cout<<"Enemy killed!"<<std::endl;
    }

    void init() override
    {
        sprite = &entity->getComponent<SpriteComponent>();
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
        //previousPosition = transform->position;
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
            if(velocity.y>0 && velocity.x< velocity.y)
            {
                sprite->Play("WalkDown");
            }
            else if(velocity.y<0 && velocity.x < velocity.y)
            {
                sprite->Play("WalkUp");
            }
            else{
                if( velocity.x > 0)
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                else
                    sprite->spriteFlip = SDL_FLIP_NONE;
                sprite->Play("WalkSideways");
            }

        }
        else {
            velocity = Vector2D(0, 0);
            sprite->Play("Idle");
            if(sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
                sprite->spriteFlip = SDL_FLIP_NONE;
        }
    }

    void setRadius(int r)
    {
        detectionRadius = r;
    }
};

class SuperEnemy: public EnemyComponent
{
public:
    void shoot()
    {
        //srand(time(NULL));
        ////if(rand() % 5 == 1)
        Game::assets->CreateProjectile(
                        Vector2D(transform->position.x, velocity.y == 1 ? transform->position.y + 75 : transform->position.y -50),
                        Vector2D(velocity.x, velocity.y),200, 1, "projectile");
    }
};