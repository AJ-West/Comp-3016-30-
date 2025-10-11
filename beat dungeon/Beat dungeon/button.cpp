#include "button.h"

Button::Button(bool display, SDL_Renderer* render, SDL_Texture* SDL_texture, SDL_FRect SDL_frect): isDisplayed(display), renderer(render), texture(SDL_texture), rect(SDL_frect) {}
Button::~Button(){}

bool Button::checkClick() {
	return false;
}

void Button::onClick() {
	if (func)
		func();
	else 
		level(num);
}

void Button::checkHover(int x, int y) {
	isHover = isWithinRect(x, y);
}

bool Button::isWithinRect(int x, int y) {
	if (x < rect.x) return false;
	if (x > rect.x + rect.w) return false;
	if (y < rect.y) return false;
	if (y > rect.y + rect.h) return false;
	return true;
}

void Button::render() {
	SDL_RenderTexture(renderer, texture, NULL, &rect);
}