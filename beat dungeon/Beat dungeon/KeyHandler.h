#pragma once
#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <irrKlang.h>
#include <SDL3_image/SDL_image.h>

#include "player.h"
#include "KeyTime.h"
#include "particles.h"

using namespace std;
using namespace irrklang;

class KeyTime;

class KeyHandler {
public:
	KeyHandler(SDL_Renderer* SDL_renderer, Player* play);
	~KeyHandler();

	void spawnKey();
	void renderKeys();
	void checkTimes();
	void updateParticles();

	void keyDown(SDL_Keycode key);
	void keyUp(SDL_Keycode key);

private:
	Player* player;

	SDL_Renderer* renderer;

	vector<pair<int, int>> locations;
	vector<KeyTime*> all_keys;

	vector<const char*> key_sounds{ "sound effects/keys/1.wav" ,"sound effects/keys/2.wav","sound effects/keys/3.wav","sound effects/keys/4.wav" ,"sound effects/keys/5.wav" };

	vector<SDL_Keycode> keyboard{
		//numbers
		SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
		//letters
		SDLK_A, SDLK_B, SDLK_C, SDLK_D, SDLK_E, SDLK_F, SDLK_G, SDLK_H, SDLK_I, SDLK_J, SDLK_K, SDLK_L, SDLK_M, SDLK_N, SDLK_O, SDLK_P, SDLK_Q, SDLK_R, SDLK_S, SDLK_T, SDLK_U, SDLK_V, SDLK_W, SDLK_X, SDLK_Y, SDLK_Z,
		//misc
		SDLK_UP, SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN
	};

	TTF_Font* font;
	SDL_Texture* text;

	SDL_Texture* key_outline;
	SDL_Texture* key_bad_outline;
	SDL_Texture* key_dot;

	vector<Particle> particles;

	ISoundEngine* sound;
};