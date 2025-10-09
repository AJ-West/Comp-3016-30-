#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include "player.h"

using namespace std;

class Monster {
public:
	//constructor
	Monster(float start_x, float start_y, SDL_Renderer* SDL_renderer, Player* player_point, float s_speed);
	~Monster();
	//update the monster position
	void move();

	//draw monster
	void render();

private:
	// monster position
	float x, y;
	vector<int> target_pos{ 0,0 };
	float speed;
	//game renderer
	SDL_Renderer* renderer;
	Player* player;
};