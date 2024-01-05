#pragma once

#include "ECS.hpp"
#include "Collision.hpp"
#include "Game.hpp"

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
            if(entity->hasComponent<EnemyComponent>())
                entity->destroy();
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