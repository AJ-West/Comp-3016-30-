#include "KeyTime.h"

KeyTime::KeyTime(SDL_Keycode key_char, bool is_good, SDL_Texture* SDL_Texture, SDL_Renderer* SDL_renderer): key(key_char), good(is_good), texture(SDL_Texture), renderer(SDL_renderer) {
	//time_made = time(0);
	// this is in miniseconds since epoch to allow for smooth increase on progress bar
	time_made = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();;
	time_expired = 750 + (rand() % 2000);
	if (!good) {
		time_expired += 1000;
		text_col = { 255,0,0,0 };
	}
	x = 0 + rand() % 700;
	if (x < 100) {
		x = 0;
		y = rand() % 400;
	}
	else if (x>700) {
		x = 700;
		y = rand() % 400;
	}
	else {
		y = 0;
	}
	
	font = TTF_OpenFont("PixelEmulator-xq08.ttf", 100);
	if (font == nullptr) {
		cerr << "Font cannot be loaded: TTF_ERROR" << SDL_GetError() << endl;
		return;
	}
	char key_text = static_cast<char>(key);
	SDL_Surface* surface = TTF_RenderText_Solid(font, &key_text, 1, text_col);
	text = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);
}

KeyTime::~KeyTime() {}

bool KeyTime::time_elapsed() {
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - time_made;
	if (milliseconds >= time_expired) {
		return true;
	}
	return false;
}

void KeyTime::render(SDL_Texture* key_dot) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_FRect target{ x, y, size, size };
	//SDL_RenderFillRect(renderer, &target);
	SDL_RenderTexture(renderer, texture, NULL, &target);

	double progress_angle = progress();
	double progress_x, progress_y;
	progress_x = (x + size / 2) + sin(360 * 3.14159 / 180.0 * progress_angle / 100 +3.14159)*size/2;
	progress_y = (y + size / 2) + cos(360 * 3.14159 / 180.0 * progress_angle / 100 + 3.14159)*size/2;

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_FRect progress_bar{ progress_x, progress_y, size/15, size / 15 };
	SDL_RenderTexture(renderer, key_dot, NULL, &progress_bar);

	SDL_FRect textRect{ x + size / 4, y + size / 4, size / 2, size / 2 };
	SDL_RenderTexture(renderer, text, NULL, &textRect);

	/*char text = static_cast<char>(key);
	SDL_Surface* surface = TTF_RenderText_Solid(font, &text, 1, text_col);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FRect textRect{ x+size/4, y +size/4, size/2, size/2 };
	SDL_RenderTexture(renderer, texture, NULL, &textRect);
	SDL_DestroySurface(surface);
	SDL_DestroyTexture(texture);*/
}

bool KeyTime::inZone() {
	return progress() >= 80;
}

double KeyTime::progress() {
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - time_made;
	return milliseconds / time_expired * 100;
}