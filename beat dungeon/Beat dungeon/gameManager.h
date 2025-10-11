#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <string>

using namespace std;

class GameManager {
public:
	GameManager(SDL_Renderer* SDL_render, SDL_Window* SDL_window);
	~GameManager();

	void render();


private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Texture* home_screen = nullptr;
	SDL_Texture* level_screen = nullptr;
	enum screens{home = 1, levels = 2};
	screens screen = home;
};