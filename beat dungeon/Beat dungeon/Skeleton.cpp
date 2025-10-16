#include "Skeleton.h"

//moves the monster towards the players position
void Skeleton::move() {
	updateTargetPos();
	vector<float> float_target{ static_cast<float>(target_pos.first), static_cast<float>(target_pos.second) };
	vector<float> target_dir = { float_target[0] - x, float_target[1] - y };
	x += target_dir[0] / (sqrt(target_dir[0] * target_dir[0])) * speed;
	y += target_dir[1] / (sqrt(target_dir[1] * target_dir[1])) * speed;
	checkAttackCollision();
}

void Skeleton::attack() {
	//need to add delay for attack animation
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ target_pos.first - player_dimen.first / 2, target_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ target_pos.first + player_dimen.first / 2, target_pos.second + player_dimen.second / 2 });
	if (checkPlayerCollision(player_corners, attackRange)) {
		cout << "game over";
	}
}

void Skeleton::checkAttackCollision() {
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ target_pos.first - player_dimen.first / 2, target_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ target_pos.first + player_dimen.first / 2, target_pos.second + player_dimen.second / 2 });
	if (checkPlayerCollision(player_corners, range)) {
		attack();
	}
}