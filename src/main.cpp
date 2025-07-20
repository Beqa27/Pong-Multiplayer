#define SDL_MAIN_HANDLED
#include "../header/includes.h"
#include "../header/variables.h"


int main()
{
    // I IMPORTED CLASS SO CODE LOOKS OVERALL CLEANER
    SDLApp app("Space Invaders", WINDOW_WIDTH, WINDOW_HEIGHT);
    Ball ball = Ball(app.renderer, ballRect, ballSpeed, ballImagePath);
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
        ball.Update(screenWidth, screenHeight, gameRunning);
        

        // DRAW STUFF HERE
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
        


        SDL_RenderClear(app.renderer);
        ball.Draw(app.renderer);

        SDL_RenderPresent(app.renderer);

        // TS IS MEASURED IN MILLISECONDS SO IM DOING 60 FPS
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}
