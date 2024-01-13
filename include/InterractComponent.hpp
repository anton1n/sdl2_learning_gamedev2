#pragma once

#include "ECS.hpp"
#include "SpriteComponent.hpp"
#include <string>


class InterractComponent : public Component
{
private:
    bool accessed;
public:
    InterractComponent() : accessed(false){
    }
    void init() override
    {

    }
    void update() override
    {
        if(accessed)
        {
            entity->destroy();
        }
    }
    void access(bool &gotItem)
    {
        std::string id = entity->getComponent<SpriteComponent>().getTex();
        if(id.compare( std::string("door")) != 0)
        {
            accessed = true;
            gotItem = true;
        }
        if(id.compare( std::string("door"))==0 && gotItem)
        {
            accessed = true;
        }
    }
};