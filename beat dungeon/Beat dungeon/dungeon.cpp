#include "dungeon.h"

Dungeon::Dungeon(int width, int height, SDL_Renderer* sdlRenderer): dungeon_width(width), dungeon_height(height), renderer(sdlRenderer) {
	outline.resize(dungeon_height, vector<char>(dungeon_width, 0));
}
Dungeon::~Dungeon(){}

void Dungeon::read_file(string fileName) {
	//retrieve text file for the sppecified level
	ifstream f(fileName);
	if (!f.is_open()) {
		cerr << "Cannot open file";
		return;
	}
	//for retrieving the data char at a time
	char ch;
	int line = 0;
	int col = 0;
	//get next char
	while (f.get(ch)) {
		cout << ch;
		//if end of line got to start of next
		if (ch == '\n') {
			line++;
			col = 0;
		}
		else{
			//assign value to vector
			outline[line][col] = ch;
			col++;
		}
	}
}

void Dungeon::render() {
	int x = 0;
	int y = 0;
	for (const auto& row : outline) {
		for (const auto& column : row) {
			//match each value in the vector (respresents parts of the board)
			switch (column) {
			case '1':
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				break;
			case '2':
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				break;
			case '3':
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			SDL_FRect textRect{ dungeon_x + x * wall_size, dungeon_y + y * wall_size, wall_size, wall_size };
			SDL_RenderFillRect(renderer, &textRect);
			x++;
		}
		x = 0;
		y++;
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
