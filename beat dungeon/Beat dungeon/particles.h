#pragma once
#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>
//generate by microsoft copilot free
//https://gameprogrammingpatterns.com/object-pool.html used for help with object pooling

struct Particle {
    float x, y;       // Position
    float vx, vy;     // Velocity
    Uint8 r, g, b, a; // Color and alpha
    int lifetime;     // Remaining life in frames
    bool inUse = false;

    void init(float px, float py, bool colour) {
        x = px;
        y = py;
        // Random velocity
        vx = (rand() % 100 - 50) / 100.0f;
        vy = (rand() % 100 - 50) / 100.0f;
        // Random color
        if (colour) {
            r = rand() % 256;
            g = rand() % 256;
            b = rand() % 256;
        }
        else {
            r = 255;
            g = 255;
            b = 255;
        }
        a = 255;
        lifetime = 60 + rand() % 30; // 1�1.5 seconds at 60 FPS
        inUse = true;
    }

    void update() {
        if (inUse) {
            x += vx;
            y += vy;
            lifetime--;
            if (lifetime < 0) inUse = false;
            a = static_cast<Uint8>(255 * (lifetime / 100.0f)); // Fade out
        }
    }

    void render(SDL_Renderer* renderer) {
        if (inUse) {
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_FRect rect = { static_cast<int>(x), static_cast<int>(y), 6, 6 };
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    bool getInUse() { return inUse; }
};

class ParticlePool {
public:
    void create(float x, float y, bool colour) {
        for (int i = 0; i < POOL_SIZE; i++) {
            if (!particles[i].getInUse()) {
                particles[i].init(x, y, colour);
                return;
            }
        }
    }

    void update() {
        for (int i = 0; i < POOL_SIZE; i++) {
            particles[i].update();
        }
    }

    void render(SDL_Renderer* renderer) {
        for (int i = 0; i < POOL_SIZE; i++) {
            particles[i].render(renderer);
        }
        
    }
private:
    static const int POOL_SIZE = 150;
    Particle particles[POOL_SIZE];
};