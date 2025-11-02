#include "dungeon.h"
#include "tilemap.h"

Dungeon::Dungeon(SDL_Renderer* sdlRenderer, int levelNumber) : renderer(sdlRenderer), levelNum(levelNumber) {
	read_file();
	keyHandler = new KeyHandler(renderer, player);
	SDL_Surface* scaleSurface = IMG_Load("images/walls.png");
	if (!scaleSurface) {
		cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return;
	}

	tileset = SDL_CreateTextureFromSurface(renderer, scaleSurface);
	SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
	if (!tileset) {
		cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return;
	}
}

Dungeon::~Dungeon() {}

void Dungeon::read_file() {
	string fileName = "levels/level" + to_string(levelNum) + ".txt";
	//retrieve text file for the sppecified level

	ifstream file(fileName, ios::binary);
	if (!file.is_open()) {
		cerr << "Cannot open file";
		return;
	}
	string encoded((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();
	string decoded = base64_decode(encoded);

	//for retrieving the data char at a time
	//char ch;
	int line = 0;
	int col = 0;
	vector<char> row;
	//get next char
	for (char ch : decoded) {
		//if end of line got to start of next
		if (ch == '\n') {
			outline.push_back(row);
			row.clear();
		}
		else if (ch != ' ') {
			row.push_back(ch);
		}
	}
	outline.push_back(row);
	createPWalkableOutline();
	spawn_player();
	save_tiles();
	createMWalkableOutline();
	spawn_monsters();
}

string Dungeon::base64_decode(const string& input) { // ai copilot free
	static const string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	vector<int> decoding_table(256, -1);
	for (int i = 0; i < 64; ++i) decoding_table[table[i]] = i;

	string output;
	int val = 0, valb = -8;
	for (unsigned char c : input) {
		if (decoding_table[c] == -1) break;
		val = (val << 6) + decoding_table[c];
		valb += 6;
		if (valb >= 0) {
			output.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return output;
}

void Dungeon::save_tiles() {
	Tilemap tilemap(walkable_outline[0].size(), walkable_outline.size(), walkable_outline);
	tiles = tilemap.identifyWalls();
	//cout << "temp";
}

void Dungeon::createPWalkableOutline() {
	vector<int> tile_row;
	for (const auto& row : outline) {
		for (const auto& column : row) {
			if (column == 'P' || column == 'M' || column == 'S') {
				tile_row.push_back(0);
			}
			else {
				tile_row.push_back(column - '0');
			}
		}
		walkable_outline.push_back(tile_row);
		tile_row.clear();
	}
}

void Dungeon::createMWalkableOutline() {
	for (auto& row : walkable_outline) {
		for (auto& column : row) {
			if (column != 1) {
				column = 0;
			}
		}
	}
}

void Dungeon::render_tiles() {
	int x = 0;
	int y = 0;
	SDL_FRect textRect{ 0,0,wall_size,wall_size };
	for (const auto& row : tiles) {
		for (const auto& column : row) {
			textRect.x = dungeon_x + x * wall_size;
			textRect.y = dungeon_y + y * wall_size;
			SDL_RenderTexture(renderer, tileset, &column, &textRect);
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
	keyHandler->renderKeys();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Dungeon::updateMonsters(float deltaTime) {
	for (const auto& monst : monsters) {
		monst->Update(deltaTime);
		if (monst->getHit()) {
			restart = true;
		}
	}
}

void Dungeon::spawn_player() {
	int x = 0;
	int y = 0;
	for (const auto& row : outline) {
		x = 0;
		for (const auto& column : row) {
			if (column == 'P') {
				SDL_FRect start{ x * wall_size, y * wall_size,50,50 };
				player = new PlayerObj(start, 150, walkable_outline, wall_size); //draw size, speed, map, cell size
				SDL_FRect size{ 6, 0, 20, 23 };
				player->AddComponent(make_shared<TextureComponent>(player, size, renderer, "images/KnightSS.png", 6));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				player->AddComponent(make_shared<movementComponent>(player));
				player->AddComponent(make_shared<environmentCollisionComponent>(player));
				break;
			}
			x++;
		}
		y++;
	}
}

void Dungeon::spawn_monsters() {
	int i = 0;
	int x = 0;
	int y = 0;
	for (const auto& row : outline) {
		x = 0;
		for (const auto& column : row) {
			if (column == 'S') {
				SDL_FRect d_size{ x * wall_size, y*wall_size, 50, 50 }; // size for sprite to be draw as
				monsters.push_back(new MonsterObj(d_size, 25, walkable_outline, wall_size));//draw size, speed, map, cell size
				SDL_FRect s_size{ 6,0,20,32 }; // size of sprite in png
				monsters[i]->AddComponent(make_shared<TextureComponent>(monsters[i], s_size, renderer, "images/skeletonSS.png", 6));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<movementComponent>(monsters[i]));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<pathfindingComponent>(monsters[i], player));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<playerCollisionComponent>(monsters[i], player, 20));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				i++;
			}
			if (column == 'M') {
				SDL_FRect d_size{ x * wall_size, y * wall_size, 50, 50 }; // size for sprite to be draw as
				monsters.push_back(new MonsterObj(d_size, 150, walkable_outline, wall_size));//draw size, speed, map, cell size
				SDL_FRect s_size{ 6,0,20,32 }; // size of sprite in png
				monsters[i]->AddComponent(make_shared<TextureComponent>(monsters[i], s_size, renderer, "images/minotaurSS.png",10));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<movementComponent>(monsters[i]));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<pathfindingComponent>(monsters[i], player));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<playerCollisionComponent>(monsters[i], player, 20));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<chargeCollisionComponent>(monsters[i], player, 800));// (&player, size, renderer, sprite)); // why is this causing LNK2019
				monsters[i]->AddComponent(make_shared<environmentCollisionComponent>(monsters[i]));
				monsters[i]->AddComponent(make_shared<particleComponent>(monsters[i], renderer));
				i++;
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
		//player->change_direction(key, true);
	}
}

void stunDelay(MonsterObj* monst) {
	this_thread::sleep_for(chrono::seconds(3));
	monst->setStunned(false);
};

bool Dungeon::update(float deltaTime) {
	spawn_key();
	//removes used keys or timed out keys
	int index = 0;
	vector<int> to_delete;
	keyHandler->checkTimes();
	keyHandler->updateParticles(deltaTime);
	if (keyHandler->getStun()) {
		for (const auto& monst : monsters) {
			monst->playerStun();
		}
		keyHandler->setStun(false);
	}
	if (keyHandler->getFailed()) {restart = true;}
	updateMonsters(deltaTime);
	player->Update(deltaTime);
	pair<int, int> cell = player->getRemoveCell();
	if (cell != pair<int, int>{NULL, NULL}) {
		setDungeonTile(cell.first, cell.second, { 300,300,32,32 });
		player->setRemoveCell(NULL, NULL);
	}
	if (player->getWin()) {
		cout << "level complete";
		complete = true;
		return true;
	}
	if (restart) {
		return true;
	}
	return false;
}

void Dungeon::spawn_key() {
	double current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();;
	if (current_time - last_time >= 1500) {
		keyHandler->spawnKey();
		last_time = current_time;
	}
}