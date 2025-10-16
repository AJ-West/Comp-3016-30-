#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>

#include "monster.h"

using namespace std;

class Player;
//class Monster;

class Skeleton : public Monster {
public:
	//constructor
	Skeleton() {};
	Skeleton(float start_x, float start_y, SDL_Renderer* SDL_renderer, Player* player_point, float s_speed) {};
	~Skeleton() {};

	//update the monster position
	virtual void move();

	virtual void attack();
	
	virtual void checkAttackCollision();

private:
	int range = 10;
	int attackRange = 20;
};