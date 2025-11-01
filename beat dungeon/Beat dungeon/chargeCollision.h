#pragma once
#include "monsterObject.h"

class chargeCollisionComponent : public Component {
public:
	virtual void update(float deltatime) {
		if (!owner->getStunned() && !owner->getCharging()) { // if can charge
			if (checkPlayerCollision(player->getDimensions()) && checkLineOfSight()) { // if can see player and is within range
				owner->setCharging(true);
				SDL_FRect target_dim = player->getDimensions();
				SDL_FRect own_dim = owner->getDimensions();
				pair<float, float>  charge_target_dir = { target_dim.x - own_dim.x, target_dim.y - own_dim.y };
				// used to scale the travel direction to allow for constant speed
				float mag = charge_target_dir.first * charge_target_dir.first + charge_target_dir.second * charge_target_dir.second;
				charge_target_dir = { charge_target_dir.first / sqrt(mag), charge_target_dir.second / sqrt(mag) };
				owner->setSpeed(owner->getSpeed() * 2);
				owner->setDirection(charge_target_dir);
				cout << charge_target_dir.first << '\n';
				cout << charge_target_dir.second << '\n';
			}
		}
	}

	bool checkLineOfSight() { // help from https://www.youtube.com/watch?v=NbSee-XM7WA
		SDL_FRect player_dim = player->getDimensions();
		SDL_FRect own_dim = owner->getDimensions();
		int cell_size = owner->getCellSize();
		vector<vector<int>> w_outline = owner->getWOutline();
		pair<float, float> direction{ (player_dim.x + player_dim.w / 2) - (own_dim.x + own_dim.w / 2), (player_dim.y + player_dim.h / 2) - (own_dim.y + own_dim.h / 2) };
		float magnitude = sqrt(direction.first * direction.first + direction.second * direction.second);
		pair<float, float> dir_norm{ direction.first / magnitude, direction.second / magnitude };
		//distance moved along x and y in a cell
		//pair<float, float> dir_cell{ dir_norm.first * cell_size, dir_norm.second * cell_size }; //times cell width/height
		pair<float, float> dir_cell{ sqrt(1 + (dir_norm.second / dir_norm.first) * (dir_norm.second / dir_norm.first)), sqrt(1 + (dir_norm.first / dir_norm.second) * (dir_norm.first / dir_norm.second)) }; //times cell width/height
		//pair<float, float> dir_cell{ sqrt(1 + (dir_norm.first / dir_norm.second) * (dir_norm.first / dir_norm.second)),sqrt(1 + (dir_norm.second / dir_norm.first) * (dir_norm.second / dir_norm.first))}; //times cell width/height
		float dx;
		float dy;

		pair<float, float> length{ 0,0 };

		//quot is cell and rem is where in cell
		div_t x_div = div(own_dim.x + own_dim.w / 2, cell_size);
		div_t y_div = div(own_dim.y + own_dim.h / 2, cell_size);

		pair<int, int> target_cell = player->getCell();

		float cx = x_div.quot;
		float cy = y_div.quot;

		// Establish Starting Conditions
		if (direction.first < 0)
		{
			dx = -1;
			length.first = (x_div.rem / cell_size) * dir_cell.first;
		}
		else
		{
			dx = 1;
			length.first = (1-(x_div.rem / cell_size)) * dir_cell.first;
			//length.first = ((x_div.quot + 1) * cell_size - own_dim.x) * dir_cell.first;
		}

		if (direction.second < 0)
		{
			dy = -1;
			length.second = (y_div.rem / cell_size) * dir_cell.second;
		}
		else
		{
			dy = 1;
			length.second = (1-(y_div.rem / cell_size)) * dir_cell.second;
			//length.second = ((y_div.quot + 1) * cell_size - own_dim.y) * dir_cell.second;
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

	}

	bool checkPlayerCollision(SDL_FRect player_dim) { // if player is within range to charge
		SDL_FRect dim = owner->getDimensions();
		int x_dist = dim.x - player_dim.x;
		int y_dist = dim.y - player_dim.y;
		if (x_dist * x_dist + y_dist * y_dist < range * range) {
			return true;
		}
		return false;
	}

	chargeCollisionComponent(MonsterObj* obj, GameObject* play, int attackRange) : Component(obj), owner(obj), player(play), range(attackRange) {}
	virtual ~chargeCollisionComponent() {}
private:
	MonsterObj* owner;
	GameObject* player;
	int range;
};