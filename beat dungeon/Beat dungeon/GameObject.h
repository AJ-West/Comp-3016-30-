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

	template <typename T> Component(T* obj) : owner(obj) {};
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
	virtual void Update(float deltatime) {
		for (auto& pair : components) {
			pair.second->update(deltatime);
		}
	}

	pair<int, int> getCell() {
		pair<int, int> cell(div(dimensions.x + dimensions.w / 2, cell_size).quot, div(dimensions.y + dimensions.h / 2, cell_size).quot);
		return cell;
	}

	virtual void wallCollision(float deltatime) = 0;

	//getters
	SDL_FRect getDimensions() { return dimensions; }
	pair<float, float> getDirection() { return direction; }
	int getSpeed() { return speed; }
	vector<vector<int>> getWOutline() { return w_outline; }
	int getCellSize() { return cell_size; }
	pair<int, int> getRemoveCell() { return removeCell; }

	//setters
	//setters
	void setDimensions(SDL_FRect dim) { dimensions = dim; }
	void setDirection(pair<float, float> dir) { direction = dir; }
	void setSpeed(float sp) { speed = sp; }
	void setWalkableCell(int y, int x, int value) { w_outline[y][x] = value; }
	void setRemoveCell(int x, int y) { removeCell.first = x; removeCell.second = y; }


	//player function monsters dont have but required for GPP
	virtual void setWin(bool win) { cout << "win"; }
	virtual void newKeys() { cout << "newKeys"; }
	virtual void rotateKeys() { cout << "rotateKeys"; }

protected:
	SDL_FRect dimensions;

private:
	unordered_map<string, shared_ptr<Component>> components;// Store components

	pair<float, float> direction{0,0};
	float speed;
	vector<vector<int>> w_outline;

	int cell_size;
	pair<int, int> removeCell{ NULL,NULL };
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

class demoComponent : public Component {
public:
	virtual void update(float deltatime) {}

	demoComponent(GameObject* obj) : Component(obj) {}
	virtual ~demoComponent() {}
};