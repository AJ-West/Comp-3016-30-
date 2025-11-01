#pragma once
#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>
//generate by microsoft copilot free

struct Particle {
    float x, y;       // Position
    float vx, vy;     // Velocity
    Uint8 r, g, b, a; // Color and alpha
    int lifetime;     // Remaining life in frames

    Particle(float px, float py, bool colour) {
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
        lifetime = 60 + rand() % 30; // 1–1.5 seconds at 60 FPS
    }

    bool update(float deltaTime) {
        x += vx ;
        y += vy ;
        lifetime--;
        if (lifetime < 0) return false;
        a = static_cast<Uint8>(255 * (lifetime / 100.0f)); // Fade out
        return true;
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_FRect rect = { static_cast<int>(x), static_cast<int>(y), 6, 6 };
        SDL_RenderFillRect(renderer, &rect);
    }
};