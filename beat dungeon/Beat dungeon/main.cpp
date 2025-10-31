// Beat dungeon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <irrKlang.h>
#include "gameManager.h"
#include "GameObject.h"
#include "components.h"
#include <memory>


using namespace std;
using namespace irrklang;

//screen size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
ISoundEngine* engine;
int FONT_SIZE = 30;
bool isRunning;

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
    window = SDL_CreateWindow("Beat dungeon", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
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

    engine = createIrrKlangDevice();
    if (!engine) {
        std::cerr << "Failed to load irrKlang DLL or initialize sound engine." << std::endl;
        return;
    }
    isRunning = true;
}

int main(int argc, char* argv[])
{
    system("pause");
    srand(time(0));
    init_environment();
    /*SDL_Surface* scaleSurface = IMG_Load("images/Knight.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    SDL_Texture* sprite = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!sprite) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }
    GameObject player({10,10,50,50});
    SDL_FRect size{ 6, 0, 20, 23 };
    player.AddComponent(make_shared<TextureComponent>(&player, size, renderer, sprite));

    //GPP testing
    while (isRunning) {
        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }
    }*/

    //Current Game
    
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
    
    engine->drop(); // clean up

    return 0;
}
