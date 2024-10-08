#include "Game.h"
#include "../Logger/Logger.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DebugRenderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../Systems/CameraMovementSystem.h"

#include <iostream>
#include <fstream>

Game::Game() 
    : _prevFrameMilliSecs(0)
{
    _registry = std::make_unique<Registry>();
    _assetManager = std::make_unique<AssetManager>();
    _eventBus = std::make_unique<EventBus>();
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
    _windowWidth = displayMod.w / 2;
    _windowHeight = displayMod.h / 2;

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
    
    //SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);

    _camera.x = 0;
    _camera.y = 0;
    _camera.w = _windowWidth;
    _camera.h = _windowHeight;

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
    _registry->AddSystem<MovementSystem>();
    _registry->AddSystem<AnimationSystem>();
    _registry->AddSystem<CollisionSystem>();
    _registry->AddSystem<RenderSystem>();
    _registry->AddSystem<DebugRenderSystem>();
    _registry->AddSystem<DamageSystem>();
    _registry->AddSystem<KeyboardMovementSystem>();
    _registry->AddSystem<CameraMovementSystem>();

    _assetManager->AddTexture(_renderer, "tank_image", "./assets/images/tank-tiger-right.png");
    _assetManager->AddTexture(_renderer, "truck_image", "./assets/images/truck-ford-right.png");
    _assetManager->AddTexture(_renderer, "chopper_spritesheet", "./assets/images/chopper-spritesheet.png");
    _assetManager->AddTexture(_renderer, "radar_image", "./assets/images/radar.png");
    _assetManager->AddTexture(_renderer, "jungle_map", "./assets/tilemaps/jungle.png");

    Entity tank = _registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10, 30), glm::vec2(1, 1), 0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(50, 0));
    tank.AddComponent<SpriteComponent>("tank_image", 32, 32, 2);
    tank.AddComponent<BoxColliderComponent>(32, 32);
    tank.AddComponent<DebugRenderComponent>(true);

    Entity truck = _registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(200, 30), glm::vec2(1, 1), 0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(-50, 0));
    truck.AddComponent<SpriteComponent>("truck_image", 32, 32, 1);
    truck.AddComponent<BoxColliderComponent>(32, 32);
    truck.AddComponent<DebugRenderComponent>(true);

    Entity chopper = _registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
    chopper.AddComponent<SpriteComponent>("chopper_spritesheet", 32, 32, 3);
    chopper.AddComponent<AnimationComponent>(2, 5, true);
    chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80), glm::vec2(-80, 0));
    chopper.AddComponent<CameraFollowComponent>();

    Entity radar = _registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2(_windowWidth - 74, _windowHeight - 74), glm::vec2(1, 1), 0);
    radar.AddComponent<SpriteComponent>("radar_image", 64, 64, 3, 0, 0, true);
    radar.AddComponent<AnimationComponent>(8, 3, true);

    std::fstream file("./assets/tilemaps/jungle.map");
    const float tileSize = 32.f;
    const float tileScale = 2.f;
    const int numMapStride = 10;
    int x = 0, y = 0;
    std::string line;
    while(std::getline(file, line)) {
        int pos;
        int idx;
        while((pos = line.find(',')) != std::string::npos) {
            idx = stoi(line.substr(0, pos));

            Entity tile = _registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(tileScale, tileScale), 0);
            tile.AddComponent<SpriteComponent>("jungle_map", tileSize, tileSize, 0, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);

            line.erase(0, pos + 1);
            x += tileSize * tileScale;
        }

        idx = stoi(line);

        Entity tile = _registry->CreateEntity();
        tile.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(tileScale, tileScale), 0);
        tile.AddComponent<SpriteComponent>("jungle_map", tileSize, tileSize, 0, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);
        x += tileSize * tileScale;
        _mapSize.x = x;
        x = 0;

        y += tileSize * tileScale;
        _mapSize.y = y;
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
                _eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
                break;
        }
    }
}

void Game::Update() {
    int timeToWait = (_prevFrameMilliSecs + MILLISECS_PER_FRAME) - SDL_GetTicks();
    if(timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - _prevFrameMilliSecs) / 1000.0;
    _prevFrameMilliSecs = SDL_GetTicks();

    _eventBus->Reset();
    _registry->GetSystem<DamageSystem>().SubscribeToEvents(_eventBus);
    _registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(_eventBus);

    _registry->GetSystem<MovementSystem>().Update(deltaTime);
    _registry->GetSystem<CollisionSystem>().Update(_eventBus);
    _registry->GetSystem<AnimationSystem>().Update();
    _registry->GetSystem<CameraMovementSystem>().Update(_camera, _mapSize);

    // 레지스트리 업데이트(실제로 entity 생성 및 삭제가 일어남)
    _registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    _registry->GetSystem<RenderSystem>().Update(_renderer, _assetManager, _camera);
    _registry->GetSystem<DebugRenderSystem>().Update(_renderer);

    SDL_RenderPresent(_renderer);
}
