#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() 
    : _prevFrameMilliSecs(0)
{}

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
    Setup();
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

glm::vec2 position = {10, 10};
void Game::Update() {
    int timeToWait = (_prevFrameMilliSecs + MILLISECS_PER_FRAME) - SDL_GetTicks();
    if(timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    double deltaTime = (SDL_GetTicks() - _prevFrameMilliSecs) / 1000.0;
    _prevFrameMilliSecs = SDL_GetTicks();

    position.x += 100.f * deltaTime;
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRt = { 
        static_cast<int>(position.x), 
        static_cast<int>(position.y), 
        32, 
        32 
    };
    
    SDL_RenderCopy(_renderer, texture, nullptr, &dstRt);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(_renderer);
}
