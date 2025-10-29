#include "monster.h"
#include "player.h"
#include "dungeon.h"

Monster::Monster(float start_x, float start_y, Player* player_point, float s_speed, vector<vector<int>> walkable_outline, Dungeon* dungeon) {
	x = start_x;
	y = start_y;
	player = player_point;
	speed = s_speed;
	player_dimen = player->getDimensions();
	w_outline = walkable_outline;
	dung = dungeon;
}
Monster::~Monster() {}

void Monster::updateTargetPos() {
	player_pos = player->getPos();
	pair<int,int> cell(div(x +width/2 - dung->getDungeonX(), dung->getWallSize()).quot, div(y + height/2 - dung->getDungeonY(), dung->getWallSize()).quot);
	/*pair<int, int> player_cell = player->getCell();
	if (player_cell.first != cell.first || player_cell.second != cell.second) {
		path = aStar(w_outline, cell.first, cell.second, player_cell.first, player_cell.second); // incase player leaves the cell
		target_pos = make_pair(path[1].x * dung->getWallSize() + dung->getWallSize() / 2 + dung->getDungeonX(), path[1].y * dung->getWallSize() + dung->getWallSize() / 2 + dung->getDungeonY());
	}
	else {
		target_pos = player_pos;
	}*/
	if (path.size() == 0) { //initiation
		pair<int, int> player_cell = player->getCell();
		path = aStar(w_outline, cell.first, cell.second, player_cell.first, player_cell.second); // incase player leaves the cell
		target_pos = make_pair(path[1].x * dung->getWallSize() + dung->getWallSize() / 2 + dung->getDungeonX(), path[1].y * dung->getWallSize() + dung->getWallSize() / 2 + dung->getDungeonY());
	}
	else if (path.size() == 1) {
		pair<int, int> player_cell = player->getCell();
		path = aStar(w_outline, cell.first, cell.second, player_cell.first, player_cell.second); // incase player leaves the cell)
		target_pos = player_pos;
	}
	else if (cell.first == path[1].x && cell.second == path[1].y) {
		pair<int, int> player_cell = player->getCell();
		path = aStar(w_outline, cell.first, cell.second, player_cell.first, player_cell.second);
		if (path.size() == 1 || path.size() == 0) {//acounts for issues when charging
			target_pos = player_pos;
		}
		else {
			target_pos = make_pair(path[1].x * dung->getWallSize() + dung->getWallSize() / 2 + dung->getDungeonX(), path[1].y * dung->getWallSize() + dung->getWallSize() / 2 + dung->getDungeonY());
		}
	}
}

void Monster::render(SDL_Renderer* renderer) {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_FRect character{ x, y, 20, 20 };
	SDL_RenderTexture(renderer, sprite, &edge_remove, &character);
	//SDL_RenderFillRect(renderer, &character);
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
	x_dist = player_corners[0].first - corner_x;
	y_dist = player_corners[1].second - corner_y;
	if (x_dist * x_dist + y_dist * y_dist < range * range) {
		return true;
		cout << "game over";
	}
	x_dist = player_corners[1].first - corner_x;
	y_dist = player_corners[0].second - corner_y;
	if (x_dist * x_dist + y_dist * y_dist < range * range) {
		return true;
		cout << "game over";
	}
	x_dist = player_corners[1].first - corner_x;
	y_dist = player_corners[1].second - corner_y;
	if (x_dist * x_dist + y_dist * y_dist < range * range) {
		return true;
		cout << "game over";
	}
	return false;
}

void Monster::loadTexture(SDL_Renderer* renderer, const char* file) {
	SDL_Surface* scaleSurface = IMG_Load(file);
	if (!scaleSurface) {
		std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	sprite = SDL_CreateTextureFromSurface(renderer, scaleSurface);
	SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
	if (!sprite) {
		std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}
}

//All below are AI generated for pathfinding created by copilot free
// Returns walkable neighboring nodes (up, right, down, left)
vector<Node*> Monster::getNeighbors(Node* node, const vector<vector<int>>& map) {
	vector<Node*> neighbors;

	// Directions: N, NE, E, SE, S, SW, W, NW
	int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	float cost[] = { 1.0f, 1.414f, 1.0f, 1.414f, 1.0f, 1.414f, 1.0f, 1.414f };

	for (int i = 0; i < 8; ++i) {
		int nx = node->x + dx[i];
		int ny = node->y + dy[i];

		// Check bounds
		if (nx >= 0 && ny >= 0 && nx < map[0].size() && ny < map.size()) {
			// Check if tile is walkable
			if (map[ny][nx] == 0) {
				// Optional: prevent corner cutting
				if (i % 2 == 1) { // Diagonal
					int adj1x = node->x + dx[i];
					int adj1y = node->y;
					int adj2x = node->x;
					int adj2y = node->y + dy[i];
					if (map[adj1y][adj1x] != 0 || map[adj2y][adj2x] != 0) continue;
				}

				neighbors.push_back(new Node{ nx, ny, cost[i], 0, nullptr });
			}
		}
	}

	return neighbors;
}

// Reconstructs the path from end node to start node by following parent pointers
vector<Node> Monster::reconstructPath(Node* endNode) {
	vector<Node> path;
	for (Node* current = endNode; current != nullptr; current = current->parent) {
		path.push_back(*current);
	}
	reverse(path.begin(), path.end()); // Reverse to get path from start to end
	return path;
}

// A* pathfinding algorithm
vector<Node> Monster::aStar(const vector<vector<int>>& map, int startX, int startY, int endX, int endY) {
	// Priority queue to store nodes to explore, sorted by lowest fCost
	priority_queue<Node*, vector<Node*>, Compare> openSet;

	// Map to store all created nodes for memory management and lookup
	unordered_map<int, Node*> allNodes;

	// Hash function to uniquely identify a node by its coordinates
	auto hash = [](int x, int y, int width) { return y * width + x; };

	// Create and initialize the start node
	Node* start = new Node{ startX, startY, 0, heuristic(startX, startY, endX, endY), nullptr };
	openSet.push(start);
	allNodes[hash(startX, startY, map[0].size())] = start;

	// Main loop: continue until there are no nodes left to explore
	while (!openSet.empty()) {
		Node* current = openSet.top(); // Get node with lowest fCost
		openSet.pop();

		// If we've reached the goal, reconstruct and return the path
		if (current->x == endX && current->y == endY) {
			auto path = reconstructPath(current);
			for (auto& pair : allNodes) delete pair.second; // Clean up memory
			return path;
		}

		// Explore neighbors of the current node
		for (Node* neighbor : getNeighbors(current, map)) {
			int id = hash(neighbor->x, neighbor->y, map[0].size());
			float tentativeG = current->gCost + 1; // Assume uniform cost between tiles

			// If this neighbor is new or we found a shorter path to it
			if (allNodes.find(id) == allNodes.end() || tentativeG < allNodes[id]->gCost) {
				neighbor->gCost = tentativeG;
				neighbor->hCost = heuristic(neighbor->x, neighbor->y, endX, endY);
				neighbor->parent = current;
				openSet.push(neighbor);
				allNodes[id] = neighbor; // Store or update the node
			}
			else {
				delete neighbor; // Discard duplicate or worse path
			}
		}
	}

	// No path found: clean up and return empty path
	for (auto& pair : allNodes) delete pair.second;
	return {};
}