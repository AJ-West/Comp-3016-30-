#include "KeyHandler.h"

KeyHandler::KeyHandler(SDL_Renderer* SDL_renderer, PlayerObj* play): renderer(SDL_renderer), player(play) {
	//top row
	locations.push_back(make_pair(0, 0));
	locations.push_back(make_pair(screen_width/12, 0));
	locations.push_back(make_pair(screen_width / 12*9, 0));
	locations.push_back(make_pair(screen_width / 12*10, 0));

	//left column
	locations.push_back(make_pair(0, screen_height / 12 ));
	locations.push_back(make_pair(0, screen_height / 12 * 2));
	locations.push_back(make_pair(0, screen_height / 12 * 3));
	locations.push_back(make_pair(0, screen_height / 12 * 4));
	locations.push_back(make_pair(0, screen_height / 12 * 5));
	locations.push_back(make_pair(0, screen_height / 12 * 6));
	locations.push_back(make_pair(0, screen_height / 12 * 7));

	//right column
	locations.push_back(make_pair(screen_width / 8*8, screen_height/12));
	locations.push_back(make_pair(screen_width / 12 * 11, screen_height / 12*2));
	locations.push_back(make_pair(screen_width / 12 * 11, screen_height / 12*3));
	locations.push_back(make_pair(screen_width / 12 * 11, screen_height / 12*4));
	locations.push_back(make_pair(screen_width / 12 * 11, screen_height / 12*5));
	locations.push_back(make_pair(screen_width / 12 * 11, screen_height / 12*6));
	locations.push_back(make_pair(screen_width / 12 * 11, screen_height / 12*7));

	bad_keys.resize(18, nullptr);

	good_locations.push_back(make_pair(screen_width / 8*2, 0));
	good_locations.push_back(make_pair(screen_width / 8*3, 0));
	good_locations.push_back(make_pair(screen_width / 8*4, 0));
	good_locations.push_back(make_pair(screen_width / 8*5, 0));

	createTextures();
}
KeyHandler::~KeyHandler(){
	sound->drop();
}

void KeyHandler::createTextures() {
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

	sound = createIrrKlangDevice();
	if (!sound) {
		std::cerr << "Failed to load irrKlang DLL or initialize sound engine." << std::endl;
		return;
	}
}

void KeyHandler::spawnKey(){
	bool unique = false;
	int pos = rand() % 18;
	while (!unique) {
		if (bad_keys[pos] != nullptr) {
			pos = rand() % 18;
		}
		else {
			unique = true;
		}
	}

	vector<vector<SDL_Keycode>> options = player->getMovementKeys();
	for (int i = 0; i < good_keys.size(); i++) {
		if (good_keys[i] == nullptr) {
			SDL_Keycode code = options[i][rand() % size(options[i])];
			char key_text = static_cast<char>(code);
			SDL_Surface* surface;
			switch (code) {
			case SDLK_UP:
				surface = IMG_Load("images/arrows/up.png");
				break;
			case SDLK_LEFT:
				surface = IMG_Load("images/arrows/left.png");
				break;
			case SDLK_DOWN:
				surface = IMG_Load("images/arrows/down.png");
				break;
			case SDLK_RIGHT:
				surface = IMG_Load("images/arrows/right.png");
				break;
			default:
				surface = TTF_RenderText_Solid(font, &key_text, 1, { 255,255,255,0 });
			}
			text = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_DestroySurface(surface);
			good_keys[i] = new KeyTime(code, text, true, key_outline, good_locations[i]);
		}
	}
	
	int random_direction = rand() % 5;
	if (random_direction == 1) {
		vector<vector<SDL_Keycode>> options = player->getMovementKeys();
		bool good = false;
		SDL_Keycode code = keyboard[rand() % size(keyboard)];
		char key_text = static_cast<char>(code);
		SDL_Surface* surface;
		switch (code) {
		case SDLK_UP:
			surface = IMG_Load("images/arrows/redup.png");
			break;
		case SDLK_LEFT:
			surface = IMG_Load("images/arrows/redleft.png");
			break;
		case SDLK_DOWN:
			surface = IMG_Load("images/arrows/redown.png");
			break;
		case SDLK_RIGHT:
			surface = IMG_Load("images/arrows/redright.png");
			break;
		default:
			surface = TTF_RenderText_Solid(font, &key_text, 1, { 255,0,0,0 });
		}
		if (!surface) {
			std::cerr << "Unable to load surface! IMG_Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return;
		}
		text = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);
		bad_keys[pos] = new KeyTime(code, text, false, key_bad_outline, locations[pos]);
	}
}

