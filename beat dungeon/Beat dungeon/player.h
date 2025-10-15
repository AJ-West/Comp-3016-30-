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
	Player(int start_x, int start_y, SDL_Renderer* SDL_renderer, Dungeon* dungeon);
	~Player();
	//update the player position
	void move();
	void change_direction(SDL_Keycode key);
	 
	//draw player
	void render();

	//checks wall collision
	bool checkCollision();
	void checkEnd();

	void win();

	//getters
	pair<int, int> getPos();
	pair<int, int> getDimensions();
	vector<vector<SDL_Keycode>> getMovementKeys() { return movement_keys; }

private:
	// player position
	float x, y;
	float speed = 0.035;
	int player_width = 20;
	int player_height = 20;
	int direction = NULL;
	// the keys for movement may change as the game goes on so storing them in arrays to check what the current movement keys are
	vector<vector<SDL_Keycode>> movement_keys;
	//game renderer
	SDL_Renderer* renderer;
	Dungeon* dung;

	//undoes latest move for the player
	void undoMove();
};