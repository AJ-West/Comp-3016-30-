#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "player.h"

using namespace std;

class KeyTime {
public:
	KeyTime();
	//KeyTime(Player* play);
	~KeyTime();

	void decrease();

	void render(SDL_Renderer* renderer);

	//getters
	int getCountdown() { return countdown; }

private:
	//Player* player;
	int countdown;
	int max;
};