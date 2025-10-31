#pragma once
#include "GameObject.h"

class PlayerObj : public GameObject {
public:
	PlayerObj(SDL_FRect dim, float sp, vector<vector<int>> walkable_outline, int size) : GameObject(dim, sp, walkable_outline, size) {}

	void wallCollision(float deltatime) {
		SDL_FRect dimensions = getDimensions();
		pair<int, int> direction = getDirection();
		float speed = getSpeed() * deltatime * -1;
		setDimensions({ dimensions.x + direction.first * speed, dimensions.y + direction.second * speed, dimensions.w, dimensions.h });
	}
};