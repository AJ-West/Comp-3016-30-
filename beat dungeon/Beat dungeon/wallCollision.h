#pragma once
#include "GameObject.h"

class environmentCollisionComponent : public Component {
public:
	virtual void update(float deltatime) { // check what the owner is colliding with
		SDL_FRect own_dim = owner->getDimensions();
		pair<int, int> centre = owner->getCell();
		vector<vector<int>> w_outline = owner->getWOutline();
		switch (w_outline[centre.second][centre.first]) {//colliding with
		case 1: // wall
			owner->wallCollision(deltatime);
			break;
		case 2: // trapdoor
			owner->setWin(true);
			break;
		case 3: // 2x modifier
			owner->setWalkableCell(centre.second, centre.first, 0);
			owner->setRemoveCell(centre.first, centre.second);
			owner->setSpeed(owner->getSpeed() * 2);
			break;
		case 4: // new keys modifier
			owner->setWalkableCell(centre.second, centre.first, 0);
			owner->setRemoveCell(centre.first, centre.second);
			owner->newKeys();
			break;
		case 5: // rotate keys modifier
			owner->setWalkableCell(centre.second, centre.first, 0);
			owner->setRemoveCell(centre.first, centre.second);
			owner->rotateKeys();
			break;
		}
	}

	environmentCollisionComponent(GameObject* obj) : Component(obj) {}
	virtual ~environmentCollisionComponent() {}
};