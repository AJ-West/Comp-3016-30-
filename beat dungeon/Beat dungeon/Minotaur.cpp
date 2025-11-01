#include "Minotaur.h"
#include "dungeon.h"
/*
Minotaur::Minotaur(float start_x, float start_y, Player* player_point, float s_speed, vector<vector<int>> walkable_outline, Dungeon* dungeon): Monster(start_x, start_y, player_point, s_speed, walkable_outline, dungeon) {
	async = new thread(&Minotaur::stun, this);
}

//moves the monster towards the players position
void Minotaur::move() {
	if (!stunned) {
		if (charging) {
			charge();
			checkChargeCollision();
		}
		else {
			updateTargetPos();
			vector<float> float_target{ static_cast<float>(target_pos.first), static_cast<float>(target_pos.second) };
			vector<float> target_dir = { float_target[0] - (x+width/2), float_target[1] - (y+height/2) };
			x += target_dir[0] / (sqrt(target_dir[0] * target_dir[0])) * speed;
			y += target_dir[1] / (sqrt(target_dir[1] * target_dir[1])) * speed;
			checkAttackCollision();
		}
	}
	else {
		updateParticles();
	}
}

void Minotaur::updateParticles() {//ai from free copilot
	// Update particles
	for (size_t i = 0; i < particles.size();) {
		if (!particles[i].update()) {
			particles.erase(particles.begin() + i);
		}
		else {
			i++;
		}
	}
}

void Minotaur::attack() {
	player_pos = player->getPos();
	//need to add delay for attack animation
	vector<pair<int, int>> player_corners;
	player_corners.push_back({ player_pos.first - player_dimen.first / 2, player_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ player_pos.first + player_dimen.first / 2, player_pos.second + player_dimen.second / 2 });
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
	player_corners.push_back({ player_pos.first - player_dimen.first / 2, player_pos.second - player_dimen.second / 2 });
	player_corners.push_back({ player_pos.first + player_dimen.first / 2, player_pos.second + player_dimen.second / 2 });
	if (checkPlayerCollision(player_corners, charge_range) && checkLineOfSight()) {
		cout << "charging";
		pair<float, float> float_target{ static_cast<float>(player_pos.first), static_cast<float>(player_pos.second) };
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

	if (w_outline[centre.second][centre.first] == 1) {
		crash();
	}
}

void Minotaur::crash() {
	stunned = true;
	speed = 0.005;
	charging = false;
	async = new thread(&Minotaur::stun, this);
	async->detach();
	// Spawn multiple particles at mouse click
	for (int i = 0; i < 25; i++) { // ai
		particles.emplace_back(x+width/2, y+height/2, false);
	}
}

void Minotaur::render(SDL_Renderer* renderer) {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_FRect character{ x, y, 40, 40 };
	SDL_RenderTexture(renderer, sprite, &edge_remove, &character);

	for (auto& p : particles) {
		p.render(renderer);
	}

	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderLine(renderer, x+width/2, y+height/2, player_pos.first +player_dimen.first, player_pos.second +player_dimen.second);
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


bool Minotaur::checkLineOfSight() { // help from https://www.youtube.com/watch?v=NbSee-XM7WA
	pair<float, float> direction{ (player_pos.first + player_dimen.first / 2) - (x +width/2), (player_pos.second + player_dimen.second / 2) - (y+height/2)};
	float magnitude = sqrt(direction.first * direction.first + direction.second * direction.second);
	pair<float, float> dir_norm{ direction.first / magnitude, direction.second / magnitude };
	//distance moved along x and y in a cell
	pair<float, float> dir_cell{ dir_norm.first * dung->getWallSize(), dir_norm.second * dung->getWallSize() }; //times cell width/height
	float dx;
	float dy;

	pair<float, float> length{ 0,0 };

	//quot is cell and rem is where in cell
	div_t x_div = div(x + width / 2 - dung->getDungeonX(), dung->getWallSize());
	div_t y_div = div(y + height / 2 - dung->getDungeonY(), dung->getWallSize());

	pair<int, int> target_cell{ div(player_pos.first + player_dimen.first / 2 - dung->getDungeonX(), dung->getWallSize()).quot , div(player_pos.second + player_dimen.second / 2 - dung->getDungeonY(), dung->getWallSize()).quot };

	float cx = x_div.quot;
	float cy = y_div.quot;

	// Establish Starting Conditions
	if (direction.first < 0)
	{
		dx = -1;
		length.first = (x_div.rem) * dir_cell.first;
	}
	else
	{
		dx = 1;
		length.first = (1 - x_div.rem) * dir_cell.first;
	}

	if (direction.second < 0)
	{
		dy = -1;
		length.second = (y_div.rem) * dir_cell.second;
	}
	else
	{
		dy = 1;
		length.second = (1-y_div.rem) * dir_cell.second;
	}

	bool collided = false;
	while (!collided) {
		//see if colliding with wall moving through the x or the y axis (left/right or top/bottom)
		if (length.first < length.second) {
			cx += dx;
			length.first += dir_cell.first;
		}
		else {
			cy += dy;
			length.second += dir_cell.second;
		}
		// is cell wall if so cannot see the player
		if (w_outline[cy][cx] == 1) { return false; }
		// if is the players cell then can see the player
		if (cx == target_cell.first && cy == target_cell.second) { return true; }
	}


	int check_points = magnitude / 1;

}*/