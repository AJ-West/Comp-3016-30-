#include "Minotaur.h"
#include "dungeon.h"

//moves the monster towards the players position
void Minotaur::move() {
	if (!stunned) {
		updateTargetPos();
		if (charging) {
			charge();
			checkChargeCollision();
		}
		else {
			vector<float> float_target{ static_cast<float>(target_pos.first), static_cast<float>(target_pos.second) };
			vector<float> target_dir = { float_target[0] - x, float_target[1] - y };
			x += target_dir[0] / (sqrt(target_dir[0] * target_dir[0])) * speed;
			y += target_dir[1] / (sqrt(target_dir[1] * target_dir[1])) * speed;
			checkAttackCollision();
		}
	}
}

void Minotaur::attack() {
	//need to add delay for attack animation
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ target_pos.first - player_dimen.first / 2, target_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ target_pos.first + player_dimen.first / 2, target_pos.second + player_dimen.second / 2 });
	if (checkPlayerCollision(player_corners, attackRange)) {
		canAttack = false;
		cout << "game over";
		//async = new thread(&Minotaur::attackDelay, this);
		hit = true;
		//async->detach();
	}
}

void Minotaur::checkAttackCollision() {
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ target_pos.first - player_dimen.first / 2, target_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ target_pos.first + player_dimen.first / 2, target_pos.second + player_dimen.second / 2 });
	if (checkPlayerCollision(player_corners, charge_range)) {
		pair<float, float> float_target{ static_cast<float>(target_pos.first), static_cast<float>(target_pos.second) };
		charge_target_dir = { float_target.first - x, float_target.second - y };
		// used to scale the travel direction to allow for constant speed
		float mag = charge_target_dir.first* charge_target_dir.first + charge_target_dir.second* charge_target_dir.second;
		charge_target_dir = { charge_target_dir.first / sqrt(mag), charge_target_dir.second / sqrt(mag) };
		speed = 0.075;
		charging = true;
	}
}

void Minotaur::charge() {
	x += charge_target_dir.first * speed;
	y += charge_target_dir.second * speed;
	checkChargeCollision();
}

void Minotaur::checkChargeCollision() {
	if (canAttack) {
		attack();
	}
	checkWallCollision();
}

void Minotaur::checkWallCollision() {
	pair<int, int> centre(div(x + width/2 - dung->getDungeonX(), dung->getWallSize()).quot, div(y + height/2 - dung->getDungeonY(), dung->getWallSize()).quot);

	if (find(wallTypes.begin(), wallTypes.end(), dung->getOutline()[centre.second][centre.first]) != wallTypes.end()) {
		crash();
	}
}

void Minotaur::crash() {
	stunned = true;
	speed = 0.005;
	charging = false;
	async = new thread(&Minotaur::stun, this);
	async->detach();
}

void Minotaur::render(SDL_Renderer* renderer) {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_FRect character{ x, y, 40, 40 };
	SDL_RenderTexture(renderer, sprite, &edge_remove, &character);
	//SDL_RenderFillRect(renderer, &character);
}

void Minotaur::stun() {
	this_thread::sleep_for(chrono::seconds(3));
	stunned = false;
}

void Minotaur::attackDelay() {
	this_thread::sleep_for(chrono::seconds(3));
	canAttack = true;
}
