#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "player.h"

using namespace std;

class KeyTime {
public:
	KeyTime(SDL_Keycode key_char);
	//KeyTime(Player* play);
	~KeyTime();

	bool time_elapsed();

	void render(SDL_Renderer* renderer);

	bool inZone();

	double progress();

	//getters
	//int getCountdown() { return countdown; }
	SDL_Keycode getKey() { return key; }

private:
	//Player* player;
	double time_made;
	double time_expired;
	int x;
	int y;

	TTF_Font* font;

	SDL_Keycode key;
};