#ifndef GAME_H
#define GAME_H

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventBus/EventBus.h"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Run();
    void Destroy();
    
private:
    void Setup();
    void ProcessInput();
    void Update();
    void Render();

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Rect _camera;
    glm::vec2 _mapSize;

    bool _isRunning;
    int _windowWidth, _windowHeight;

    int _prevFrameMilliSecs;

    std::unique_ptr<Registry> _registry;
    std::unique_ptr<AssetManager> _assetManager;
    std::unique_ptr<EventBus> _eventBus;
};

#endif