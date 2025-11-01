#pragma once
#include "particles.h"
#include "minotaurObject.h"

class particleComponent : public Component {// renderers the object
public:
	virtual void update(float deltatime) {
		vector<Particle> particles = owner->getParticles();
		for (size_t i = 0; i < particles.size();) {
			if (!particles[i].update(deltatime)) {
				particles.erase(particles.begin() + i);
			}
			else {
				particles[i].render(renderer);
				i++;
			}
		}
	}

	particleComponent(MonsterObj* obj, SDL_Renderer* render) : Component(obj), owner(obj), renderer(render){};
	virtual ~particleComponent() {}
private:
	MonsterObj* owner;
	SDL_Renderer* renderer;
};