#pragma once
#include <SDL2/SDL.h>

#include "ColliderComponent.hpp"


class Collision
{
public:
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);

};