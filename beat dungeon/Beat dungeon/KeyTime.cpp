#include "KeyTime.h"

KeyTime::KeyTime() {
	srand(time(0));
	max = 5000 + rand() % 10000;
	countdown = max;
}

/*KeyTime::KeyTime(Player* play) : player(play) {
	max = 1 + rand() % 100;
	countdown = max;
}*/
KeyTime::~KeyTime() {}

void KeyTime::decrease() {
	countdown--;
	cout << countdown;
}

void KeyTime::render(SDL_Renderer* renderer) {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_FRect background{ 0, 0, 20, 100 };
	SDL_RenderFillRect(renderer, &background);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_FRect target{ 0, 80, 20, 20 };
	SDL_RenderFillRect(renderer, &target);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_FRect progress{ 0, 100-(countdown*100)/max, 20, 20};
	SDL_RenderFillRect(renderer, &progress);
}