#include "../header/includes.h"
#include "../header/variables.h"

Paddle::Paddle(int speed, SDL_Rect rect, SDL_Scancode upKey, SDL_Scancode downKey)
    : speed(speed), PaddleRectl(rect), upKey(upKey), downKey(downKey) {}

void Paddle::update(const Uint8* keystates) {
    if (keystates[upKey]) {
        PaddleRectl.y -= speed;
        if (PaddleRectl.y < 0) PaddleRectl.y = 0;
    }
    if (keystates[downKey]) {
        PaddleRectl.y += speed;
        if (PaddleRectl.y + PaddleRectl.h > WINDOW_HEIGHT)
            PaddleRectl.y = WINDOW_HEIGHT - PaddleRectl.h;
    }
}

void Paddle::draw(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &PaddleRectl);
}
