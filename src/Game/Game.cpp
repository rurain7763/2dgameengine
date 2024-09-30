#include "Game.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() 
    : _prevFrameMilliSecs(0)
{
    _registry = std::make_unique<Registry>();
}

Game::~Game() {

}

void Game::Init() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error init SDL");
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
        Logger::Err("Error creating window");
        return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if(!_renderer) {
        Logger::Err("Error creating renderer");
        return;
    }
    
    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);

    _isRunning = true;

    LOG("Hello world!");
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
    Entity tank = _registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10, 30), glm::vec2(1, 1), 0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(50, 0));
    tank.RemoveComponent<RigidBodyComponent>();
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

    // system update
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    // render system

    SDL_RenderPresent(_renderer);
}
