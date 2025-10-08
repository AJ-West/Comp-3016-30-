#include "dungeon.h"

Dungeon::Dungeon(int width, int height, SDL_Renderer* sdlRenderer): renderer(sdlRenderer) {
	dungeon_width = width;
	dungeon_height = height;
	outline.resize(dungeon_height, vector<char>(dungeon_width, 0));
}
Dungeon::~Dungeon(){}

void Dungeon::read_file(string fileName) {
	ifstream f(fileName);
	if (!f.is_open()) {
		cerr << "Cannot open file";
		return;
	}
	char ch;
	int line = 0;
	int col = 0;
	while (f.get(ch)) {
		cout << ch;
		if (ch == '\n') {
			line++;
			col = 0;
		}
		else{
			outline[line][col] = ch;
			col++;
		}
	}
}

void Dungeon::load() {
	int x = 0;
	int y = 0;
	for (const auto& row : outline) {
		for (const auto& column : row) {
			switch (column) {
			case '1':
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				break;
			case '2':
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				break;
			case '3':
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			SDL_FRect textRect{ x * 60, y * 60, 60, 60 };
			SDL_RenderFillRect(renderer, &textRect);
			x++;
		}
		x = 0;
		y++;
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Dungeon::render() {

}
