// Beat dungeon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>


#include "dungeon.h"
#include "player.h"
#include "monster.h"

using namespace std;

//screen size
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

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
    font = TTF_OpenFont("Freedom-10eM.ttf", FONT_SIZE);
    if (font == nullptr) {
        cerr << "Font cannot be loaded: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    isRunning = true;
}

//renderer the game each frame 
void render(Dungeon dung, Player player, Monster monster) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    dung.render();
    player.render();
    monster.render();
    SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[])
{
    init_environment();
    //temp initiation of dungeon and player
    Dungeon dung(10, 10, renderer);
    Player player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, renderer);
    //Monster(int start_x, int start_y, SDL_Renderer* SDL_renderer, Player* player_point, int s_speed):
    Monster monster(0, 0, renderer, &player, 0.025);
    //reads the test level
    dung.read_file("levels/level1.txt");
    while (isRunning) {
        render(dung, player, monster);
        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_EVENT_KEY_UP) {
                player.handle_input(event);
            }
        }
        monster.move();
    }

    return 0;
}
