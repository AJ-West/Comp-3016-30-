#include "player.h"

Player::Player(int start_x, int start_y, SDL_Renderer* SDL_renderer, Dungeon* dungeon): x(start_x), y(start_y), renderer(SDL_renderer), dung(dungeon) {
	// Sets up the default movement keys for the player
	movement_keys.resize(4, vector<SDL_Keycode>(1, 0));
	movement_keys[0][0] = SDLK_W;
	movement_keys[1][0] = SDLK_A;
	movement_keys[2][0] = SDLK_S;
	movement_keys[3][0] = SDLK_D;

	SDL_Surface* scaleSurface = IMG_Load("images/Knight.png");
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
Player::~Player() {}

void Player::change_direction(SDL_Keycode key, bool down) {
	if (down) {
		int index = 0;
		// checks which row the key is in
		// row 0 is up, row 1 is left, row 2 is down, row 3 is right
		for (const auto& row : movement_keys) {
			auto it = find(row.begin(), row.end(), key);
			if (it != row.end()) {
				break;
			}
			index++;
		}
		direction = index;
	}
	else {
		direction = 5;
	}
}

void Player::move() {
	// if the player is now inside a wall it undoes the move
	if (!checkCollision()) {
		// move the player in that direction
		switch (direction) {
		case 0:
			y -= speed;
			break;
		case 1:
			x -= speed;
			break;
		case 2:
			y += speed;
			break;
		case 3:
			x += speed;
			break;
		}
	}
	
	checkEnd();
}

void Player::undoMove() {
	switch (direction) {
	case 0:
		y += 20;
		break;
	case 1:
		x += 20;
		break;
	case 2:
		y -= 20;
		break;
	case 3:
		x -= 20;
		break;
	}
}

void Player::render() {
	// draw the player at its position
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_FRect character{ x, y, player_width, player_height };
	//SDL_RenderFillRect(renderer, &character);
	SDL_RenderTexture(renderer, sprite, &edge_remove, &character);
}

// check if the player is within a wall
bool Player::checkCollision() {
	float pot_x = x + player_width/2;
	float pot_y = y + player_height/2;

	switch (direction) {
	case 0:
		pot_y -= speed;
		break;
	case 1:
		pot_x -= speed;
		break;
	case 2:
		pot_y += speed;
		break;
	case 3:
		pot_x += speed;
		break;
	}

	pair<int, int> centre(div(pot_x -dung->getDungeonX(), dung->getWallSize()).quot, div(pot_y - dung->getDungeonY(), dung->getWallSize()).quot);

	if (find(wallTypes.begin(), wallTypes.end(), dung->getOutline()[centre.second][centre.first]) != wallTypes.end()) {
		return true;
	}
	return false;
}

// check if the player is at the end of the level
void Player::checkEnd() {
	// get the cell index for each corner of the player
	pair<int, int> x_bounds(div(x - dung->getDungeonX(), dung->getWallSize()).quot, div(x - dung->getDungeonX() + player_width - 1, dung->getWallSize()).quot);
	pair<int, int> y_bounds(div(y - dung->getDungeonY(), dung->getWallSize()).quot, div(y - dung->getDungeonY() + player_height - 1, dung->getWallSize()).quot);

	// check if any corners of the player is within an exit and completes the level if so
	if (dung->getOutline()[y_bounds.first][x_bounds.first] == '9') {
		levelComplete();
	}
	else if (dung->getOutline()[y_bounds.first][x_bounds.second] == '9') {
		levelComplete();
	}
	else if (dung->getOutline()[y_bounds.second][x_bounds.first] == '9') {
		levelComplete();
	}
	else if (dung->getOutline()[y_bounds.second][x_bounds.second] == '9') {
		levelComplete();
	}
}

void Player::levelComplete() {
	win = true;
}

//getters
pair<int, int> Player::getPos() {
	pair<int, int> pos = { x,y };
	return pos;
}

pair<int, int> Player::getDimensions() {
	pair<int, int> pos = { player_width, player_height };
	return pos;
}