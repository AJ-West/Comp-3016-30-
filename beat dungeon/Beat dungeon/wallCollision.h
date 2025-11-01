#pragma once
#include "GameObject.h"

class environmentCollisionComponent : public Component {
public:
	virtual void update(float deltatime) {
		SDL_FRect own_dim = owner->getDimensions();
		pair<int, int> centre = owner->getCell();
		vector<vector<int>> w_outline = owner->getWOutline();
		if (w_outline[centre.second][centre.first] == 1) {
			owner->wallCollision(deltatime);
		}
		if (w_outline[centre.second][centre.first] == 2) {
			owner->setWin(true);
			//levelComplete();
			cout << "level complete";
		}
		else if (w_outline[centre.second][centre.first] == 3) {//speed
			owner->setWalkableCell(centre.second, centre.first, 0);
			//w_outline[centre.second][centre.first] = 0;
			//dung->setDungeonTile(centre.first, centre.second, { 300,300,32,32 });
			//doubleSpeed();
			owner->setRemoveCell(centre.first,centre.second);
			owner->setSpeed(owner->getSpeed()*2);
		}
		else if (w_outline[centre.second][centre.first] == 4) {//new keys
			owner->setWalkableCell(centre.second, centre.first, 0);
			//dung->setDungeonTile(centre.first, centre.second, { 300,300,32,32 });
			owner->setRemoveCell(centre.first, centre.second);
			owner->newKeys();
		}
		else if (w_outline[centre.second][centre.first] == 5) {//rotate keys
			owner->setWalkableCell(centre.second, centre.first, 0);
			//dung->setDungeonTile(centre.first, centre.second, { 300,300,32,32 });
			owner->setRemoveCell(centre.first, centre.second);
			owner->rotateKeys();

		}
	}

	environmentCollisionComponent(GameObject* obj) : Component(obj) {}
	virtual ~environmentCollisionComponent() {}
};