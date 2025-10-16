#include "monster.h"

Monster::Monster() {}
Monster::Monster(float start_x, float start_y, SDL_Renderer* SDL_renderer, Player* player_point, float s_speed) : x(start_x), y(start_y), renderer(SDL_renderer), player(player_point), speed(s_speed) {
	player_dimen = player->getDimensions();
}
Monster::~Monster() {}

//moves the monster towards the players position
void Monster::move() {
	target_pos = player->getPos();
	vector<float> float_target{ static_cast<float>(target_pos.first), static_cast<float>(target_pos.second) };
	vector<float> target_dir = { float_target[0] - x, float_target[1] - y };
	x += target_dir[0] / (sqrt(target_dir[0] * target_dir[0])) * speed;
	y += target_dir[1] / (sqrt(target_dir[1] * target_dir[1])) * speed;
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ target_pos.first - player_dimen.first / 2, target_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ target_pos.first + player_dimen.first / 2, target_pos.second + player_dimen.second / 2 });
	checkPlayerCollision(player_corners);
}

void Monster::render() {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_FRect character{ x, y, 20, 20 };
	SDL_RenderFillRect(renderer, &character);
}


void Monster::checkPlayerCollision(vector<pair<int, int>> player_corners) {
	checkCorner(x, y, player_corners);
	checkCorner(x, y + height, player_corners);
	checkCorner(x + width, y, player_corners);
	checkCorner(x + width, y + height, player_corners);
}

void Monster::checkCorner(int corner_x, int corner_y, vector<pair<int, int>> player_corners) {
	if (player_corners[0].first < corner_x && corner_x < player_corners[1].first) {
		if (player_corners[0].second < corner_y && corner_y < player_corners[1].second) {
			cout << "game over";
		}
	}
}
