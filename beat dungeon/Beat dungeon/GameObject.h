#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

#include <SDL3/SDL.h>

using namespace std;

class Component;

//inspired by slides

//GameObject class that holds multiple components
class GameObject {
public:
	GameObject(SDL_FRect dim):  dimensions(dim) {}

	//add a component to the GameObject
	template <typename T> void AddComponent(shared_ptr<T> component);
	
	//get a component from GameObject
	template <typename T> shared_ptr<T> AddComponent();

	//update all components
	void Update();

	//getters
	SDL_FRect getDimensions() { return dimensions; }

private:
	unordered_map<string, shared_ptr<Component>> components;// Store components

	SDL_FRect dimensions;
};