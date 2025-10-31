#pragma once
#include "GameObject.h"

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

class pathfindingComponent : public Component {
public:
	virtual void update(float deltatime) {
		pair<int, int> cell = owner->getCell();
		pair<int, int> player_cell = player->getCell();
		path = aStar(owner->getWOutline(), cell.first, cell.second, player_cell.first, player_cell.second); // incase player leaves the cell
		if (path.size() > 1) {
			if (path[1].x > cell.first) {
				if (path[1].y > cell.second) { owner->setDirection({ 1,1 }); }
				else if (path[1].y = cell.second) { owner->setDirection({ 1,0 }); }
				else { owner->setDirection({ 1,-1 }); }
			}
			else if (path[1].x == cell.first) {
				if (path[1].y > cell.second) { owner->setDirection({ 0,1 }); }
				else { owner->setDirection({ 0,-1 }); }
			}
			else {
				if (path[1].y > cell.second) { owner->setDirection({ -1,1 }); }
				else if (path[1].y = cell.second) { owner->setDirection({ -1,0 }); }
				else { owner->setDirection({ -1,-1 }); }
			}
		}
	}

	//pathfinding is ai generated

//All below are AI generated for pathfinding created by copilot free
// Returns walkable neighboring nodes (up, right, down, left)
	vector<Node*> getNeighbors(Node* node, const vector<vector<int>>& map) {
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
	vector<Node> reconstructPath(Node* endNode) {
		vector<Node> path;
		for (Node* current = endNode; current != nullptr; current = current->parent) {
			path.push_back(*current);
		}
		reverse(path.begin(), path.end()); // Reverse to get path from start to end
		return path;
	}

	// A* pathfinding algorithm
	vector<Node> aStar(const vector<vector<int>>& map, int startX, int startY, int endX, int endY) {
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

	pathfindingComponent(GameObject* obj, GameObject* play) : Component(obj), player(play) {}
	virtual ~pathfindingComponent() {}
private:
	GameObject* player;

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

	vector<Node> path{ NULL };
};
