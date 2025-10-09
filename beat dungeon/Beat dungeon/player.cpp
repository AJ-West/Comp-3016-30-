#include "player.h"

Player::Player(int start_x, int start_y, SDL_Renderer* SDL_renderer): x(start_x), y(start_y), renderer(SDL_renderer) {
	movement_keys.resize(4, vector<SDL_Keycode>(1, 0));
	movement_keys[0][0] = SDLK_W;
	movement_keys[1][0] = SDLK_A;
	movement_keys[2][0] = SDLK_S;
	movement_keys[3][0] = SDLK_D;
}
Player::~Player() {}

void Player::move(SDL_Keycode key) {
	int index = 0;
	for (const auto& row : movement_keys) {
		auto it = find(row.begin(), row.end(), key);
		if (it != row.end()) {
			break;
		}
		index++;
	}
	switch (index) {
	case 0:
		y -= 20;
		break;
	case 1:
		x -= 20;
		break;
	case 2:
		y += 20;
		break;
	case 3:
		x += 20;
		break;
	}
}

void Player::handle_input(SDL_Event input) {
	SDL_Keycode key = input.key.key;
	if (key != SDLK_SPACE) {
		move(key);
	}
}

void Player::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_FRect character{ x, y, 20, 20 };
	SDL_RenderFillRect(renderer, &character);
}