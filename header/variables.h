#pragma once
#include "SDL.h"

// Screen stuff
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Paddle stuff
extern int paddleSpeed;
extern SDL_Rect paddleRect1;
extern SDL_Rect paddleRect2;

// Ball related stuff
extern SDL_Rect ballRect;
extern int ballSpeed;
extern const char* ballImagePath;
extern bool gameRunning;
extern int screenWidth;
extern int screenHeight;


extern SDL_Rect paddleLeft;
extern SDL_Rect paddleRight;
