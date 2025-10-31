#pragma once
#include <thread>
#include "particles.h"
#include "GameObject.h"

class MonsterObj : public GameObject {
public:
	MonsterObj(SDL_FRect dim, float sp, vector<vector<int>> walkable_outline, int size): GameObject(dim, sp, walkable_outline, size) {
		async = new thread(&MonsterObj::stun, this); // delay on spawn
	}
	
	void wallCollision(float deltatime) {
		stunned = true;
		setSpeed(getSpeed() / 2);
		charging = false;
		async = new thread(&MonsterObj::stun, this);
		async->detach();
		// Spawn multiple particles at mouse click
		for (int i = 0; i < 25; i++) { // ai
			particles.emplace_back(dimensions.x + dimensions.w / 2, dimensions.y + dimensions.h / 2, false);
		}
	}

	void stun() {
		this_thread::sleep_for(chrono::seconds(3));
		stunned = false;
	}

	//getters
	bool getCharging() { return charging; }
	bool getStunned() { return stunned; }
	vector<Particle> getParticles() { return particles; }

	//setters
	void setCharging(bool charge) { charging = charge; }
	void setStunned(bool stun) { stunned = stun; }

private:
	bool charging = false;
	bool stunned = true;

	thread* async;

	vector<Particle> particles;
};