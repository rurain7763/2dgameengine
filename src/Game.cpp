#include "Game.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>

Game::Game() {

}

Game::~Game() {

}

void Game::Init() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error init SDL\n";
        return;
    }

    SDL_DisplayMode displayMod;
    SDL_GetCurrentDisplayMode(0, &displayMod);
    _windowWidth = displayMod.w;
    _windowHeight = displayMod.h;

    _window = SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _windowWidth,
        _windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if(!_window) {
        std::cerr << "Error creating window\n";
        return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if(!_renderer) {
        std::cerr << "Error creating renderer\n";
        return;
    }
    
    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);

    _isRunning = true;
}

void Game::Run() {
    while(_isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Game::Setup() {
    
}

void Game::ProcessInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    _isRunning = false;
                }
                break;
        }
    }
}

void Game::Update() {

}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRt = { 10, 10, 64, 64 };
    SDL_RenderCopy(_renderer, texture, nullptr, &dstRt);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(_renderer);
}
