// Beat dungeon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>


#include "dungeon.h"
#include "player.h"
#include "gameManager.h"
#include "KeyTime.h"

using namespace std;

//screen size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
int FONT_SIZE = 30;
bool isRunning;

//creates the window, renderer and font for the game
void init_environment() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    window = SDL_CreateWindow("Snake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
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

int main(int argc, char* argv[])
{
    system("pause");
    init_environment();
    GameManager manager(renderer, window);

    manager.setUpHome();
   
    while (isRunning) {
        manager.render();
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
        manager.update();
    }

    return 0;
}
