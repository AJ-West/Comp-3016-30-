#pragma once
#include "GameObject.h"

class TextureComponent : public Component {// renderers the object
public:
	virtual void update(float deltatime) {
		SDL_FRect size = owner->getDimensions();
		SDL_RenderTexture(renderer, texture, &location, &size);
	}

	TextureComponent(GameObject* obj, SDL_FRect Clocation, SDL_Renderer* Crenderer, SDL_Texture* Ctexture) : Component(obj), location(Clocation), renderer(Crenderer), texture(Ctexture) {};
	virtual ~TextureComponent() {}
private:
	SDL_FRect location; // location of art in png
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};