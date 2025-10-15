#pragma once
#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>

#include <SDL3_image/SDL_image.h>

#include "player.h"

using namespace std;

class KeyTime {
public:
	KeyTime(SDL_Keycode key_char);
	//KeyTime(Player* play);
	~KeyTime();

	bool time_elapsed();

	void render(SDL_Renderer* renderer, SDL_Texture* key_outline, SDL_Texture* key_dot);

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
	int size = 100;

	TTF_Font* font;

	SDL_Keycode key;
};