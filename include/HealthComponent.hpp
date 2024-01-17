#pragma once

#include "ECS.hpp"
#include "Collision.hpp"
#include "Game.hpp"

extern Manager manager;

class HealthComponent: public Component
{
private:
    int healthPoints;
public:
    bool hasDied = false;
    //std::map<const char*, HP> animations;
    HealthComponent(int hp) : healthPoints(hp)
    {}
    ~HealthComponent(){}

    void init() override
    {

    }

    void update() override
    {
        if(healthPoints <= 0)
        {
            if(entity->hasComponent<EnemyComponent>()) {
                Vector2D pos = entity->getComponent<TransformComponent>().position;
                entity->destroy();
                auto& dead(manager.addEntity());
                dead.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 3, 0);
                dead.addComponent<SpriteComponent>("dead",false);
                dead.addGroup(Game::groupOverlays);
            }
            else{
                Game::gameState = START_MENU;
            }
        }
    }

    void hit()
    {
        healthPoints--;
    }

    int getHealth()
    {
        return healthPoints;
    }

    void setHealth(int h)
    {
        healthPoints = h;
    }

};