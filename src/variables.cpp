#include "../header/variables.h"
#include <iostream>
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

// Text stuff

SDL_Rect txtSrcRect = { 0, 0, 100, 50 };
SDL_Rect txtDstRect = { (WINDOW_WIDTH - 120) / 2, 20, 50, 100 }; 

// GamePlay

int playerScore_1 = 0;
int playerScore_2 = 0;

std::string playerScoreToTxt_1 = std::to_string(playerScore_1);
std::string playerScoreToTxt_2 = std::to_string(playerScore_2);