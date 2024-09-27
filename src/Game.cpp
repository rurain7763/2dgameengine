#include "Game.h"
#include "SDL2/SDL.h"
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

    _window = SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
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

}
