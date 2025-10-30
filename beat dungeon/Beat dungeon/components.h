#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

#include "GameObject.h"

#include <SDL3/SDL.h>

//inspired by slides

class Component {
public:
	virtual void update() = 0;//to be implemented by derived components

	Component(GameObject* obj): owner(obj){};
	virtual ~Component() = default;
protected:
	GameObject* owner;
};

class TextureComponent : public Component {
public:
	virtual void update() {
		SDL_FRect size = owner->getDimensions();
		SDL_RenderTexture(renderer, texture, &location, &size);
	}
	
	TextureComponent(GameObject* obj, SDL_FRect Clocation, SDL_Renderer* Crenderer, SDL_Texture* Ctexture): Component(obj), location(Clocation), renderer(Crenderer), texture(Ctexture) {};
	virtual ~TextureComponent() {}
private:
	SDL_FRect location; // location of art in png
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};