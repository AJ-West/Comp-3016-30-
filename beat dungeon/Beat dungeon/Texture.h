#pragma once
#include "GameObject.h"

class TextureComponent : public Component {// renderers the object
public:
	virtual void update(float deltatime) { // render the current frame
		SDL_FRect size = owner->getDimensions();
		SDL_RenderTexture(renderer, texture, &frames[current_animation][current_frame], &size);
		check_animation();
		updateTime();
	}

	void updateTime() { // update duration of how long left on current frame
		time_left--;
		if (time_left <= 0) {
			time_left = time_per_frame;
			current_frame++;
			if (current_frame == frames[current_animation].size()) { current_frame = 0; }
		}
	}

	void check_animation() { // checks if animation should change
		pair<int, int> own_dir = owner->getDirection();
		if (owner->getCharging()) { // charge animation and direction
			if (current_animation != 2) {
				current_animation = 2;
				current_frame = 0;
			}
			if (dir != own_dir.first) {
				dir = own_dir.first;
				setDirection();
			}
		}
		else {
			if (owner->getDirection() != pair<int,int>{0,0}) { // walk animation and direction
				if (current_animation != 1) {
					current_animation = 1;
					current_frame = 0;
				}
				if (dir != own_dir.first) {
					dir = own_dir.first;
					setDirection();
				}
			}
			else if (current_animation != 0) { // idle animation
				current_animation = 0;
				current_frame = 0;
			}
		}
	}

	void setDirection() { // flips the sprite to face the correct direction
		if (owner->getDirection().first == 1) {
			SDL_FRect size = owner->getDimensions();
			if (size.w < 0) {
				size.x = size.x + size.w;
				size.w = -size.w;
				owner->setDimensions(size);
			}
		}
		else if (owner->getDirection().first == -1) {
			SDL_FRect size = owner->getDimensions();
			if (size.w > 0) {
				size.x = size.x + size.w;
				size.w = -size.w;
				owner->setDimensions(size);
			}
		}
	}

	void createTexture(const char* file) { // loads the sprite sheet on creation
		SDL_Surface* scaleSurface = IMG_Load(file);
		if (!scaleSurface) {
			std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
		}

		texture = SDL_CreateTextureFromSurface(renderer, scaleSurface);
		SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
		if (!texture) {
			std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
		}
		vector<SDL_FRect> animation_Type;
		//hard coded create idle, walk, charge
		for (int i = 0; i < all_frames; i++) { // saves the locations of each animation frame
			if (i == 2||i == 6) {
				frames.emplace_back(animation_Type);
				animation_Type.clear();
				if (i == 6) {
					location.x = 10;
					location.w = 22;
				}
			}
			SDL_FRect loc{ location.x + 32 * i, location.y, location.w, location.h };
			animation_Type.emplace_back(loc);
		}
		frames.emplace_back(animation_Type);
	}
	

	TextureComponent(GameObject* obj, SDL_FRect Clocation, SDL_Renderer* Crenderer, const char* file, int frames) : Component(obj), location(Clocation), renderer(Crenderer), all_frames(frames) {
		createTexture(file);
		time_left = time_per_frame;
	};
	virtual ~TextureComponent() {}
private:
	SDL_FRect location; // location of art in png
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	int time_per_frame = 0.5 * 60; //seconds*fps
	int time_left;

	int current_animation = 0;

	int all_frames; // total frames across all animations
	int current_frame = 0;

	vector<vector<SDL_FRect>> frames;

	int dir = 0; // which way sprite is facing
};