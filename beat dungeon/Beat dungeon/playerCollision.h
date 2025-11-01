#pragma once
#include "monsterObject.h"
class playerCollisionComponent : public Component {
public:
	virtual void update(float deltatime) {//if can hit player end level
		if (checkPlayerCollision(player->getDimensions())) {
			owner->setHit(true);
		}
	}

	bool checkPlayerCollision(SDL_FRect player_dim) {// see if player is close enough to hit
		SDL_FRect dim = owner->getDimensions();
		int x_dist = dim.x - player_dim.x;
		int y_dist = dim.y - player_dim.y;
		if (x_dist * x_dist + y_dist * y_dist < range * range) {
			return true;
		}
		return false;
	}

	playerCollisionComponent(MonsterObj* obj, GameObject* play, int attackRange) : Component(obj), owner(obj), player(play), range(attackRange) {}
	virtual ~playerCollisionComponent() {}
private:
	MonsterObj* owner;
	GameObject* player;
	int range;
};