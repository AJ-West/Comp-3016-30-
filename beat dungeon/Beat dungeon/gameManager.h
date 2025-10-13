#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <cstdlib>

#include "button.h"
#include "dungeon.h"
#include "KeyTime.h"

using namespace std;

class GameManager {
public:
	GameManager(SDL_Renderer* SDL_render, SDL_Window* SDL_window);
	~GameManager();

	void render();

	void handleInput(SDL_Event& event);
	void update();

	void setUpHome();
	void setUpLevelSelect();

	void loadLevel(int level);

private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Texture* home_screen = nullptr;
	SDL_Texture* level_screen = nullptr;
	enum screens{home = 1, levels = 2, inLevel = 3};
	screens screen = home;

	vector<Button> buttons;

	Dungeon* dung;
};