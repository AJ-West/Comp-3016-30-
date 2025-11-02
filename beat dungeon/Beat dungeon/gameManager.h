#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <irrKlang.h>

#include "globals.h"

#include "button.h"
#include "dungeon.h"
#include "KeyTime.h"

using namespace std;
using namespace irrklang;

class GameManager {
public:
	GameManager(SDL_Renderer* SDL_render, SDL_Window* SDL_window);
	~GameManager();

	void render();

	void handleInput(SDL_Event& event);
	void update(float deltaTime);

	void setUpHome();
	void setUpLevelSelect();
	void setUpStory();
	void setUpEnd();

	void levelPaused();
	void nextLevel();
	void tryagain();
	void exitlevel();

	void loadLevel(int level);

	int getUnlockedLevels();
	void unlockLevel();

	string base64_encode(const string& input);
	string base64_decode(const string& input);

private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	vector<SDL_Texture*> story_screens;
	enum screens{home = 1, levels = 2, pauseLevel = 3, story = 4, end = 5, inLevel = 6};
	screens screen = home;

	vector<Button> buttons;

	Dungeon* dung;

	ISoundEngine* music;

	int currentLevel = 0;
	bool paused = false;
};