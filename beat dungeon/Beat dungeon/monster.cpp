#include "monster.h"

Monster::Monster(){}
Monster::Monster(float start_x, float start_y, SDL_Renderer* SDL_renderer, Player* player_point, float s_speed): x(start_x), y(start_y), renderer(SDL_renderer), player(player_point), speed(s_speed) {
	player_dimen = player->getDimensions();
}
Monster::~Monster() {}

void Monster::updateTargetPos() {
	target_pos = player->getPos();
}

void Monster::render() {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_FRect character{ x, y, 20, 20 };
	SDL_RenderFillRect(renderer, &character);
}


bool Monster::checkPlayerCollision(vector<pair<int, int>> player_corners, int range) {
	if (checkCorner(x, y, player_corners, range)) { return true; }
	if (checkCorner(x, y + height, player_corners, range)) { return true; }
	if (checkCorner(x+width, y, player_corners, range)) { return true; }
	if (checkCorner(x+width, y+height, player_corners, range)) { return true; }
	return false;
}

bool Monster::checkCorner(int corner_x, int corner_y, vector<pair<int, int>> player_corners, int range) {
	int x_dist = player_corners[0].first - corner_x;
	int y_dist = player_corners[0].second - corner_y;
	if (x_dist*x_dist + y_dist*y_dist < range * range) {
		return true;
		cout << "game over";
	}
	return false;
	/*if (player_corners[0].first < corner_x && corner_x < player_corners[1].first) {
		if (player_corners[0].second < corner_y && corner_y < player_corners[1].second) {
			cout << "game over";
		}
	}*/
}
