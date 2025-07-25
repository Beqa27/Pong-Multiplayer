#define SDL_MAIN_HANDLED
#include "../header/includes.h"
#include "../header/variables.h"

// MP: Define network port and game roles
const int NETWORK_PORT = 1234;
enum class GameRole { NONE, SERVER, CLIENT };
GameRole currentRole = GameRole::NONE;

enum class SceneType {
    MENU,
    GAME,
    MULTIPLAYER
};
SceneType currentScene = SceneType::MENU;

NetworkManager networkManager;
bool isMultiplayer = false;

// MP UPDATE: Add target variables for interpolation on the client
float targetBallX = 0, targetBallY = 0;
float targetPaddleLeftY = 0, targetPaddleRightY = 0;

// MP UPDATE: Add a simple linear interpolation function
float lerp(float start, float end, float alpha) {
    return start + alpha * (end - start);
}


// Menu scene struct (NO CHANGES TO THIS STRUCT)
struct MenuScene {
    SDL_Rect startButtonRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50, 200, 50 };
    SDL_Rect multiplayerButtonRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 25, 200, 50 };

    void handleEvent(const SDL_Event& e, SceneType& scene) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x;
            int my = e.button.y;

            if (mx >= startButtonRect.x && mx <= startButtonRect.x + startButtonRect.w &&
                my >= startButtonRect.y && my <= startButtonRect.y + startButtonRect.h) {
                scene = SceneType::GAME;
                isMultiplayer = false;
                currentRole = GameRole::NONE;
            }
            if (mx >= multiplayerButtonRect.x && mx <= multiplayerButtonRect.x + multiplayerButtonRect.w &&
                my >= multiplayerButtonRect.y && my <= multiplayerButtonRect.y + multiplayerButtonRect.h) {
                scene = SceneType::MULTIPLAYER;
            }
        }
    }

    void render(SDL_Renderer* renderer, TTF_Font* font) {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &startButtonRect);
        SDL_RenderFillRect(renderer, &multiplayerButtonRect);

        SDL_Color white = {255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(font, "Single Player", white);
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        int textW, textH;
        SDL_QueryTexture(textTex, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = { startButtonRect.x + (startButtonRect.w - textW) / 2, startButtonRect.y + (startButtonRect.h - textH) / 2, textW, textH };
        SDL_RenderCopy(renderer, textTex, NULL, &textRect);
        SDL_DestroyTexture(textTex);

        surf = TTF_RenderText_Blended(font, "Multiplayer", white);
        textTex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        SDL_QueryTexture(textTex, NULL, NULL, &textW, &textH);
        textRect = { multiplayerButtonRect.x + (multiplayerButtonRect.w - textW) / 2, multiplayerButtonRect.y + (multiplayerButtonRect.h - textH) / 2, textW, textH };
        SDL_RenderCopy(renderer, textTex, NULL, &textRect);
        SDL_DestroyTexture(textTex);
    }
};

// Multiplayer Scene Struct (NO CHANGES TO THIS STRUCT)
struct MultiplayerScene {
    std::string inputText = "127.0.0.1";
    SDL_Rect inputRect = { WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2, 300, 40 };
    SDL_Rect hostButtonRect = { WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 60, 140, 50 };
    SDL_Rect joinButtonRect = { WINDOW_WIDTH / 2 + 10, WINDOW_HEIGHT / 2 + 60, 140, 50 };
    std::string message = "Enter Server IP and choose an option";

    void handleEvent(const SDL_Event& e, SceneType& scene, Ball& ball) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x;
            int my = e.button.y;
            
