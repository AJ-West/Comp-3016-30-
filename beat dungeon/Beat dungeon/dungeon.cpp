#include "dungeon.h"

Dungeon::Dungeon(SDL_Renderer* sdlRenderer, int levelNumber): renderer(sdlRenderer), levelNum(levelNumber) {
	read_file();
	keyHandler = new KeyHandler(renderer, player);
	SDL_Surface* scaleSurface = IMG_Load("images/walls.png");
	if (!scaleSurface) {
		std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	tileset = SDL_CreateTextureFromSurface(renderer, scaleSurface);
	SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
	if (!tileset) {
		std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}
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
	save_tiles();
	spawn_entities();
}

void Dungeon::save_tiles() {
	vector<SDL_FRect> tile_row;
	SDL_FRect tile_loc{ 0,0,32,32 };
	for (const auto& row : outline) {
		for (const auto& column : row) {
			//match each value in the vector (respresents parts of the board)
			switch (column) {
			case '1': //wall
				tile_loc.x = 0;
				tile_loc.y = 0;
				tile_row.push_back(tile_loc);
				break;
			case '2': //door
				tile_loc.x = 0;
				tile_loc.y = 32;
				tile_row.push_back(tile_loc);
				break;
			case '3': //corner 1
				tile_loc.x = 64;
				tile_loc.y = 32;
				tile_row.push_back(tile_loc);
				break;
			case '4': //corner 2
				tile_loc.x = 96;
				tile_loc.y = 32;
				tile_row.push_back(tile_loc);
				break;
			case '5': //corner 3
				tile_loc.x = 64;
				tile_loc.y = 64;
				tile_row.push_back(tile_loc);
				break;
			case '6': //corner 4
				tile_loc.x = 96;
				tile_loc.y = 64;
				tile_row.push_back(tile_loc);
				break;
			case '7': //vertical wall
				tile_loc.x = 32 + 32 * rand() % 2;
				tile_loc.y = 0;
				tile_row.push_back(tile_loc);
				break;
			case '8': //horizontal wall
				tile_loc.x = 96 + 32 * rand() % 2;
				tile_loc.y = 0;
				tile_row.push_back(tile_loc);
				break;
			case '9': //trapdoor
				tile_loc.x = 32;
				tile_loc.y = 32;
				tile_row.push_back(tile_loc);
				break;
			default:
				tile_loc.x = 100;
				tile_loc.y = 100;
				tile_row.push_back(tile_loc);
			}
		}
		tiles.push_back(tile_row);
		tile_row.clear();
	}
}

void Dungeon::render_tiles() {
	int x = 0;
	int y = 0;
	SDL_FRect textRect{ 0,0,wall_size,wall_size };
	for (const auto& row : tiles) {
		for (const auto& column : row) {
			if (column.x != 100) {
				textRect.x = dungeon_x + x * wall_size;
				textRect.y = dungeon_y + y * wall_size;
				SDL_RenderTexture(renderer, tileset, &column, &textRect);
			}
			//SDL_RenderFillRect(renderer, &textRect);
			x++;
		}
		x = 0;
		y++;
	}
}

void Dungeon::render() {
	int x = 0;
	int y = 0;
	render_tiles();
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
			if (column == 'S') {
				//monsters.resize(i + 1);
				//Skeleton monster(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.0025);
				monsters.push_back(make_unique<Skeleton>(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.005));
				monsters[i]->loadTexture(renderer, "images/skeleton.png");
				//monsters[i] = monster;
				i++;
			}
			if (column == 'M') {
				//monsters.resize(i + 1);
				//Minotaur monster(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.0025, this);
				monsters.push_back(make_unique<Minotaur>(x * wall_size + dungeon_x, y * wall_size + dungeon_y, player, 0.005, this));
				monsters[i]->loadTexture(renderer, "images/minotaur.png");
				//monsters[i] = monster;
				i++;
			}
			if (column == 'P') {
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
		if (input.type == SDL_EVENT_KEY_DOWN && !current_Key) {
			keyHandler->keyDown(key);
			current_Key = true;
		}
		else if (input.type == SDL_EVENT_KEY_UP && current_Key) {
			keyHandler->keyUp(key);
			current_Key = false;
		}
		// for testing purposes
		//player->change_direction(key);
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
	if (current_time - last_time >= 1500) {
		keyHandler->spawnKey();
		last_time = current_time;
	}
}