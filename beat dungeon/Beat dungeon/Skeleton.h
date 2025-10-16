#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>

#include "player.h"

using namespace std;

class Player;

class Monster {
public:
	//constructor
	Monster();
	Monster(float start_x, float start_y, SDL_Renderer* SDL_renderer, Player* player_point, float s_speed);
	~Monster();
	//update the monster position
	void move();

	//draw monster
	void render();

	void checkPlayerCollision(vector<pair<int, int>> player_corners);
	void checkCorner(int corner_x, int corner_y, vector<pair<int, int>> player_corners);

private:
	// monster position
	float x, y;
	float width = 20, height = 20;
	pair<int, int> target_pos{ 0,0 };
	pair<int, int> player_dimen;
	float speed;
	//game renderer
	SDL_Renderer* renderer;
	Player* player;
};