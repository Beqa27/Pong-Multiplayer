#pragma once
#include "includes.h"

class Paddle{
public:
    int speed;
    SDL_Rect PaddleRectl;
    SDL_Scancode upKey;
    SDL_Scancode downKey;

    Paddle(int speed, SDL_Rect rect, SDL_Scancode upKey, SDL_Scancode downKey);

    void update(const Uint8* keystates);
    void draw(SDL_Renderer * renderer);
};
