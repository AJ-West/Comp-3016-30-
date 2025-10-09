#pragma once
#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

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

private:
    //game renderer
    SDL_Renderer* renderer;
    //storing the dungeon as chars
    vector<vector<char>> outline;
    //dungeon size
    int dungeon_width, dungeon_height;
};