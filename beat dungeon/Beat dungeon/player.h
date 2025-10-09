#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>

using namespace std;

class Player {
public:
	//constructor
	Player(int start_x, int start_y, SDL_Renderer* SDL_renderer);
	~Player();
	//update the player position
	void move(SDL_Keycode key);
	 
	void handle_input(SDL_Event input);
	//draw player
	void render();

	vector<int> getPos();

private:
	// player position
	int x, y;
	// the keys for movement may change as the game goes on so storing them in arrays to check what the current movement keys are
	vector<vector<SDL_Keycode>> movement_keys;
	//game renderer
	SDL_Renderer* renderer;
};