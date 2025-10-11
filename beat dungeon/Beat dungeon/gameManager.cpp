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
    for (auto& button : buttons) {
        button.render();
    }
    SDL_RenderPresent(renderer);
}

void GameManager::setUpHome() {
    screen = home;

    SDL_Surface* scaleSurface = IMG_Load("images/start.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_Texture* button_texture = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!level_screen) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    SDL_FRect rect{ 350,600,100,100 };

    buttons.resize(1);
    Button button(true, renderer, button_texture, rect);
    button.setFunc([&]() {setUpLevelSelect(); });
    buttons[0] = button;
}

void GameManager::setUpLevelSelect() {
    screen = levels;

    SDL_Surface* scaleSurface = IMG_Load("images/level.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_Texture* button_texture = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!level_screen) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    buttons.resize(9);
    int i = 1;
    for (auto& index : buttons) {
        div_t pos = div(i-1, 3);
        SDL_FRect rect{ 80 + 240*pos.rem,80+240*pos.quot,160,160 };
        Button button(true, renderer, button_texture, rect);
        button.setLevel([&](int i) {loadLevel(i); },i);
        index = button;
        i++;
    }
}

void GameManager::loadLevel(int level) {
    cout << "loadlevel";
    cout << level;
    cout << "loadedlevel";
}

void GameManager::handleInput(SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        for (auto& button : buttons) {
            button.checkHover(event.motion.x, event.motion.y);
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if(event.button.button == SDL_BUTTON_LEFT)
        for (auto& button : buttons) {
            if (button.getIsHover()) {
                button.onClick();
                break;
            }
        }
    }
}