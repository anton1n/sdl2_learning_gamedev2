#pragma once

#include "ECS.hpp"
#include "Collision.hpp"

class HealthComponent: public Component
{
private:
    int healthPoints;
public:
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
            entity->destroy();
        }
    }

    void hit()
    {
        healthPoints--;
    }

};