#pragma once

#include "monster.h"

using namespace std;

class Dungeon;
class Player;

class Minotaur : public Monster {
public:
	//constructor
	Minotaur(float start_x, float start_y, Player* player_point, float s_speed, Dungeon* dungeon): Monster(start_x, start_y, player_point, s_speed) {};
	~Minotaur() {};

	//update the monster position
	virtual void move();

	virtual void attack();

	virtual void checkAttackCollision();

	virtual void render(SDL_Renderer* renderer);

	void charge();
	void checkChargeCollision();
	void checkWallCollision();
	void crash();

private:
	int charge_range = 500;
	bool charging = false;
	pair<float, float> charge_target_dir{ 0,0 };
	Dungeon* dung;
	bool stunned = true;
};