            if (mx >= hostButtonRect.x && mx <= hostButtonRect.x + hostButtonRect.w && my >= hostButtonRect.y && my <= hostButtonRect.y + hostButtonRect.h) {
                if (networkManager.Host(NETWORK_PORT)) {
                    message = "Waiting for client to connect...";
                    currentRole = GameRole::SERVER;
                    isMultiplayer = true;
                } else {
                    message = "Failed to host server.";
                }
            }
            else if (mx >= joinButtonRect.x && mx <= joinButtonRect.x + joinButtonRect.w && my >= joinButtonRect.y && my <= joinButtonRect.y + joinButtonRect.h) {
                message = "Connecting...";
                if (networkManager.Connect(inputText.c_str(), NETWORK_PORT)) {
                    currentRole = GameRole::CLIENT;
                    isMultiplayer = true;
                    // MP UPDATE: Initialize target positions to the ball's current local state
                    // to prevent it from jumping to 0,0 on connection.
                    targetBallX = ball.ballRect.x;
                    targetBallY = ball.ballRect.y;
                    targetPaddleLeftY = WINDOW_HEIGHT / 2 - 50;
                    targetPaddleRightY = WINDOW_HEIGHT / 2 - 50;
                    scene = SceneType::GAME;
                } else {
                    message = "Failed to connect to server.";
                }
            }
        } else if (e.type == SDL_TEXTINPUT) {
            inputText += e.text.text;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_BACKSPACE && !inputText.empty()) {
                inputText.pop_back();
            }
        }
    }

    void update(SceneType& scene) {
        if (currentRole == GameRole::SERVER && !networkManager.IsConnected()) {
            if (networkManager.CheckForClient()) {
                scene = SceneType::GAME;
            }
        }
    }

    void render(SDL_Renderer* renderer, TTF_Font* font) { // NO CHANGES to this render func
        SDL_SetRenderDrawColor(renderer, 20, 20, 60, 255);
        SDL_RenderClear(renderer);

        SDL_Color white = {255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), white);
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        int textW, textH;
        SDL_QueryTexture(textTex, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = { WINDOW_WIDTH / 2 - textW / 2, WINDOW_HEIGHT / 2 - 50, textW, textH };
        SDL_RenderCopy(renderer, textTex, NULL, &textRect);
        SDL_DestroyTexture(textTex);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &inputRect);
        if (!inputText.empty()) {
            surf = TTF_RenderText_Blended(font, inputText.c_str(), {0, 0, 0});
            textTex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FreeSurface(surf);
            SDL_QueryTexture(textTex, NULL, NULL, &textW, &textH);
            SDL_Rect textRect_input = { inputRect.x + 5, inputRect.y + (inputRect.h - textH) / 2, textW, textH };
            SDL_RenderCopy(renderer, textTex, NULL, &textRect_input);
            SDL_DestroyTexture(textTex);
        }
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &hostButtonRect);
        SDL_RenderFillRect(renderer, &joinButtonRect);
        surf = TTF_RenderText_Blended(font, "Host Game", white);
        textTex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_QueryTexture(textTex, NULL, NULL, &textW, &textH);
        textRect = { hostButtonRect.x + (hostButtonRect.w - textW) / 2, hostButtonRect.y + (hostButtonRect.h - textH) / 2, textW, textH };
        SDL_RenderCopy(renderer, textTex, NULL, &textRect);
        SDL_DestroyTexture(textTex);
        surf = TTF_RenderText_Blended(font, "Join Game", white);
        textTex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_QueryTexture(textTex, NULL, NULL, &textW, &textH);
        textRect = { joinButtonRect.x + (joinButtonRect.w - textW) / 2, joinButtonRect.y + (joinButtonRect.h - textH) / 2, textW, textH };
        SDL_RenderCopy(renderer, textTex, NULL, &textRect);
        SDL_DestroyTexture(textTex);
    }
};

void resetGame(Ball& ball) {
    playerScore_1 = 0;
    playerScore_2 = 0;
    ball.reset();
}


