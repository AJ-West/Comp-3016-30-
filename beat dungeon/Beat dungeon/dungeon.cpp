#include "dungeon.h"

Dungeon::Dungeon(SDL_Renderer* sdlRenderer, int levelNumber): renderer(sdlRenderer), levelNum(levelNumber) {
	read_file();
}
Dungeon::~Dungeon(){}

void Dungeon::read_file() {
	string fileName = "levels/level" + to_string(levelNum) + ".txt";
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
	vector<char> row;
	//get next char
	while (f.get(ch)) {
		//if end of line got to start of next
		if (ch == '\n') {
			outline.push_back(row);
			row.clear();
			//line++;
			//col = 0;
		}
		else{
			row.push_back(ch);
			//assign value to vector
			//outline[line][col] = ch;
			//col++;
		}
	}
	outline.push_back(row);
	spawn_entities();
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
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
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
	player->render();
	for (auto& monster : monsters) {
		monster.render();
	}
	for (auto& key : current_keys) {
		key->render(renderer);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}


void Dungeon::moveMonsters() {
	for (auto& monst : monsters) {
		monst.move();
	}
}

void Dungeon::spawn_entities() {
	int i = 0;
	int x = 0;
	int y = 0;
	for (const auto& row : outline) {
		x = 0;
		for (const auto& column : row) {
			if (column == '2') {
				//monsters.resize(i + 1);
				Monster monster(x * wall_size + dungeon_x, y * wall_size + dungeon_y, renderer, player, 0.0025);
				monsters.push_back(monster);
				//monsters[i] = monster;
				i++;
			}
			if (column == '4') {
				player = new Player(x * wall_size + dungeon_x, y * wall_size + dungeon_y, renderer, this);
			}
			x++;
		}
		y++;
	}
}

void Dungeon::handleInput(SDL_Event input) {
	SDL_Keycode key = input.key.key;
	// if a key for movement
	if (key != SDLK_SPACE) {
		for (auto& keyT : current_keys) {
			if (keyT->getKey() == key) {
				if (keyT->inZone()) {
					player->move(key);
				}
			}
		}
	}
}

void Dungeon::update() {
	time_t current_time = time(0);
	if (current_time - last_time >= 1) {
		vector<vector<SDL_Keycode>> options = player->getMovementKeys();
		int random_direction = rand() % 4;
		SDL_Keycode code = options[random_direction][rand() % size(options[random_direction])];
		current_keys.push_back(new KeyTime(code));
		last_time = current_time;
	}
	int index = 0;
	vector<int> to_delete;
	for (auto& key : current_keys) {
		if (key->time_elapsed()) {
			to_delete.push_back(index);
		}
		index++;
	}
	int count = 0;
	for (auto& ind : to_delete) {
		delete current_keys[ind];
		current_keys.erase(current_keys.begin() + ind - count);
		count++;
	}
	moveMonsters();
}