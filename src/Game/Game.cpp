#include "Game.h"
#include "../Logger/Logger.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

Game::Game() 
    : _prevFrameMilliSecs(0)
{
    _registry = std::make_unique<Registry>();
    _assetManager = std::make_unique<AssetManager>();
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
    _registry->AddSystem<MovementSystem>();
    _registry->AddSystem<RenderSystem>();

    _assetManager->AddTexture(_renderer, "tank_image", "./assets/images/tank-tiger-right.png");
    _assetManager->AddTexture(_renderer, "truck_image", "./assets/images/truck-ford-right.png");
    _assetManager->AddTexture(_renderer, "jungle_map", "./assets/tilemaps/jungle.png");

    Entity tank = _registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10, 30), glm::vec2(1, 1), 0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(50, 0));
    tank.AddComponent<SpriteComponent>("tank_image", 32, 32);

    Entity truck = _registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(50, 100), glm::vec2(1, 1), 0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0, 50));
    truck.AddComponent<SpriteComponent>("truck_image", 32, 32);

    std::fstream file("./assets/tilemaps/jungle.map");
    const float tileSize = 32.f;
    const int numMapStride = 10;
    int x = 0, y = 0;
    std::string line;
    while(std::getline(file, line)) {
        int pos;
        int idx;
        while((pos = line.find(',')) != std::string::npos) {
            idx = stoi(line.substr(0, pos));

            Entity tile = _registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0);
            tile.AddComponent<SpriteComponent>("jungle_map", tileSize, tileSize, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);

            line.erase(0, pos + 1);
            x += tileSize;
        }

        idx = stoi(line);

        Entity tile = _registry->CreateEntity();
        tile.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0);
        tile.AddComponent<SpriteComponent>("jungle_map", tileSize, tileSize, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);

        y += tileSize;
        x = 0;
    }
    file.close();
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
    float deltaTime = (SDL_GetTicks() - _prevFrameMilliSecs) / 1000.0;
    _prevFrameMilliSecs = SDL_GetTicks();

    _registry->GetSystem<MovementSystem>().Update(deltaTime);

    // 레지스트리 업데이트(실제로 entity 생성 및 삭제가 일어남)
    _registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    _registry->GetSystem<RenderSystem>().Update(_renderer, _assetManager);

    SDL_RenderPresent(_renderer);
}
