#pragma once

#include "Components.hpp"
#include "Vector2D.hpp"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	int speed = 3;

	int height = 32;
	int width = 32;
	int scale = 1;

	TransformComponent()
	{
		position.Zero();
	}
	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
    TransformComponent(float x, float y, int speed)
    {
        position.x = x;
        position.y = y;
        this->speed = speed;
    }
	TransformComponent(int sc)
	{
		position.x = 400;
		position.y = 600;
		//velocity.x = 1;
		//velocity.y = 1;
		scale = sc;
	}

	TransformComponent(float x, float y, int h, int w, int s)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = s;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update()override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}


};