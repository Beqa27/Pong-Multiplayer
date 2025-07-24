#define SDL_MAIN_HANDLED
#include "../header/includes.h"
#include "../header/variables.h"

enum class SceneType {
    MENU,
    GAME
};

SceneType currentScene = SceneType::MENU;

// Menu scene struct
struct MenuScene {
    SDL_Rect startButtonRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 25, 200, 50 };

    void handleEvent(const SDL_Event& e, SceneType& scene) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x;
            int my = e.button.y;
            if (mx >= startButtonRect.x && mx <= startButtonRect.x + startButtonRect.w &&
                my >= startButtonRect.y && my <= startButtonRect.y + startButtonRect.h) {
                scene = SceneType::GAME;
            }
        }
    }

    void render(SDL_Renderer* renderer, TTF_Font* font) {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Draw start button
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &startButtonRect);

        // Draw button text
        SDL_Color white = {255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(font, "Start", white);
        if (surf) {
            SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FreeSurface(surf);

            int textW, textH;
            SDL_QueryTexture(textTex, NULL, NULL, &textW, &textH);
            SDL_Rect textRect = { startButtonRect.x + (startButtonRect.w - textW) / 2,
                                  startButtonRect.y + (startButtonRect.h - textH) / 2,
                                  textW, textH };
            SDL_RenderCopy(renderer, textTex, NULL, &textRect);
            SDL_DestroyTexture(textTex);
        }
    }
};

int main()
{
    SDLApp app("Space Invaders", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!app.window || !app.renderer || !app.font) return -1;

    Ball ball(app.renderer, ballRect, ballSpeed, ballImagePath);
    Paddle paddleLeft(5, paddleRect1, SDL_SCANCODE_W, SDL_SCANCODE_S);
    Paddle paddleRight(5, paddleRect2, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    // Initial score textures
    std::string playerScoreToTxt_1 = std::to_string(playerScore_1);
    std::string playerScoreToTxt_2 = std::to_string(playerScore_2);

    SDL_Texture* textTexture1 = nullptr;
    SDL_Texture* textTexture2 = nullptr;

    // Create initial score textures
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

    MenuScene menu;

    SDL_Event e;
    bool running = true;

    while (running)
    {
while (SDL_PollEvent(&e))
{
    if (e.type == SDL_QUIT) running = false;

    if (currentScene == SceneType::MENU)
    {
        menu.handleEvent(e, currentScene);
    }
    else if (currentScene == SceneType::GAME)
    {
        // Check if Escape is pressed → go back to menu
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            // Reset scores or game state if needed
            playerScore_1 = 0;
            playerScore_2 = 0;

            // Reset ball and paddles if you want (optional)
            ballRect = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 50, 50 };
            paddleRect1 = { 50, WINDOW_HEIGHT/2 - 50, 10, 100 };
            paddleRect2 = { WINDOW_WIDTH - 60, WINDOW_HEIGHT/2 - 50, 10, 100 };

            // Reset textures
            playerScoreToTxt_1 = std::to_string(playerScore_1);
            playerScoreToTxt_2 = std::to_string(playerScore_2);

            SDL_DestroyTexture(textTexture1);
            SDL_DestroyTexture(textTexture2);

            SDL_Surface* surf = TTF_RenderText_Blended(app.font, playerScoreToTxt_1.c_str(), {255,255,255});
            if (surf) {
                textTexture1 = SDL_CreateTextureFromSurface(app.renderer, surf);
                SDL_FreeSurface(surf);
            }
            surf = TTF_RenderText_Blended(app.font, playerScoreToTxt_2.c_str(), {255,255,255});
            if (surf) {
                textTexture2 = SDL_CreateTextureFromSurface(app.renderer, surf);
                SDL_FreeSurface(surf);
            }
            ball.reset();

            currentScene = SceneType::MENU;
        }
    }
}


        if (currentScene == SceneType::MENU ) {
            menu.render(app.renderer, app.font);
        }
        else if (currentScene == SceneType::GAME) {
            // Game update logic
            const Uint8* keystates = SDL_GetKeyboardState(nullptr);
            ball.Update(screenWidth, screenHeight, gameRunning, paddleLeft.PaddleRectl, paddleRight.PaddleRectl);
            paddleLeft.update(keystates);
            paddleRight.update(keystates);

            // Update score textures if score changed
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

            // Render game
            SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
            SDL_RenderClear(app.renderer);

            SDL_SetRenderDrawColor(app.renderer, 150, 150, 150, 255);
            SDL_RenderDrawLine(app.renderer, screenWidth / 2, 0, screenWidth / 2, screenHeight);

            ball.Draw(app.renderer);
            paddleLeft.draw(app.renderer);
            paddleRight.draw(app.renderer);

            // Draw scores
            SDL_Rect txtDstRect1 = { screenWidth / 4, 10, 30, 30 };
            SDL_Rect txtDstRect2 = { 3 * screenWidth / 4, 10, 30, 30 };
            if (textTexture1)
                SDL_RenderCopy(app.renderer, textTexture1, nullptr, &txtDstRect1);
            if (textTexture2)
                SDL_RenderCopy(app.renderer, textTexture2, nullptr, &txtDstRect2);
        }

        SDL_RenderPresent(app.renderer);
        SDL_Delay(16);
    }

    // Cleanup
    if (textTexture1) SDL_DestroyTexture(textTexture1);
    if (textTexture2) SDL_DestroyTexture(textTexture2);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}
