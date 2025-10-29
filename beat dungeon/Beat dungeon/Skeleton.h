#pragma once

#include "monster.h"

using namespace std;

class Player;

class Skeleton : public Monster {
public:
	//constructor
	Skeleton(float start_x, float start_y, Player* player_point, float s_speed, vector<vector<int>> walkable_outline, Dungeon* dungeon) : Monster(start_x, start_y, player_point, s_speed, walkable_outline, dungeon) {};
	~Skeleton() {};

	//update the monster position
	virtual void move();

	virtual void attack();
	
	virtual void checkAttackCollision();

private:
	int range = 9;
};