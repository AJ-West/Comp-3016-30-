#include "dungeon.h"

Dungeon::Dungeon(SDL_Renderer* sdlRenderer, int levelNumber): renderer(sdlRenderer), levelNum(levelNumber) {
	read_file();
	keyHandler = new KeyHandler(renderer, player);
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
		monster->render(renderer);
	}
	keyHandler->renderKeys();
	/*for (auto& key : current_keys) {
		if (key->getGood()) {
			key->render(renderer, key_outline, key_dot);
		}
		else {
			key->render(renderer, key_bad_outline, key_dot);
		}
	}*/
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}


void Dungeon::moveMonsters() {
	for (const auto& monst : monsters) {
		monst->move();
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
				//Skeleton monster(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.0025);
				monsters.push_back(make_unique<Skeleton>(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.005));
				//monsters[i] = monster;
				i++;
			}
			if (column == '5') {
				//monsters.resize(i + 1);
				//Minotaur monster(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.0025, this);
				monsters.push_back(make_unique<Minotaur>(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.005, this));
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
		//code for game
		/*for (auto& keyT : current_keys) {
			if (keyT->getKey() == key) {
				if (keyT->inZone()) {
					player->change_direction(key);
					keyT->setUsed(true);
					break;
				}
			}
		}*/
		// for testing purposes
		player->change_direction(key);
	}
}

void Dungeon::update() {
	spawn_key();
	//removes used keys or timed out keys
	int index = 0;
	vector<int> to_delete;
	keyHandler->checkTimes();
	moveMonsters();
	player->move();
}

void Dungeon::spawn_key() {
	double current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();;
	if (current_time - last_time >= 750) {
		keyHandler->spawnKey();
		last_time = current_time;
	}
}