#pragma once
#pragma once

#include <vector>
#include <SDL3/SDL.h>

using namespace std;

class Tilemap { //github copilot generated class
public:
    Tilemap(int width, int height, vector<vector<int>> outline) : width(width), height(height), tiles(outline) {}
    int getTile(int x, int y) const;
	vector<vector<SDL_FRect>> identifyWalls();

	vector<int> isTileAdjacent(int x, int y, int tileType);

	vector<vector<SDL_FRect>> getTileLocs() { return tile_locs; }

private:
    int width;
    int height;
    vector<vector<int>> tiles;

	vector<vector<SDL_FRect>> tile_locs;
};

int Tilemap::getTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x];
    }
    return -1; // Invalid tile
}

vector<vector<SDL_FRect>> Tilemap::identifyWalls() {
	SDL_FRect loc{ 0,0,32,32 };
    for (int y = 0; y < height; ++y) {
		vector<SDL_FRect> y_row;
        for (int x = 0; x < width; ++x) {
            if (tiles[y][x] == 1) { // Assuming 1 is a wall tile
				vector<int> adjacent = isTileAdjacent(x, y, 1);
				// Determine wall type based on adjacent tiles
				if (adjacent[0] != 1 && adjacent[1] != 1 && (adjacent[2] == 1 || adjacent[3] == 1)) {
					// Vertical wall
					loc.x = 96;
					loc.y = 0;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if ((adjacent[0] == 1 || adjacent[1] == 1) && adjacent[2] != 1 && adjacent[3] != 1) {
					// Horizontal wall
					loc.x = 0;
					loc.y = 0;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] != 1 && adjacent[1] == 1 && adjacent[2] != 1 && adjacent[3] == 1) {
					// Top-left corner
					loc.x = 32;
					loc.y = 0;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] == 1 && adjacent[1] != 1 && adjacent[2] != 1 && adjacent[3] == 1) {
					// Top-right corner
					loc.x = 64;
					loc.y = 0;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] != 1 && adjacent[1] == 1 && adjacent[2] == 1 && adjacent[3] != 1) {
					// Bottom-left corner
					loc.x = 32;
					loc.y = 32;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] == 1 && adjacent[1] != 1 && adjacent[2] == 1 && adjacent[3] != 1) {
					// Bottom-right corner
					loc.x = 64;
					loc.y = 32;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] == 1 && adjacent[1] == 1 && adjacent[2] == 1 && adjacent[3] != 1) {
					// either side and up 
					loc.x = 96;
					loc.y = 32;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] == 1 && adjacent[1] == 1 && adjacent[2] != 1 && adjacent[3] == 1) {
					// either side and down 
					loc.x = 128;
					loc.y = 32;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] == 1 && adjacent[1] == 1 && adjacent[2] != 1 && adjacent[3] == 1) {
					// up down left
					loc.x = 64;
					loc.y = 0;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else if (adjacent[0] == 1 && adjacent[1] == 1 && adjacent[2] != 1 && adjacent[3] == 1) {
					// up down right
					loc.x = 32;
					loc.y = 0;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
				else {
					// Isolated wall or other configuration
					loc.x = 0;
					loc.y = 0;
					y_row.emplace_back(loc); // Wall location in tilemap sprite
				}
            }
            else if (tiles[y][x] == 3) { // 2x
				loc.x = 0;
				loc.y = 64;
				y_row.emplace_back(loc);
            }
			else if (tiles[y][x] == 4) { // new keys
				loc.x = 32;
				loc.y = 64;
				y_row.emplace_back(loc);
			}
			else if (tiles[y][x] == 5) { // rotate keys
				loc.x = 0;
				loc.y = 96;
				y_row.emplace_back(loc);
			}
			else if (tiles[y][x] == 2) { // door
				loc.x = 32;
				loc.y = 96;
				y_row.emplace_back(loc);
			}
			else { // empty tile
				loc.x = 600;
				loc.y = 600;
				y_row.emplace_back(loc);
			}
        }
		tile_locs.push_back(y_row);
		y_row.clear();
	}
	return tile_locs;
}

vector<int> Tilemap::isTileAdjacent(int x, int y, int tileType) {
	vector<int> adjacent{ getTile(x - 1, y), getTile(x + 1, y), getTile(x, y - 1), getTile(x, y + 1) }; // left, right, up, down
	return adjacent;
}
