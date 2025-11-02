#include "KeyTime.h"

KeyTime::KeyTime(SDL_Keycode KeyT, SDL_Texture* key_text, bool is_good, SDL_Texture* SDL_Texture, pair<int, int> pos): key(KeyT), text(key_text), good(is_good), texture(SDL_Texture), position(pos) {
	time_made = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count(); // work out time key was made
	time_expired = 2500 + (rand() % 3000); // when the key will expire
	if (!is_good) { time_expired += 2000; } // bad keys last more time
}

KeyTime::~KeyTime() {}

bool KeyTime::time_elapsed() { // time since key was made
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - time_made;
	if (milliseconds >= time_expired) {
		return true;
	}
	return false;
}

void KeyTime::render(SDL_Texture* key_dot, SDL_Renderer* renderer) { // render all parts of the key time event
	//outline rendering
	SDL_FRect target{ position.first, position.second, size, size };
	SDL_RenderTexture(renderer, texture, NULL, &target);

	//calculate progress
	double progress_angle = progress();
	double progress_x, progress_y;
	progress_x = (position.first + size / 2) + sin(360 * 3.14159 / 180.0 * progress_angle / 100 +3.14159)*size/2;
	progress_y = (position.second + size / 2) + cos(360 * 3.14159 / 180.0 * progress_angle / 100 + 3.14159)*size/2;

	// render red dot at progress
	SDL_FRect progress_bar{ progress_x, progress_y, size/15, size / 15 };
	SDL_RenderTexture(renderer, key_dot, NULL, &progress_bar);

	//render text
	SDL_FRect textRect{ position.first + size / 4, position.second + size / 4, size / 2, size / 2 };
	SDL_RenderTexture(renderer, text, NULL, &textRect);
}

bool KeyTime::inZone() {
	return progress() >= 75;
}

double KeyTime::progress() { // calculate how far round the key is
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - time_made;
	return milliseconds / time_expired * 100;
}