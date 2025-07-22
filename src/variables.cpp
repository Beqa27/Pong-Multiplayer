#include "../header/variables.h"

// Paddle stuff
int paddleSpeed = 5;
SDL_Rect paddleRect1 = { 50, WINDOW_HEIGHT / 2 - 50, 10, 100 };
SDL_Rect paddleRect2 = { WINDOW_WIDTH - 60, WINDOW_HEIGHT / 2 - 50, 10, 100 };

// Ball related stuff
SDL_Rect ballRect = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 50, 50 };
int ballSpeed = 5;
const char* ballImagePath = "assets/ball.png";
bool gameRunning = true;
int screenWidth = WINDOW_WIDTH;
int screenHeight = WINDOW_HEIGHT;

SDL_Rect paddleLeft = { 50, WINDOW_HEIGHT / 2 - 50, 10, 100 };
SDL_Rect paddleRight = { WINDOW_WIDTH - 60, WINDOW_HEIGHT / 2 - 50, 10, 100 };
