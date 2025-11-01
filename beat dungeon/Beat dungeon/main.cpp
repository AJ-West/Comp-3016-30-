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

    engine = createIrrKlangDevice();
    if (!engine) {
        std::cerr << "Failed to load irrKlang DLL or initialize sound engine." << std::endl;
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
    /*SDL_Surface* scaleSurface = IMG_Load("images/Knight.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    SDL_Texture* sprite = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    //SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!sprite) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    scaleSurface = IMG_Load("images/minotaur.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    SDL_Texture* msprite = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    //SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!sprite) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    scaleSurface = IMG_Load("images/skeleton.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    SDL_Texture* ssprite = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!sprite) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    vector<vector<int>> map{ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };
    int cell_size = 40;

    PlayerObj player({10,10,50,50},150, map, cell_size);
    SDL_FRect size{ 6, 0, 20, 23 };
    player.AddComponent(make_shared<TextureComponent>(&player, size, renderer, sprite));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    player.AddComponent(make_shared<movementComponent>(&player));// (&player, size, renderer, sprite)); // why is this causing LNK2019

    MonsterObj minotaur({ 500, 10, 80, 80 }, 150, map, cell_size);
    size.x = 6;
    size.y = 1;
    size.w = 20;
    size.h = 26;
    minotaur.AddComponent(make_shared<TextureComponent>(&minotaur, size, renderer, msprite));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    minotaur.AddComponent(make_shared<movementComponent>(&minotaur));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    minotaur.AddComponent(make_shared<pathfindingComponent>(&minotaur, &player));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    minotaur.AddComponent(make_shared<playerCollisionComponent>(&minotaur, &player, 80));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    minotaur.AddComponent(make_shared<chargeCollisionComponent>(&minotaur, &player, 800));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    

    MonsterObj skeleton({ 10, 500, 50, 50 }, 150, map, cell_size);
    size.x = 6;
    size.y = 0;
    size.w = 20;
    size.h = 32;
    skeleton.AddComponent(make_shared<TextureComponent>(&skeleton, size, renderer, ssprite));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    skeleton.AddComponent(make_shared<movementComponent>(&skeleton));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    skeleton.AddComponent(make_shared<pathfindingComponent>(&skeleton, &player));// (&player, size, renderer, sprite)); // why is this causing LNK2019
    skeleton.AddComponent(make_shared<playerCollisionComponent>(&skeleton, &player, 50));// (&player, size, renderer, sprite)); // why is this causing LNK2019

    Uint32 lastTime = SDL_GetTicks();

    //GPP testing
    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();
        SDL_RenderClear(renderer);
        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                SDL_Keycode key = event.key.key;
                switch (key) {
                case SDLK_W:
                    player.setDirection({0,-1});
                    break;
                case SDLK_A:
                    player.setDirection({ -1,0 });
                    break;
                case SDLK_S:
                    player.setDirection({ 0,1 });
                    break;
                case SDLK_D:
                    player.setDirection({ 1,0 });
                    break;
                }
            }
        }
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        player.Update(deltaTime);
        minotaur.Update(deltaTime);
        skeleton.Update(deltaTime);
        SDL_RenderPresent(renderer);

        // Frame rate capping
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < Frame_Delay) {
            SDL_Delay(Frame_Delay - frameTime);
        }
    }*/

    //Current Game
    
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
    
    engine->drop(); // clean up

    return 0;
}