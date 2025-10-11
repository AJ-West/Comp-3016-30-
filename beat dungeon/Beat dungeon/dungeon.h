#pragma once
#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

#include "player.h"
#include "monster.h"

using namespace std;

class Player;
class Monster;

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
};