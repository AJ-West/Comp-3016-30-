#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <string>

using namespace std;

class Button {
public:
	Button(){}
	Button(bool display, SDL_Renderer* render, SDL_Texture* SDL_texture, SDL_FRect SDL_frect);
	~Button();

	bool checkClick();
	void onClick();
	void checkHover(int x, int y);

	bool isWithinRect(int x, int y);

	void render();

	//setters
	void setIsDisplayed(bool display) { isDisplayed = display; }
	void setFunc(function<void()> function) { func = function; }
	void setLevel(function<void(int)> levelLoad, int number) { level = levelLoad; num = number; }

	//getters
	bool getIsDisplayed() { return isDisplayed; }
	bool getIsHover() { return isHover; }

private:
	bool isDisplayed;
	bool isHover = false;

	function<void()> func;
	function<void(int)> level;
	int num;

	SDL_Texture* texture;
	SDL_FRect rect;

	SDL_Renderer* renderer;

	string name;
};