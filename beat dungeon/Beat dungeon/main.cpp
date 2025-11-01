// Beat dungeon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <irrKlang.h>
#include "gameManager.h"
#include <memory>

using namespace std;
using namespace irrklang;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
ISoundEngine* engine;
int FONT_SIZE = 30;
bool isRunning;

//for consistency (from ai)
const int FPS = 60;
const int Frame_Delay = 1000 / FPS; // milliseconds per frame

//creates the window, renderer and font for the game
void init_environment() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cerr << "SDL could not initialize: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    window = SDL_CreateWindow("Beat dungeon", screen_width, screen_height, 0);
    if (window == nullptr) {
        cerr << "Window cannot be created: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        cerr << "Renderer cannot be created: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    font = TTF_OpenFont("PixelEmulator-xq08.ttf", FONT_SIZE);
    if (font == nullptr) {
        cerr << "Font cannot be loaded: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    isRunning = true;
}

//inspired by slides

int main(int argc, char* argv[])
{
    system("pause");
    srand(time(0));
    init_environment();
        
    GameManager manager(renderer, window);

    manager.setUpHome();

    Uint32 lastTime = SDL_GetTicks();
   
    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();
        //manager.render();
        
        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else {
                manager.handleInput(event);
            }
        }
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        manager.update(deltaTime);

        // Frame rate capping
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < Frame_Delay) {
            SDL_Delay(Frame_Delay - frameTime);
        }
    }
    

    return 0;
}