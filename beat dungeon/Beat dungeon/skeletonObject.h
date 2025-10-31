#pragma once
#include "GameObject.h"

class Skeleton : GameObject {
public:
	Skeleton(SDL_FRect dim, float sp, vector<vector<int>> walkable_outline, int size) : GameObject(dim, sp, walkable_outline, size) {}
};
