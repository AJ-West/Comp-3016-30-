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
#include <memory>

#include "KeyTime.h"
#include "KeyHandler.h"

using namespace std;

class PlayerObj;
class MonsterObj;
class KeyTime;
class KeyHandler;
//class Minotaur;


//objects
#include "monsterObject.h"
#include "playerObject.h"

//components

#include "pathfinding.h"
#include "Texture.h"
#include "playerCollision.h"
#include "chargeCollision.h"
#include "particleComponent.h"
#include "movement.h"
#include "wallCollision.h"

class Dungeon {
public:
    //constructor
    Dungeon() {}
    Dungeon(SDL_Renderer* sdlrenderer, int levelNumber);
    ~Dungeon();

    //read the specified text file for the level
    void read_file();

    //draw the map each frame
    void render();

    vector<vector<char>> getOutline() { return outline; }

    void handleInput(SDL_Event input);

    void updateMonsters(float deltaTime);

    void spawn_monsters();
    void spawn_player();
    void save_tiles();
    void render_tiles();

    bool update(float deltaTime);

    void spawn_key();

    void createPWalkableOutline();
    void createMWalkableOutline();

    string base64_decode(const string& input);

    //getters
    int getDungeonX() { return dungeon_x; }
    int getDungeonY() { return dungeon_x; }
    int getWallSize() { return wall_size; }
    bool getComplete() { return complete; }
    vector<char> getWallTypes() { return wallTypes; }
    bool getRestart() { return restart; }

    //setters
    void setDungeonTile(int x, int y, SDL_FRect tile) { tiles[y][x] = tile; }

private:
    bool current_Key = false;

    //game renderer
    SDL_Renderer* renderer;
    //storing the dungeon as chars
    vector<vector<char>> outline;
    vector<vector<SDL_FRect>> tiles;
    vector<vector<int>> walkable_outline;


    int levelNum;
    //dungeon features
    int dungeon_x = 0, dungeon_y = 0;
    int dungeon_width, dungeon_height;
    int wall_size = 80;

    PlayerObj* player;

    vector<MonsterObj*> monsters;

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

    KeyHandler* keyHandler;

    SDL_Texture* tileset;

    bool complete = false;
    bool restart = false;

    vector<char> wallTypes{ '1','2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' };
};