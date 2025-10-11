#include "gameManager.h"

GameManager::GameManager(SDL_Renderer* SDL_render, SDL_Window* SDL_window): renderer(SDL_render), window(SDL_window) {
    SDL_Surface* scaleSurface = IMG_Load("images/title screen.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    home_screen = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!home_screen) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    scaleSurface = IMG_Load("images/level screen.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    level_screen = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!level_screen) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}
GameManager::~GameManager(){}

void GameManager::render() {
    SDL_RenderClear(renderer);
    switch (screen) {
    case home:
        SDL_RenderTexture(renderer, home_screen, NULL, NULL);
        break;
    case levels:
        SDL_RenderTexture(renderer, level_screen, NULL, NULL);
        break;
    }
    SDL_RenderPresent(renderer);
}