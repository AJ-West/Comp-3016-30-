#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>

#include "monster.h"
#include "dungeon.h"

using namespace std;

class Dungeon;
class Player;
//class Monster;

class Minotaur : public Monster {
public:
	//constructor
	Minotaur() {};
	Minotaur(float start_x, float start_y, SDL_Renderer* SDL_renderer, Player* player_point, float s_speed, Dungeon* dungeon) {};
	~Minotaur() {};

	//update the monster position
	virtual void move();

	virtual void attack();

	virtual void checkAttackCollision();

	void charge();
	void checkChargeCollision();
	void checkWallCollision();
	void crash();

private:
	int charge_range = 500;
	bool charging = false;
	pair<float, float> charge_target_dir{ 0,0 };
	Dungeon* dung;
};