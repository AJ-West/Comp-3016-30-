#pragma once
#include "GameObject.h"

class PlayerObj : public GameObject {
public:
	PlayerObj(SDL_FRect dim, float sp, vector<vector<int>> walkable_outline, int size) : GameObject(dim, sp, walkable_outline, size) {
		// Sets up the default movement keys for the player
		movement_keys.resize(4, vector<SDL_Keycode>(1, 0));
		movement_keys[0][0] = SDLK_W;
		movement_keys[1][0] = SDLK_A;
		movement_keys[2][0] = SDLK_S;
		movement_keys[3][0] = SDLK_D;
	}

	void wallCollision(float deltatime) {
		SDL_FRect dimensions = getDimensions();
		pair<int, int> direction = getDirection();
		float speed = getSpeed() * deltatime * -1;
		setDimensions({ dimensions.x + direction.first * speed, dimensions.y + direction.second * speed, dimensions.w, dimensions.h });
	}

	void change_direction(SDL_Keycode key, bool down) {
		if (down) {
			int index = 0;
			// checks which row the key is in
			// row 0 is up, row 1 is left, row 2 is down, row 3 is right
			for (const auto& row : movement_keys) {
				auto it = find(row.begin(), row.end(), key);
				if (it != row.end()) {
					break;
				}
				index++;
			}
			switch (index) {
			case 0:
				setDirection({ 0,-1 });
				break;
			case 1:
				setDirection({ -1,0 });
				break;
			case 2:
				setDirection({ 0,1 });
				break;
			case 3:
				setDirection({ 1,0 });
				break;
			}
		}
		else {
			setDirection({ 0,0 });
		}
	}

	//getters
	bool getWin() { return won; }
	vector<vector<SDL_Keycode>> getMovementKeys() { return movement_keys; }

	//setters
	void setWin(bool win) { won = win; }
private:
	vector<vector<SDL_Keycode>> movement_keys;

	bool won = false;
};