#define SDL_MAIN_HANDLED
#include "../header/includes.h"
#include "../header/variables.h"

int main()
{
    // I IMPORTED CLASS SO CODE LOOKS OVERALL CLEANER
    SDLApp app("Space Invaders", WINDOW_WIDTH, WINDOW_HEIGHT);
    Ball ball = Ball(app.renderer, ballRect, ballSpeed, ballImagePath);
    Paddle paddleLeft(5, paddleRect1, SDL_SCANCODE_W, SDL_SCANCODE_S);
    Paddle paddleRight(5, paddleRect2, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    SDL_Event e;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // UPDATE LOOP
        const Uint8 *keystates = SDL_GetKeyboardState(nullptr);
        ball.Update(screenWidth, screenHeight, gameRunning, paddleLeft.PaddleRectl, paddleRight.PaddleRectl);
        paddleLeft.update(keystates);
        paddleRight.update(keystates);

        // DRAW STUFF HERE

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);

        SDL_RenderClear(app.renderer);
        SDL_SetRenderDrawColor(app.renderer, 150, 150, 150, 255);
        SDL_RenderDrawLine(app.renderer, screenWidth / 2, 0, screenWidth / 2, screenHeight);
        ball.Draw(app.renderer);

        paddleLeft.draw(app.renderer);
        paddleRight.draw(app.renderer);

        SDL_RenderPresent(app.renderer);

        // TS IS MEASURED IN MILLISECONDS SO IM DOING 60 FPS
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}