void KeyHandler::spawnGoodKey(int index) {
	vector<vector<SDL_Keycode>> options = player->getMovementKeys();	
	SDL_Keycode code = options[index][rand() % size(options[index])];
	char key_text = static_cast<char>(code);
	SDL_Surface* surface;
	switch (code) {
	case SDLK_UP:
		surface = IMG_Load("images/arrows/up.png");
		break;
	case SDLK_LEFT:
		surface = IMG_Load("images/arrows/left.png");
		break;
	case SDLK_DOWN:
		surface = IMG_Load("images/arrows/down.png");
		break;
	case SDLK_RIGHT:
		surface = IMG_Load("images/arrows/right.png");
		break;
	default:
		surface = TTF_RenderText_Solid(font, &key_text, 1, { 255,255,255,0 });
	}
	text = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);
	good_keys[index] = new KeyTime(code, text, true, key_outline, good_locations[index]);
}

void KeyHandler::renderKeys() {
	for (auto& key : bad_keys) {
		if(key != nullptr)
			key->render(key_dot, renderer);
	}
	for (auto& key : good_keys) {
		if (key != nullptr)
			key->render(key_dot, renderer);
	}
	particles.render(renderer);
}

void KeyHandler::checkTimes() { // see if any keys need removing
	int index = 0;
	for (auto& key : good_keys) {
		if (key != nullptr) {
			if ((key->time_elapsed() || key->getUsed())) { // if good key has been pressed or ran out
				if (key->getIsDown()) { // if still pressing a key as it ran out act as though let go og good key
					player->change_direction(key->getKey(), false);
					int rand = std::rand() % 5;
					sound->play2D(key_sounds[rand], false);
					for (int i = 0; i < 25; i++) { // ai
						particles.create(good_locations[index].first+ 40, good_locations[index].second+ 40, true);
					}
				}
				delete key;
				key = nullptr;
				spawnGoodKey(index);
			}
		}
		index++;
	}
	index = 0;
	for (auto& key : bad_keys) {
		if (key != nullptr) {
			if (key->time_elapsed()) { // if bad key has ran out fail level
				sound->play2D("sound effects/missed.wav", false);
				failed = true;
				delete key;
				key = nullptr;
			}
			else if (key->getUsed()) { // if pressed bad key before it ran out
				stun = true;
				delete key;
				key = nullptr;
			}
		}
		index++;
	}
}

void KeyHandler::updateParticles(float deltaTime) {//ai from free copilot
	particles.update();
}

void KeyHandler::keyDown(SDL_Keycode key) {
	for (auto& keyT : good_keys) {
		if (keyT != nullptr) {
			if (keyT->getKey() == key) { // if key matches held down key
				if (keyT->inZone()) { // if in correct zone
					player->change_direction(key, true);
					keyT->setIsDown(true);
					break;
				}
			}
		}
	}
	for (auto& keyT : bad_keys) {
		if (keyT != nullptr) {
			if (keyT->getKey() == key) { // if key matches held down key
				if (keyT->inZone()) { // if in correct zone
					keyT->setIsDown(true);
					break;
				}
			}
		}
	}
}

void KeyHandler::keyUp(SDL_Keycode key) { // on release of key
	int index = 0;
	for (auto& keyT : good_keys) {
		if (keyT != nullptr) {
			if (keyT->getKey() == key) { // if key just released
				if (keyT->inZone()) { // if in correct zone
					player->change_direction(key, false);
					keyT->setIsDown(false);
					keyT->setUsed(true);
					int rand = std::rand() % 5;
					sound->play2D(key_sounds[rand], false);
					// Spawn multiple particles at mouse click
					for (int i = 0; i < 25; i++) { // ai
						particles.create(good_locations[index].first+40, good_locations[index].second+40, true);
					}
					break;
				}
			}
		}
		index++;
	}
	index = 0;
	for (auto& keyT : bad_keys) {
		if (keyT != nullptr) {
			if (keyT->getKey() == key) { // if key just released
				if (keyT->inZone()) { // if in correct zone
					keyT->setIsDown(false);
					keyT->setUsed(true);
					int rand = std::rand() % 5;
					sound->play2D(key_sounds[rand], false);
					// Spawn multiple particles at mouse click
					for (int i = 0; i < 25; i++) { // ai
						particles.create(locations[index].first + 40, locations[index].second + 40, true);
					}
					break;
				}
			}
		}
		index++;
	}
}