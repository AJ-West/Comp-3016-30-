#pragma once
#include "particles.h"
#include "minotaurObject.h"

class particleComponent : public Component {// renderers the object
public:
	virtual void update(float deltatime) {
		vector<Particle> particles = owner->getParticles();
		for (size_t i = 0; i < particles.size();) {
			if (!particles[i].update()) {
				particles.erase(particles.begin() + i);
			}
			else {
				i++;
			}
		}
	}

	particleComponent(MonsterObj* obj) : Component(obj), owner(obj) {};
	virtual ~particleComponent() {}
private:
	MonsterObj* owner;
};