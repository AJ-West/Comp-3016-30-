#pragma once
#include "GameObject.h"

class movementComponent : public Component {
public:
	virtual void update(float deltatime) { // update position based off direction of movement
		SDL_FRect dimensions = owner->getDimensions();
		pair<float, float> direction = owner->getDirection();
		float speed = owner->getSpeed() * deltatime;
		owner->setDimensions({ dimensions.x + direction.first * speed, dimensions.y + direction.second * speed, dimensions.w, dimensions.h });
	}

	movementComponent(GameObject* obj) : Component(obj) {}
	virtual ~movementComponent() {}
};