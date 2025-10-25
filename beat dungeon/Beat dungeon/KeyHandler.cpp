#include "KeyHandler.h"

KeyHandler::KeyHandler(SDL_Renderer* SDL_renderer, Player* play): renderer(SDL_renderer), player(play) {
	//top row
	locations.push_back(make_pair(0, 0));
	locations.push_back(make_pair(100, 0));
	locations.push_back(make_pair(200, 0));
	locations.push_back(make_pair(300, 0));
	locations.push_back(make_pair(400, 0));
	locations.push_back(make_pair(500, 0));
	locations.push_back(make_pair(600, 0));
	locations.push_back(make_pair(700, 0));

	//left column
	locations.push_back(make_pair(0, 100));
	locations.push_back(make_pair(0, 200));
	locations.push_back(make_pair(0, 300));
	locations.push_back(make_pair(0, 400));
	locations.push_back(make_pair(0, 500));
	locations.push_back(make_pair(0, 600));
	locations.push_back(make_pair(0, 700));

	//right column
	locations.push_back(make_pair(700, 100));
	locations.push_back(make_pair(700, 200));
	locations.push_back(make_pair(700, 300));
	locations.push_back(make_pair(700, 400));
	locations.push_back(make_pair(700, 500));
	locations.push_back(make_pair(700, 600));
	locations.push_back(make_pair(700, 700));

	all_keys.resize(22, nullptr);

	//create textures
	SDL_Surface* scaleSurface = IMG_Load("images/outline.png");
	if (!scaleSurface) {
		std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
		return;
	}

	key_outline = SDL_CreateTextureFromSurface(renderer, scaleSurface);
	SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
	if (!key_outline) {
		std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	scaleSurface = IMG_Load("images/badoutline.png");
	if (!scaleSurface) {
		std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
		return;
	}

	key_bad_outline = SDL_CreateTextureFromSurface(renderer, scaleSurface);
	SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
	if (!key_bad_outline) {
		std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	scaleSurface = IMG_Load("images/progress dot.png");
	if (!scaleSurface) {
		std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
		return;
	}

	key_dot = SDL_CreateTextureFromSurface(renderer, scaleSurface);
	SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
	if (!key_dot) {
		std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	font = TTF_OpenFont("PixelEmulator-xq08.ttf", 100);
	if (font == nullptr) {
		cerr << "Font cannot be loaded: TTF_ERROR" << SDL_GetError() << endl;
		return;
	}
}
KeyHandler::~KeyHandler(){}

void KeyHandler::spawnKey(){
	bool unique = false;
	int pos = rand() % 22;
	while (!unique) {
		if (all_keys[pos] != nullptr) {
			pos = rand() % 22;
		}
		else {
			unique = true;
		}
	}

	vector<vector<SDL_Keycode>> options = player->getMovementKeys();
	int random_direction = rand() % 5;
	bool good = false;
	SDL_Keycode code = keyboard[rand() % size(keyboard)];
	if (random_direction != 4) {
		code = options[random_direction][rand() % size(options[random_direction])];
		good = true;
	}
	
	char key_text = static_cast<char>(code);
	if (good) {
		SDL_Surface* surface = TTF_RenderText_Solid(font, &key_text, 1, { 255,255,255,0 });
		text = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);
		all_keys[pos] = new KeyTime(code, text, true, key_outline, locations[pos]);
	}
	else {
		SDL_Surface* surface = TTF_RenderText_Solid(font, &key_text, 1, { 255,0,0,0 });
		text = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);
		all_keys[pos] = new KeyTime(code, text, false, key_bad_outline, locations[pos]);
	}
}

void KeyHandler::renderKeys() {
	for (auto& key : all_keys) {
		if(key != nullptr)
			key->render(key_dot, renderer);
	}
}

void KeyHandler::checkTimes() {
	for (auto& key : all_keys) {
		if (key != nullptr) {
			if ((key->time_elapsed() || key->getUsed()) && key->getGood()) {
				if (key->getIsDown()) {
					player->change_direction(key->getKey(), false);
				}
				delete key;
				key = nullptr;
			}
			else if (key->time_elapsed() && !key->getGood()) {
				cout << "level failed";
				delete key;
				key = nullptr;
			}
			else if (key->getUsed() && !key->getGood()) {
				delete key;
				key = nullptr;
			}
		}
	}
}

void KeyHandler::keyDown(SDL_Keycode key) {
	//code for game
	for (auto& keyT : all_keys) {
		if (keyT != nullptr) {
			if (keyT->getKey() == key) {
				if (keyT->inZone()) {
					player->change_direction(key, true);
					keyT->setIsDown(true);
					break;
				}
			}
		}
	}
}

void KeyHandler::keyUp(SDL_Keycode key) {
	for (auto& keyT : all_keys) {
		if (keyT != nullptr) {
			if (keyT->getKey() == key) {
				if (keyT->inZone()) {
					player->change_direction(key, false);
					keyT->setIsDown(false);
					keyT->setUsed(true);
					break;
				}
			}
		}
	}
}