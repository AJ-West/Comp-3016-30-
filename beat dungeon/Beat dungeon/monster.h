#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

class Player;
class Minotaur;
class Skeleton;


class Monster {
public:
	//constructor
	Monster(float start_x, float start_y, Player* player_point, float s_speed);
	~Monster();
	//update the monster position
	virtual void move() = 0;
	virtual void attack() = 0;

	virtual void checkAttackCollision() = 0;

	void updateTargetPos();

	void loadTexture(SDL_Renderer* renderer, const char* file);

	//draw monster
	virtual void render(SDL_Renderer* SDL_renderer);

	bool checkPlayerCollision(vector<pair<int, int>> player_corners, int range);
	bool checkCorner(int corner_x, int corner_y, vector<pair<int, int>> player_corners, int range);

	//setters
	//setters
	void setWallTypes(vector<char> walls) { wallTypes = walls; }

protected:
	// monster position
	float x, y;
	float width = 20, height = 20;
	pair<int, int> target_pos{ 0,0 };
	pair<int, int> player_dimen;
	float speed;
	float attackRange = 10;
	bool canAttack = true;

	vector<char> wallTypes;

	SDL_Texture* sprite = nullptr;
	SDL_FRect edge_remove{ 6, 0, 20, 32 };

private:
	Player* player;
};