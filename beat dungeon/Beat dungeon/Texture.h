#pragma once
#include "GameObject.h"

class TextureComponent : public Component {// renderers the object
public:
	virtual void update(float deltatime) {
		SDL_FRect size = owner->getDimensions();
		SDL_RenderTexture(renderer, texture, &location, &size);
	}

	void createTexture(const char* file) {
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
	}

	TextureComponent(GameObject* obj, SDL_FRect Clocation, SDL_Renderer* Crenderer, const char* file) : Component(obj), location(Clocation), renderer(Crenderer) {
		createTexture(file);
	};
	virtual ~TextureComponent() {}
private:
	SDL_FRect location; // location of art in png
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};