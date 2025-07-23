#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

class SDLApp
{
public:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;  // move font here as a member

    SDLApp(const char *title, int width, int height)
    {
        if (SDL_Init(SDL_INIT_VIDEO) == -1)
        {
            std::cerr << "SDL INIT ERROR: " << SDL_GetError() << std::endl;
        }
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        {
            std::cerr << "IMG INIT ERROR: " << IMG_GetError() << std::endl;
        }
        if (TTF_Init() == -1)
        {
            std::cerr << "TTF INIT ERROR: " << TTF_GetError() << std::endl;
        }

        font = TTF_OpenFont("font/basic_square.ttf", 32);
        if (!font)
        {
            std::cerr << "TTF Open Font ERROR: " << TTF_GetError() << std::endl;
            
        }

        window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width, height,
                                  SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    ~SDLApp()
    {
        if (font)
        {
            TTF_CloseFont(font);
            font = nullptr;
        }
        if (renderer)
            SDL_DestroyRenderer(renderer);
        if (window)
            SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    SDL_Window *getWindow() const { return window; }
    SDL_Renderer *getRenderer() const { return renderer; }
};
