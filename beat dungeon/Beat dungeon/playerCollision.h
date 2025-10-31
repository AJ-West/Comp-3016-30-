#pragma once
#include "GameObject.h"
class playerCollisionComponent : public Component {
public:
	virtual void update(float deltatime) {
		if (checkPlayerCollision(player->getDimensions())) {
			cout << "levelfailed";
		}
	}

	bool checkPlayerCollision(SDL_FRect player_dim) {
		SDL_FRect dim = owner->getDimensions();
		int x_dist = dim.x - player_dim.x;
		int y_dist = dim.y - player_dim.y;
		if (x_dist * x_dist + y_dist * y_dist < range * range) {
			return true;
		}
		return false;
	}

	playerCollisionComponent(GameObject* obj, GameObject* play, int attackRange) : Component(obj), player(play), range(attackRange) {}
	virtual ~playerCollisionComponent() {}
private:
	GameObject* player;
	int range;
};