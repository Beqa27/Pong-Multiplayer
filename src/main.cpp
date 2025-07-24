#define SDL_MAIN_HANDLED
#include "../header/includes.h"
#include "../header/variables.h"

int main()
{
    SDLApp app("Space Invaders", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!app.window || !app.renderer || !app.font) return -1;

    Ball ball(app.renderer, ballRect, ballSpeed, ballImagePath);
    Paddle paddleLeft(5, paddleRect1, SDL_SCANCODE_W, SDL_SCANCODE_S);
    Paddle paddleRight(5, paddleRect2, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    // DELETED: Local score variables
    // int playerScore1 = 0;
    // int playerScore2 = 0;

    // MODIFIED: Use the global score variables from variables.h
    std::string playerScoreToTxt_1 = std::to_string(playerScore_1);
    std::string playerScoreToTxt_2 = std::to_string(playerScore_2);

    SDL_Texture* textTexture1 = nullptr;
    SDL_Texture* textTexture2 = nullptr;

    // Create initial textures
    {
        SDL_Surface* surf = TTF_RenderText_Blended(app.font, playerScoreToTxt_1.c_str(), {255, 255, 255});
        if (surf) {
            textTexture1 = SDL_CreateTextureFromSurface(app.renderer, surf);
            SDL_FreeSurface(surf);
        }

        surf = TTF_RenderText_Blended(app.font, playerScoreToTxt_2.c_str(), {255, 255, 255});
        if (surf) {
            textTexture2 = SDL_CreateTextureFromSurface(app.renderer, surf);
            SDL_FreeSurface(surf);
        }
    }

    SDL_Event e;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) running = false;
        }

        const Uint8* keystates = SDL_GetKeyboardState(nullptr);
        ball.Update(screenWidth, screenHeight, gameRunning, paddleLeft.PaddleRectl, paddleRight.PaddleRectl);
        paddleLeft.update(keystates);
        paddleRight.update(keystates);

        // Convert scores to string and recreate textures if score changed
        // MODIFIED: Use the global score variables
        std::string newScoreStr1 = std::to_string(playerScore_1);
        if (newScoreStr1 != playerScoreToTxt_1) {
            playerScoreToTxt_1 = newScoreStr1;
            SDL_DestroyTexture(textTexture1);
            SDL_Surface* surf = TTF_RenderText_Blended(app.font, playerScoreToTxt_1.c_str(), {255, 255, 255});
            if (surf) {
                textTexture1 = SDL_CreateTextureFromSurface(app.renderer, surf);
                SDL_FreeSurface(surf);
            }
        }

        // MODIFIED: Use the global score variables
        std::string newScoreStr2 = std::to_string(playerScore_2);
        if (newScoreStr2 != playerScoreToTxt_2) {
            playerScoreToTxt_2 = newScoreStr2;
            SDL_DestroyTexture(textTexture2);
            SDL_Surface* surf = TTF_RenderText_Blended(app.font, playerScoreToTxt_2.c_str(), {255, 255, 255});
            if (surf) {
                textTexture2 = SDL_CreateTextureFromSurface(app.renderer, surf);
                SDL_FreeSurface(surf);
            }
        }

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        SDL_SetRenderDrawColor(app.renderer, 150, 150, 150, 255);
        SDL_RenderDrawLine(app.renderer, screenWidth / 2, 0, screenWidth / 2, screenHeight);

        ball.Draw(app.renderer);
        paddleLeft.draw(app.renderer);
        paddleRight.draw(app.renderer);

        // Draw both scores
        SDL_Rect txtDstRect1 = { screenWidth / 4, 10, 30, 30 }; // adjust size and position
        SDL_Rect txtDstRect2 = { 3 * screenWidth / 4, 10, 30, 30 };
        if (textTexture1)
            SDL_RenderCopy(app.renderer, textTexture1, nullptr, &txtDstRect1);
        if (textTexture2)
            SDL_RenderCopy(app.renderer, textTexture2, nullptr, &txtDstRect2);

        SDL_RenderPresent(app.renderer);
        SDL_Delay(16);
    }

    // ... (rest of the file is unchanged)
    if (textTexture1) SDL_DestroyTexture(textTexture1);
    if (textTexture2) SDL_DestroyTexture(textTexture2);
    //TTF_CloseFont(app.font);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    
    SDL_Quit();

    return 0;
}