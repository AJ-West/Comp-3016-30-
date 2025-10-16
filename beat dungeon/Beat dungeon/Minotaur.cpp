#include "Minotaur.h"

//moves the monster towards the players position
void Minotaur::move() {
	updateTargetPos();
	if (charging) {
		charge();
	}
	else {
		vector<float> float_target{ static_cast<float>(target_pos.first), static_cast<float>(target_pos.second) };
		vector<float> target_dir = { float_target[0] - x, float_target[1] - y };
		x += target_dir[0] / (sqrt(target_dir[0] * target_dir[0])) * speed;
		y += target_dir[1] / (sqrt(target_dir[1] * target_dir[1])) * speed;
	}
	checkAttackCollision();
}

void Minotaur::attack() {
	//need to add delay for attack animation
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ target_pos.first - player_dimen.first / 2, target_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ target_pos.first + player_dimen.first / 2, target_pos.second + player_dimen.second / 2 });
	if (checkPlayerCollision(player_corners, attackRange)) {
		cout << "game over";
	}
}

void Minotaur::checkAttackCollision() {
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ target_pos.first - player_dimen.first / 2, target_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ target_pos.first + player_dimen.first / 2, target_pos.second + player_dimen.second / 2 });
	if (checkPlayerCollision(player_corners, charge_range)) {
		pair<float, float> float_target{ static_cast<float>(target_pos.first), static_cast<float>(target_pos.second) };
		charge_target_dir = { float_target.first - x, float_target.second - y };
		charging = true;
	}
}

void Minotaur::charge() {
	x += charge_target_dir.first / (sqrt(charge_target_dir.first * charge_target_dir.first)) * speed;
	y += charge_target_dir.first / (sqrt(charge_target_dir.first * charge_target_dir.first)) * speed;
	checkChargeCollision();
}

void Minotaur::checkChargeCollision() {
	attack();
	checkWallCollision();
}

void Minotaur::checkWallCollision() {
	// get the cell index for each corner of the player
	pair<int, int> x_bounds(div(x - dung->getDungeonX(), dung->getWallSize()).quot, div(x - dung->getDungeonX() + width - 1, dung->getWallSize()).quot);
	pair<int, int> y_bounds(div(y - dung->getDungeonY(), dung->getWallSize()).quot, div(y - dung->getDungeonY() + height - 1, dung->getWallSize()).quot);

	// check if any corners of the player is within an exit and crashes the minotaur is so
	if (dung->getOutline()[y_bounds.first][x_bounds.first] == '3') {
		crash();
	}
	else if (dung->getOutline()[y_bounds.first][x_bounds.second] == '3') {
		crash();
	}
	else if (dung->getOutline()[y_bounds.second][x_bounds.first] == '3') {
		crash();
	}
	else if (dung->getOutline()[y_bounds.second][x_bounds.second] == '3') {
		crash();
	}
}

void Minotaur::crash() {
	cout << "pause";
	charging = false;
}