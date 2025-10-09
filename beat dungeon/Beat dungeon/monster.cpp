#include "monster.h"

Monster::Monster(float start_x, float start_y, SDL_Renderer* SDL_renderer, Player* player_point, float s_speed): x(start_x), y(start_y), renderer(SDL_renderer), player(player_point), speed(s_speed) {}
Monster::~Monster() {}

//moves the monster towards the players position
void Monster::move() {
	target_pos = player->getPos();
	vector<float> float_target{ static_cast<float>(target_pos[0]), static_cast<float>(target_pos[1]) };
	vector<float> target_dir = { float_target[0] - x, float_target[1] - y };
	x += target_dir[0] / (sqrt(target_dir[0] * target_dir[0])) * speed;
	y += target_dir[1] / (sqrt(target_dir[1] * target_dir[1])) * speed;
}

void Monster::render() {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_FRect character{ x, y, 20, 20 };
	SDL_RenderFillRect(renderer, &character);
}