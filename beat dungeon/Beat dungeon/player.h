#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>

using namespace std;

class Player {
public:
	Player(int start_x, int start_y, SDL_Renderer* SDL_renderer);
	~Player();

	void move(SDL_Keycode key);

	void handle_input(SDL_Event input);

	void render();

private:
	int x;
	int y;
	vector<vector<SDL_Keycode>> movement_keys;
	SDL_Renderer* renderer;
};