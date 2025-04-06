#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
    class Game{
public:
    SDL_Windows* windows;
    SDL_Renderer* renderer;
    bool running;
    Game(){}
    void render (){}
    void run(){}
    ~ Game (){}
    };



