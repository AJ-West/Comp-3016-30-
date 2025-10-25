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
	KeyTime(SDL_Texture* key_text, bool is_good, SDL_Texture* SDL_Texture, pair<int,int> pos);
	//KeyTime(Player* play);
	~KeyTime();

	bool time_elapsed();

	void render(SDL_Texture* key_dot, SDL_Renderer* renderer);

	bool inZone();

	double progress();

	//getters
	//int getCountdown() { return countdown; }
	SDL_Keycode getKey() { return key; }
	bool getUsed() { return used; }
	bool getGood() { return good; }

	//setters
	void setUsed(bool use) { used = use; }

private:
	//Player* player;
	double time_made;
	double time_expired;
	pair<int, int> position;
	int size = 100;

	bool good;
	bool used = false;

	TTF_Font* font;

	SDL_Keycode key;

	SDL_Texture* texture;
	SDL_Texture* text;
};