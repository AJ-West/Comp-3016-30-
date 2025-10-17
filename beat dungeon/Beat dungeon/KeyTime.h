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
	KeyTime(SDL_Keycode key_char, bool is_good, SDL_Texture* SDL_Texture, SDL_Renderer* SDL_renderer);
	//KeyTime(Player* play);
	~KeyTime();

	bool time_elapsed();

	void render(SDL_Texture* key_dot);

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
	int x;
	int y;
	int size = 100;

	bool good;
	bool used = false;

	TTF_Font* font;

	SDL_Keycode key;

	SDL_Color text_col{ 255,255,255,0 };

	SDL_Texture* texture;
	SDL_Texture* text;

	SDL_Renderer* renderer;
};