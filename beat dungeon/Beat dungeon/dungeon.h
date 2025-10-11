#pragma once
#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "monster.h"

using namespace std;

class Dungeon {
public:
    //constructor
    Dungeon(int width, int height, SDL_Renderer* sdlrenderer);
    ~Dungeon();

    //read the specified text file for the level
    void read_file(string fileName);

    //draw the map each frame
    void render();

    vector<vector<char>> getOutline() { return outline; }

    //getters
    int getDungeonX() { return dungeon_x; }
    int getDungeonY() { return dungeon_x; }
    int getWallSize() { return wall_size; }

private:
    //game renderer
    SDL_Renderer* renderer;
    //storing the dungeon as chars
    vector<vector<char>> outline;
    //dungeon features
    int dungeon_x = 100, dungeon_y = 100;
    int dungeon_width, dungeon_height;
    int wall_size = 60;
};