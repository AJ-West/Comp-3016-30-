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

    scaleSurface = IMG_Load("images/pause.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    pause_screen = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!pause_screen) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    music = createIrrKlangDevice();
    if (!music) {
        std::cerr << "Failed to load irrKlang DLL or initialize sound engine." << std::endl;
        return;
    }
    //music->play2D("background track.mp3", true); // looped playback
    //music->setSoundVolume(0.8f);
}
GameManager::~GameManager(){}

void GameManager::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    switch (screen) {
    case home:
        SDL_RenderTexture(renderer, home_screen, NULL, NULL);
        break;
    case levels:
        SDL_RenderTexture(renderer, level_screen, NULL, NULL);
        break;
    case inLevel:
        dung->render();
        break;
    case pauseLevel:
        SDL_RenderTexture(renderer, pause_screen, NULL, NULL);
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
    if (!button_texture) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    buttons.resize(getUnlockedLevels());
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

void GameManager::levelPaused() {
    screen = pauseLevel;

    SDL_Surface* scaleSurface;

    if (dung->getComplete()) {
        scaleSurface = IMG_Load("images/next.png");
    }
    else {
        scaleSurface = IMG_Load("images/tryagain.png");
    }
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_Texture* level_texture = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!level_texture) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    scaleSurface = IMG_Load("images/exit level.png");
    if (!scaleSurface) {
        std::cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_Texture* exit_texture = SDL_CreateTextureFromSurface(renderer, scaleSurface);
    SDL_DestroySurface(scaleSurface); // Free the surface after creating the texture
    if (!exit_texture) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    SDL_FRect rect{ 220, 220 , 360,160 };//button size
    Button button(true, renderer, level_texture, rect);
    if (dung->getComplete()) {
        button.setFunc([&]() {nextLevel(); });
        if (currentLevel != 9) {
            unlockLevel();
        }
    }
    else {
        button.setFunc([&]() {tryagain(); });
    }
    rect.x = 220;
    rect.y = 420;
    rect.w = 360;
    rect.h = 160;
    Button ebutton(true, renderer, exit_texture, rect);
    ebutton.setFunc([&]() {exitlevel(); });
    buttons.emplace_back(button);
    buttons.emplace_back(ebutton);

}

void GameManager::nextLevel() {
    delete dung;
    paused = false;
    loadLevel(currentLevel + 1);
}

void GameManager::tryagain() {
    delete dung;
    paused = false;
    loadLevel(currentLevel);
}

void GameManager::exitlevel() {
    setUpLevelSelect();
    delete dung;
    dung = nullptr;
    paused = false;
}

void GameManager::unlockLevel() { // ai generated 
    //retrieve text file for the sppecified level

    ifstream file("levels/unlocked.txt", ios::binary);
    if (!file.is_open()) {
        cerr << "Cannot open file";
        return;
    }
    string encoded((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    string decoded = base64_decode(encoded);
    cout << decoded; // output current level
    if (currentLevel == stoi(decoded)) {//if is last level unlocked not playing a previously completed level
        string encoded = base64_encode(to_string(currentLevel+1));
        std::ofstream file("levels/unlocked.txt", std::ios::binary);
        file << encoded;
        file.close();
    }
}

int GameManager::getUnlockedLevels() {// ai generated
    ifstream file("levels/unlocked.txt", ios::binary);
    if (!file.is_open()) {
        cerr << "Cannot open file";
        return 1;
    }
    string encoded((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    string decoded = base64_decode(encoded);
    cout << decoded; // output current level
    return stoi(decoded);
}

void GameManager::loadLevel(int level) {
    buttons.resize(0);
    screen = inLevel;
    currentLevel = level;
    dung = new Dungeon(renderer, level);
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
    else if (dung && (event.type == SDL_EVENT_KEY_UP || event.type == SDL_EVENT_KEY_DOWN)) {
        dung->handleInput(event);
    }
}

void GameManager::update() {
    if (dung && !paused) {
        if (dung->update()) {
            paused = true;
            levelPaused();
        }
    }
}

//ai generated

string GameManager::base64_encode(const string& input) {
    static const char table[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    string output;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            output.push_back(table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) output.push_back(table[((val << 8) >> (valb + 8)) & 0x3F]);
    while (output.size() % 4) output.push_back('=');
    return output;
}


string GameManager::base64_decode(const string& input) {
    static const string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    vector<int> decoding_table(256, -1);
    for (int i = 0; i < 64; ++i) decoding_table[table[i]] = i;

    string output;
    int val = 0, valb = -8;
    for (unsigned char c : input) {
        if (decoding_table[c] == -1) break;
        val = (val << 6) + decoding_table[c];
        valb += 6;
        if (valb >= 0) {
            output.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return output;
}