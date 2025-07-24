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
    void Update(int screenWidth, int screenHeight, bool& gameRunning, const SDL_Rect& paddleLeftRect, const SDL_Rect& paddleRightRect);
    
    // ADDED: Reset function declaration
    void Reset();
};