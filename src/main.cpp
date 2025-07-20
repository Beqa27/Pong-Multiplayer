#define SDL_MAIN_HANDLED
#include "../header/includes.h"

// #define SDL_MAIN_HANDLED
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
int main()
{
    // I IMPORTED CLASS SO CODE LOOKS OVERALL CLEANER
    SDLApp app("Space Invaders", WINDOW_WIDTH, WINDOW_HEIGHT);

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
 
        

        // DRAW STUFF HERE
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);

        SDL_RenderClear(app.renderer);

        SDL_RenderPresent(app.renderer);

        // TS IS MEASURED IN MILLISECONDS SO IM DOING 60 FPS
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}
