#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <queue>
#include <cmath>
#include <unordered_map>


using namespace std;

class Player;
class Minotaur;
class Skeleton;
class Dungeon;

struct Node { //ai generated for pathfinding
	int x, y;               // Coordinates of the node
	float gCost, hCost;     // gCost: cost from start to this node, hCost: estimated cost to goal
	Node* parent;           // Pointer to parent node for path reconstruction

	// Total cost = gCost + hCost
	float fCost() const { return gCost + hCost; }

	// Comparison operator for priority queue (min-heap)
	bool operator>(const Node& other) const {
		return fCost() > other.fCost();
	}
};

class Monster {
public:
	//constructor
	Monster(float start_x, float start_y, Player* player_point, float s_speed, vector<vector<int>> walkable_outline, Dungeon* dungeon);
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

	//getters
	bool getHit() { return hit; }

	//setters
	void setWallTypes(vector<char> walls) { wallTypes = walls; }

protected:
	// monster position
	float x, y;
	float width = 20, height = 20;
	pair<int, int> target_pos{ 0,0 };
	pair<float, float> player_pos{ 0,0 };
	pair<int, int> player_dimen;
	float speed;
	float attackRange = 10;
	bool canAttack = true;
	bool hit = false;

	Player* player;

	vector<vector<int>> w_outline;
	Dungeon* dung;

	vector<char> wallTypes;

	SDL_Texture* sprite = nullptr;
	SDL_FRect edge_remove{ 6, 0, 20, 32 };

	vector<Node> path{NULL};

    // Returns walkable neighboring nodes (up, right, down, left)
	vector<Node*> getNeighbors(Node* node, const vector<vector<int>>& map);

    // Reconstructs the path from end node to start node by following parent pointers
	vector<Node> reconstructPath(Node* endNode);

    // A* pathfinding algorithm
	vector<Node> aStar(const vector<vector<int>>& map, int startX, int startY, int endX, int endY);

private:

	//ai generated for pathfinding

	// Comparator for priority queue to sort nodes by lowest fCost
	struct Compare {
		bool operator()(Node* a, Node* b) {
			return a->fCost() > b->fCost();
		}
	};

	// Heuristic function using Manhattan distance
	float heuristic(int x1, int y1, int x2, int y2) {
		return abs(x1 - x2) + abs(y1 - y2);
	}
};