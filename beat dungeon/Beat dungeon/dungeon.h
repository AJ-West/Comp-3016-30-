#pragma once
#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>

#include "player.h"
#include "monster.h"
#include "KeyTime.h"
#include "Skeleton.h"
#include "Minotaur.h"

using namespace std;

class Player;
//class Monster;
class KeyTime;
class Minotaur;

class Dungeon {
public:
    //constructor
    Dungeon() {};
    Dungeon(SDL_Renderer* sdlrenderer, int levelNumber);
    ~Dungeon();

    //read the specified text file for the level
    void read_file();

    //draw the map each frame
    void render();

    vector<vector<char>> getOutline() { return outline; }

    void handleInput(SDL_Event input);

    void moveMonsters();

    void spawn_entities();

    void update();

    void spawn_key();

    //getters
    int getDungeonX() { return dungeon_x; }
    int getDungeonY() { return dungeon_x; }
    int getWallSize() { return wall_size; }

private:
    //game renderer
    SDL_Renderer* renderer;
    //storing the dungeon as chars
    vector<vector<char>> outline;
    int levelNum;
    //dungeon features
    int dungeon_x = 100, dungeon_y = 100;
    int dungeon_width, dungeon_height;
    int wall_size = 60;

    Player* player;

    vector<Monster> monsters;

    vector<KeyTime*> current_keys;

    vector<SDL_Keycode> all_keys{
        //numbers
        SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
        //letters
        SDLK_A, SDLK_B, SDLK_C, SDLK_D, SDLK_E, SDLK_F, SDLK_G, SDLK_H, SDLK_I, SDLK_J, SDLK_K, SDLK_L, SDLK_M, SDLK_N, SDLK_O, SDLK_P, SDLK_Q, SDLK_R, SDLK_S, SDLK_T, SDLK_U, SDLK_V, SDLK_W, SDLK_X, SDLK_Y, SDLK_Z,
        //misc
        SDLK_SPACE, SDLK_UP, SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN
    };

    double last_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    SDL_Texture* key_outline;
    SDL_Texture* key_bad_outline;
    SDL_Texture* key_dot;
};