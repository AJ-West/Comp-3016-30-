#pragma once

#include "monster.h"

using namespace std;

class Player;

class Skeleton : public Monster {
public:
	//constructor
	Skeleton(float start_x, float start_y, Player* player_point, float s_speed) : Monster(start_x, start_y, player_point, s_speed) {};
	~Skeleton() {};

	//update the monster position
	virtual void move();

	virtual void attack();
	
	virtual void checkAttackCollision();

private:
	int range = 10;
	int attackRange = 20;
};