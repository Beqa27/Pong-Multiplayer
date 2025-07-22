#include "../header/includes.h"

Ball::Ball(SDL_Renderer* renderer, SDL_Rect ballRect, int speed, const char* ballSpritePath)
    : speed(speed), ballRect(ballRect)
{
    SDL_Surface* surface = IMG_Load(ballSpritePath);
    if (surface) {
        ballTex = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        SDL_Log("Failed to load image: %s", SDL_GetError());
    }

    // Start ball diagonally
    dx = (rand() % 2 == 0) ? 1.0f : -1.0f;
    dy = ((rand() % 200) / 100.0f - 1.0f); // random between -1 and 1

    // Normalize so total speed remains constant
    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;
}


Ball::~Ball() {
    if (ballTex) {
        SDL_DestroyTexture(ballTex);
        ballTex = nullptr;
    }
}

void Ball::Draw(SDL_Renderer* renderer) {
    if (ballTex) {
        SDL_RenderCopy(renderer, ballTex, nullptr, &ballRect);
    }
}

void Ball::Update(int screenWidth, int screenHeight, bool& gameRunning, const SDL_Rect& paddleLeftRect, const SDL_Rect& paddleRightRect) {
    ballRect.x += static_cast<int>(dx * speed);
    ballRect.y += static_cast<int>(dy * speed);

    if (ballRect.y <= 0) {
        ballRect.y = 0;
        dy = -dy;
    }
    if (ballRect.y + ballRect.h >= screenHeight) {
        ballRect.y = screenHeight - ballRect.h;
        dy = -dy;
    }

    if (ballRect.x <= 0) {
        gameRunning = false;
        SDL_Log("Game Over! Right player wins!");
    }
    else if (ballRect.x + ballRect.w >= screenWidth) {
        gameRunning = false;
        SDL_Log("Game Over! Left player wins!");
    }

    // Paddle bounce logic
    if (SDL_HasIntersection(&ballRect, &paddleLeftRect)) {
        ballRect.x = paddleLeftRect.x + paddleLeftRect.w;  // Prevent sticking
        dx = -dx;
    }
    else if (SDL_HasIntersection(&ballRect, &paddleRightRect)) {
        ballRect.x = paddleRightRect.x - ballRect.w;  // Prevent sticking
        dx = -dx;
    }
}


