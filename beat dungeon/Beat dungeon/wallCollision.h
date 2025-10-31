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
			//levelComplete();
			cout << "level complete";
		}
		else if (w_outline[centre.second][centre.first] == 3) {//speed
			//w_outline[centre.second][centre.first] = 0;
			//dung->setDungeonTile(centre.first, centre.second, { 300,300,32,32 });
			//doubleSpeed();
			cout << "speed";
		}
		else if (w_outline[centre.second][centre.first] == 4) {//new keys
			w_outline[centre.second][centre.first] = 0;
			//dung->setDungeonTile(centre.first, centre.second, { 300,300,32,32 });
			//newKeys();
			cout << "new keys";
		}
		else if (w_outline[centre.second][centre.first] == 5) {//rotate keys
			//w_outline[centre.second][centre.first] = 0;
			//dung->setDungeonTile(centre.first, centre.second, { 300,300,32,32 });
			//rotateKeys();
			cout << "rotate keys";

		}
	}

	environmentCollisionComponent(GameObject* obj) : Component(obj) {}
	virtual ~environmentCollisionComponent() {}
};