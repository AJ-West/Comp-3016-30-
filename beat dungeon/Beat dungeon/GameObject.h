#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <queue>
#include <cmath>
#include <unordered_map>

#include <SDL3/SDL.h>


using namespace std;

class GameObject;

//GameObject class that holds multiple components

class Component {
public:
	virtual void update(float deltatime) = 0;//to be implemented by derived components

	Component(GameObject* obj) : owner(obj) {};
	virtual ~Component() = default;
protected:
	GameObject* owner;
};

class GameObject {
public:
	GameObject(SDL_FRect dim, float sp, vector<vector<int>> walkable_outline, int size) : dimensions(dim), speed(sp), w_outline(walkable_outline), cell_size(size) {}

	//add a component to the GameObject
	template <typename T> void AddComponent(shared_ptr<T> component) {
		components[typeid(T).name()] = component;
	}

	//get a component from GameObject
	template <typename T> shared_ptr<T> AddComponent() {
		return static_pointer_cast<T>(components[typeid(T).name()]);
	}

	//update all components
	void Update(float deltatime) {
		for (auto& pair : components) {
			pair.second->update(deltatime);
		}
	}

	pair<int, int> getCell() {
		pair<int, int> cell(div(dimensions.x + dimensions.w / 2, cell_size).quot, div(dimensions.y + dimensions.h / 2, cell_size).quot);
		return cell;
	}

	//getters
	SDL_FRect getDimensions() { return dimensions; }
	pair<int, int> getDirection() { return direction; }
	int getSpeed() { return speed; }
	vector<vector<int>> getWOutline() { return w_outline; }

	//setters
	//setters
	void setDimensions(SDL_FRect dim) { dimensions = dim; }
	void setDirection(pair<int, int> dir) { direction = dir; }
	void setSpeed(float sp) { speed = sp; }

private:
	unordered_map<string, shared_ptr<Component>> components;// Store components

	SDL_FRect dimensions;
	pair<int, int> direction{0,0};
	float speed;
	vector<vector<int>> w_outline;

	int cell_size;
};

class movementComponent : public Component {
public:
	virtual void update(float deltatime){
		SDL_FRect dimensions = owner->getDimensions();
		pair<int, int> direction = owner->getDirection();
		float speed = owner->getSpeed() * deltatime;
		owner->setDimensions({ dimensions.x + direction.first*speed, dimensions.y + direction.second * speed, dimensions.w, dimensions.h });
	}

	movementComponent(GameObject* obj) : Component(obj) {}
	virtual ~movementComponent(){}
};

class animationComponent : public Component {
public:
	virtual void update(Uint32 deltatime) {}

	animationComponent(GameObject* obj) : Component(obj) {}
	virtual ~animationComponent() {}
};

class attackComponent : public Component {
public:
	virtual void update(float deltatime) {}

	attackComponent(GameObject* obj) : Component(obj) {}
	virtual ~attackComponent() {}
};
/*
class wallCollisionComponent : public Component {
public:
	virtual void update(float deltatime) {
		pair<int, int> centre(div(x + width / 2 - dung->getDungeonX(), dung->getWallSize()).quot, div(y + height / 2 - dung->getDungeonY(), dung->getWallSize()).quot);

		if (w_outline[centre.second][centre.first] == 1) {
			crash();
		}
	}

	collisionComponent(GameObject* obj) : Component(obj) {}
	virtual ~collisionComponent() {}
};*/

class chargeCollisionComponent : public Component {
public:
	virtual void update(float deltatime) {
		if (!stunned && !charging) {
			if (checkPlayerCollision(player->getDimensions())) {
				charging = true;
				owner->setSpeed(owner->getSpeed() * 2);
			}
		}
		else {

		}
	}

	bool checkPlayerCollision(SDL_FRect player_dim) {
		SDL_FRect dim = owner->getDimensions();
		int x_dist = dim.x - player_dim.x;
		int y_dist = dim.y - player_dim.y;
		if (x_dist * x_dist + y_dist * y_dist < range * range) {
			return true;
		}
		return false;
	}

	chargeCollisionComponent(GameObject* obj, GameObject* play, int attackRange) : Component(obj), player(play), range(attackRange) {}
	virtual ~chargeCollisionComponent() {}
private:
	GameObject* player;
	int range;
	bool stunned = false;
	bool charging = false;
};

class demoComponent : public Component {
public:
	virtual void update(float deltatime) {}

	demoComponent(GameObject* obj) : Component(obj) {}
	virtual ~demoComponent() {}
};