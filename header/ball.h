#pragma once
#include "includes.h"

class Ball {
public:
    int speed;
    SDL_Texture* ballTex = nullptr;        
    SDL_Rect ballRect;

  
    float dx;
    float dy;

    Ball(SDL_Renderer* renderer, SDL_Rect ballRect, int speed, const char* ballSpritePath);
    ~Ball();

    void Draw(SDL_Renderer* renderer);

    // Pass in paddles & game state so we can handle bouncing and game over
    void Update(int screenWidth, int screenHeight, bool& gameRunning);
};
