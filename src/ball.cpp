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

void Ball::Update(int screenWidth, int screenHeight, bool& gameRunning) {
    // Move ball
    ballRect.x += static_cast<int>(dx * speed);
    ballRect.y += static_cast<int>(dy * speed);

    // Bounce only on top and bottom
    if (ballRect.y <= 0) {
        ballRect.y = 0;
        dy = -dy;
    }
    if (ballRect.y + ballRect.h >= screenHeight) {
        ballRect.y = screenHeight - ballRect.h;
        dy = -dy;
    }

    // If ball touches left or right edge, game over or reset
    if (ballRect.x <= 0) {
        gameRunning = false;  // left side lost
        SDL_Log("Game Over! Right player wins!");
    }
    else if (ballRect.x + ballRect.w >= screenWidth) {
        gameRunning = false;  // right side lost
        SDL_Log("Game Over! Left player wins!");
    }
}

