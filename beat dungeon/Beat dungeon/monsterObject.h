#pragma once
#include <thread>
#include "particles.h"
#include "GameObject.h"

class MonsterObj : public GameObject {
public:
	MonsterObj(SDL_FRect dim, float sp, const vector<vector<int>>& walkable_outline, int size): GameObject(dim, sp, walkable_outline, size) {
		async = new thread(&MonsterObj::stun, this); // delay on spawn
	}
	
	void wallCollision(float deltaTime) {
		// Always respond to wall collisions by reducing speed and stopping movement.
		// If not currently stunned, also set stunned and start the stun timer.
		if (!stunned) {
			stunned = true;
			async = new thread(&MonsterObj::stun, this);
			async->detach();
		}
		// Reduce speed (tests expect speed to be reduced when colliding)
		setSpeed(getSpeed() / 2);
		charging = false;
		// Spawn particles (visual effect) - harmless for tests
		for (int i = 0; i < 25; i++) {
			particles.create(dimensions.x + dimensions.w / 2, dimensions.y + dimensions.h / 2, false);
		}
		pair<float, float> direction = getDirection();
		setDimensions({ dimensions.x - direction.first * deltaTime * getSpeed() * 2 , dimensions.y - direction.second * deltaTime * getSpeed() * 2, dimensions.w, dimensions.h});
		setDirection({ 0,0 });
	}

	void playerStun() {
		stunned = true;
		setDirection({ 0,0 });
		async = new thread(&MonsterObj::stun, this);
		async->detach();
	}

	void stun() {
		this_thread::sleep_for(chrono::seconds(3));
		stunned = false;
	}

	void updateParticles(float deltaTime, SDL_Renderer* renderer) {
		particles.update();
		particles.render(renderer);
	}

	//getters
	bool getStunned() { return stunned; }
	bool getHit() { return hit; }

	//setters
	void setCharging(bool charge) { charging = charge; }
	void setStunned(bool stun) { stunned = stun; }
	void setHit(bool hasHit) { hit = hasHit; }

private:
	bool stunned = true;
	bool hit = false;

	thread* async;

	ParticlePool particles;
};