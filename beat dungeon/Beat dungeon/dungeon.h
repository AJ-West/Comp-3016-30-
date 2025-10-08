#pragma once
#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

using namespace std;

class Dungeon {
public:
    Dungeon(int width, int height, SDL_Renderer* sdlrenderer);
    ~Dungeon();

    void read_file(string fileName);

    void load();

    void render();

private:
    SDL_Renderer* renderer;
    vector<vector<char>> outline;
    int dungeon_width;
    int dungeon_height;
};