int main() {
    SDLApp app("Pong", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!app.window || !app.renderer || !app.font) return -1;
    SDL_StartTextInput();

    Ball ball(app.renderer, ballRect, ballSpeed, ballImagePath);
    Paddle paddleLeft(5, paddleRect1, SDL_SCANCODE_W, SDL_SCANCODE_S);
    Paddle paddleRight(5, paddleRect2, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    MenuScene menu;
    MultiplayerScene multiplayer;
    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;

            if (currentScene == SceneType::MENU) {
                menu.handleEvent(e, currentScene);
            } else if (currentScene == SceneType::MULTIPLAYER) {
                multiplayer.handleEvent(e, currentScene, ball);
                 if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                    currentScene = SceneType::MENU;
                    networkManager.CloseConnection();
                    currentRole = GameRole::NONE;
                }
            } else if (currentScene == SceneType::GAME) {
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                    currentScene = SceneType::MENU;
                    if(isMultiplayer) {
                        networkManager.CloseConnection();
                        isMultiplayer = false;
                        currentRole = GameRole::NONE;
                        resetGame(ball);
                    }
                }
            }
        }
        
        const Uint8* keystates = SDL_GetKeyboardState(nullptr);

        // Update Logic
        if (currentScene == SceneType::GAME) {
            if (!isMultiplayer) { // SINGLE PLAYER
                paddleLeft.update(keystates);
                paddleRight.update(keystates);
                ball.Update(screenWidth, screenHeight, gameRunning, paddleLeft.PaddleRectl, paddleRight.PaddleRectl);
            } else { // MULTIPLAYER
                if (!networkManager.IsConnected()) {
                    currentScene = SceneType::MENU;
                    isMultiplayer = false;
                    currentRole = GameRole::NONE;
                    resetGame(ball);
                    continue;
                }

                if (currentRole == GameRole::SERVER) {
                    InputPacket input;
                    if (networkManager.ReceiveData(&input, sizeof(InputPacket))) {
                        paddleRight.PaddleRectl.y = input.paddleY;
                    }
                    paddleLeft.update(keystates);
                    ball.Update(screenWidth, screenHeight, gameRunning, paddleLeft.PaddleRectl, paddleRight.PaddleRectl);
                    
                    GameStatePacket state;
                    state.ballX = ball.ballRect.x;
                    state.ballY = ball.ballRect.y;
                    state.paddleLeftY = paddleLeft.PaddleRectl.y;
                    state.paddleRightY = paddleRight.PaddleRectl.y;
                    state.scoreLeft = playerScore_1;
                    state.scoreRight = playerScore_2;
                    networkManager.SendData(&state, sizeof(GameStatePacket));

                } else if (currentRole == GameRole::CLIENT) {
                    // MP UPDATE: This logic is almost completely new.
                    
                    // 1. Send our local input to the server every frame.
                    paddleRight.update(keystates);
                    InputPacket input;
                    input.paddleY = paddleRight.PaddleRectl.y;
                    networkManager.SendData(&input, sizeof(InputPacket));
                    
                    // 2. Check for new data from the server and update our TARGETS.
                    GameStatePacket state;
                    if(networkManager.ReceiveData(&state, sizeof(GameStatePacket))) {
                        targetBallX = state.ballX;
                        targetBallY = state.ballY;
                        targetPaddleLeftY = state.paddleLeftY;
                        targetPaddleRightY = state.paddleRightY;
                        playerScore_1 = state.scoreLeft;
                        playerScore_2 = state.scoreRight;
                    }

                    // 3. Every frame, smoothly interpolate our local objects towards their targets.
                    // This creates smooth movement regardless of network jitter.
                    float interpolationFactor = 0.15f; // Adjust this value to make it smoother or more responsive
                    ball.ballRect.x = lerp(ball.ballRect.x, targetBallX, interpolationFactor);
                    ball.ballRect.y = lerp(ball.ballRect.y, targetBallY, interpolationFactor);
                    paddleLeft.PaddleRectl.y = lerp(paddleLeft.PaddleRectl.y, targetPaddleLeftY, interpolationFactor);
                    
                    // We interpolate our own paddle as well to ensure it matches the server's authoritative state,
                    // which prevents desync issues.
                    paddleRight.PaddleRectl.y = lerp(paddleRight.PaddleRectl.y, targetPaddleRightY, interpolationFactor);
                }
            }
        } else if (currentScene == SceneType::MULTIPLAYER) {
            multiplayer.update(currentScene);
        }

        // Rendering Logic (NO CHANGES HERE)
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        if (currentScene == SceneType::MENU) {
            menu.render(app.renderer, app.font);
        } else if (currentScene == SceneType::MULTIPLAYER) {
            multiplayer.render(app.renderer, app.font);
        } else if (currentScene == SceneType::GAME) {
            SDL_SetRenderDrawColor(app.renderer, 150, 150, 150, 255);
            SDL_RenderDrawLine(app.renderer, screenWidth / 2, 0, screenWidth / 2, screenHeight);
            
            ball.Draw(app.renderer);
            paddleLeft.draw(app.renderer);
            paddleRight.draw(app.renderer);
            
            SDL_Color white = {255, 255, 255};
            std::string score1_str = std::to_string(playerScore_1);
            std::string score2_str = std::to_string(playerScore_2);
            SDL_Surface* surf1 = TTF_RenderText_Blended(app.font, score1_str.c_str(), white);
            SDL_Surface* surf2 = TTF_RenderText_Blended(app.font, score2_str.c_str(), white);
            SDL_Texture* tex1 = SDL_CreateTextureFromSurface(app.renderer, surf1);
            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(app.renderer, surf2);
            SDL_Rect scoreRect1 = { screenWidth / 4, 10, surf1->w, surf1->h };
            SDL_Rect scoreRect2 = { 3 * screenWidth / 4, 10, surf2->w, surf2->h };
            SDL_RenderCopy(app.renderer, tex1, nullptr, &scoreRect1);
            SDL_RenderCopy(app.renderer, tex2, nullptr, &scoreRect2);
            SDL_FreeSurface(surf1);
            SDL_FreeSurface(surf2);
            SDL_DestroyTexture(tex1);
            SDL_DestroyTexture(tex2);
        }

        SDL_RenderPresent(app.renderer);
        SDL_Delay(16);
    }

    networkManager.CloseConnection();
    return 0;
}