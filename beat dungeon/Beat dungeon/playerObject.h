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

	void newKeys() {
		int pos = rand() % potential_keys[0].size();
		movement_keys[0].push_back(potential_keys[0][pos]);
		movement_keys[1].push_back(potential_keys[1][pos]);
		movement_keys[2].push_back(potential_keys[2][pos]);
		movement_keys[3].push_back(potential_keys[3][pos]);
		potential_keys[0].erase(potential_keys[0].begin() + pos - 1);
		potential_keys[1].erase(potential_keys[1].begin() + pos - 1);
		potential_keys[2].erase(potential_keys[2].begin() + pos - 1);
		potential_keys[3].erase(potential_keys[3].begin() + pos - 1);
	}

	void rotateKeys() {
		int dir = rand() % 2;
		if (dir == 0) { // anti-clockwise
			vector<SDL_Keycode> temp = movement_keys[0];
			movement_keys[0] = movement_keys[1];
			movement_keys[1] = movement_keys[2];
			movement_keys[2] = movement_keys[3];
			movement_keys[3] = temp;
		}
		else if (dir == 1) { // clockwise
			vector<SDL_Keycode> temp = movement_keys[3];
			movement_keys[3] = movement_keys[2];
			movement_keys[2] = movement_keys[1];
			movement_keys[1] = movement_keys[0];
			movement_keys[0] = temp;
		}
	}

	//getters
	bool getWin() { return won; }
	vector<vector<SDL_Keycode>> getMovementKeys() { return movement_keys; }
	

	//setters
	void setWin(bool win) { won = win; }
	
private:
	vector<vector<SDL_Keycode>> movement_keys;

	pair<int, int> removeCell{ NULL,NULL };

	bool won = false;

	vector<vector<SDL_Keycode>> potential_keys{ {SDLK_I, SDLK_T, SDLK_UP},{SDLK_J, SDLK_F, SDLK_LEFT},{SDLK_K, SDLK_G, SDLK_DOWN},{SDLK_L, SDLK_H, SDLK_RIGHT} };

};