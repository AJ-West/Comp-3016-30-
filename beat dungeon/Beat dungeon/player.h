#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <utility>

#include "dungeon.h"

using namespace std;

class Dungeon;

class Player {
public:
	//constructor
	Player(){}
	Player(int start_x, int start_y, SDL_Renderer* SDL_renderer, Dungeon* dungeon, vector<vector<int>> walkable_outline);
	~Player();
	//update the player position
	void move();
	void change_direction(SDL_Keycode key, bool down);
	 
	//draw player
	void render();

	bool checkCollision();

	void levelComplete();

	//modifier effects
	void doubleSpeed();
	void newKeys();
	void rotateKeys();

	//getters
	pair<float, float> getPos();
	pair<int, int> getCell();
	pair<int, int> getDimensions();
	vector<vector<SDL_Keycode>> getMovementKeys() { return movement_keys; }
	bool getWin() { return win; }


private:
	// player position
	float x, y;
	float speed = 0.035;
	int player_width = 40;
	int player_height = 40;
	SDL_FRect edge_remove{ 6, 0, 20, 23 };
	int direction = 5;
	// the keys for movement may change as the game goes on so storing them in arrays to check what the current movement keys are
	vector<vector<SDL_Keycode>> movement_keys;
	vector<vector<SDL_Keycode>> potential_keys{ {SDLK_I, SDLK_T, SDLK_UP},{SDLK_J, SDLK_F, SDLK_LEFT},{SDLK_K, SDLK_G, SDLK_DOWN},{SDLK_L, SDLK_H, SDLK_RIGHT} };
	//game renderer
	SDL_Renderer* renderer;
	Dungeon* dung;

	SDL_Texture* sprite;

	bool win = false;

	vector<vector<int>> w_outline;

	//undoes latest move for the player
	void undoMove();
};