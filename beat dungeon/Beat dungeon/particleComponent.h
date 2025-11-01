#pragma once
#include "particles.h"
#include "minotaurObject.h"

class particleComponent : public Component {// renderers the object
public:
	virtual void update(float deltaTime) { // update all particles
		owner->updateParticles(deltaTime, renderer);
	}

	particleComponent(MonsterObj* obj, SDL_Renderer* render) : Component(obj), owner(obj), renderer(render){};
	virtual ~particleComponent() {}
private:
	MonsterObj* owner;
	SDL_Renderer* renderer;
};