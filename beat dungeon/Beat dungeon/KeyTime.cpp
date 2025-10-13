#include "KeyTime.h"

KeyTime::KeyTime(SDL_Keycode key_char): key(key_char) {
	//time_made = time(0);
	// this is in miniseconds since epoch to allow for smooth increase on progress bar
	time_made = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();;
	time_expired = 1000 + (rand() % 5000);
	x = rand() % 800;
	y = rand() % 800;
	font = TTF_OpenFont("PixelEmulator-xq08.ttf", 100);
	if (font == nullptr) {
		cerr << "Font cannot be loaded: TTF_ERROR" << SDL_GetError() << endl;
		return;
	}
}

/*KeyTime::KeyTime(Player* play) : player(play) {
	max = 1 + rand() % 100;
	countdown = max;
}*/
KeyTime::~KeyTime() {}

bool KeyTime::time_elapsed() {
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - time_made;
	if (milliseconds >= time_expired) {
		return true;
	}
	return false;
}

void KeyTime::render(SDL_Renderer* renderer) {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_FRect background{ x, y, 20, 100 };
	SDL_RenderFillRect(renderer, &background);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_FRect target{ x, y+80, 20, 20 };
	SDL_RenderFillRect(renderer, &target);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_FRect progress_bar{ x, y+progress(), 20, 2};
	SDL_RenderFillRect(renderer, &progress_bar);

	char text = static_cast<char>(key);
	SDL_Surface* surface = TTF_RenderText_Solid(font, &text, 1, { 255,0,0,0 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FRect textRect{ x, y + 80, 20, 20 };
	SDL_RenderTexture(renderer, texture, NULL, &textRect);
	SDL_DestroySurface(surface);
	SDL_DestroyTexture(texture);
}

bool KeyTime::inZone() {
	return progress() >= 80;
}

double KeyTime::progress() {
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - time_made;
	return milliseconds / time_expired * 100;
}