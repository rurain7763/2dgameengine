#include "Game.h"
#include "../Logger/Logger.h"
#include "LevelLoader.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_sdl.h"
#include "../../libs/imgui/imgui_impl_sdl.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DebugRenderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifeCycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/HealthUISystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../Systems/ScriptSystem.h"
#include "../Systems/AudioSystem.h"
#include "../Systems/AutoKillSystem.h"

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
    // sdl 초기화
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        LOG_ERR("error init SDL");
        return;
    }

    if(TTF_Init() != 0) {
        LOG_ERR("error init TTF");
        return;
    }

    if(SDL_Init(SDL_INIT_AUDIO) != 0) {
        LOG_ERR("error init SDL audio");
        return;
    }

    // 기본 오디오 플레이어 오픈
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        LOG_ERR("error init Mix_OpenAudio");
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

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_TARGETTEXTURE);
    if(!_renderer) {
        Logger::Err("Error creating renderer");
        return;
    }

    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);

    // imgui 초기화
    ImGui::CreateContext();
    ImGuiSDL::Initialize(_renderer, _windowWidth, _windowHeight);

    _gameTexure = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _windowWidth, _windowHeight);
    _engineTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _windowWidth, _windowHeight);

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
    _registry->Clear();
    _assetManager->ClearAssets();

    SDL_DestroyTexture(_gameTexure);
    SDL_DestroyTexture(_engineTexture);

    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();

    Mix_CloseAudio();
    TTF_Quit();
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
    _registry->AddSystem<ProjectileEmitSystem>();
    _registry->AddSystem<ProjectileLifeCycleSystem>();
    _registry->AddSystem<RenderTextSystem>();
    _registry->AddSystem<HealthUISystem>();
    _registry->AddSystem<RenderGUISystem>();
    _registry->AddSystem<ScriptSystem>();
    _registry->AddSystem<AudioSystem>();
    _registry->AddSystem<AutoKillSystem>();

    _lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
    _registry->GetSystem<ScriptSystem>().CreateLuaBindings(_lua);

    LevelLoader loader;
    glm::vec2 mapSize = loader.LoadLevel(_lua, _registry, _assetManager, _renderer, _windowWidth, _windowHeight, 1);
    _mapSize = mapSize;
}

void Game::ProcessInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        // SDL input 처리
        ImGui_ImplSDL2_ProcessEvent(&event);
        ImGuiIO& io = ImGui::GetIO();

        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        // game input 처리
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

    _registry->GetSystem<MovementSystem>().SubscribeToEvents(_eventBus);
    _registry->GetSystem<DamageSystem>().SubscribeToEvents(_eventBus);
    _registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(_eventBus);
    _registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(_eventBus);

    _registry->GetSystem<MovementSystem>().Update(deltaTime, _mapSize);
    _registry->GetSystem<CollisionSystem>().Update(_eventBus);
    _registry->GetSystem<AnimationSystem>().Update();
    _registry->GetSystem<ProjectileLifeCycleSystem>().Update();
    _registry->GetSystem<ProjectileEmitSystem>().Update(_registry);
    _registry->GetSystem<HealthUISystem>().Update(_registry, _renderer);
    _registry->GetSystem<ScriptSystem>().Update(deltaTime);
    _registry->GetSystem<AutoKillSystem>().Update();

    _registry->GetSystem<CameraMovementSystem>().Update(_camera, _mapSize);

    _registry->GetSystem<AudioSystem>().Update(_assetManager);

    // 레지스트리 업데이트(실제로 entity 생성 및 삭제가 일어남)
    _registry->Update();
}

void Game::Render() {
    SDL_SetRenderTarget(_renderer, _gameTexure);

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    _registry->GetSystem<RenderSystem>().Update(_renderer, _assetManager, _camera);
    _registry->GetSystem<RenderTextSystem>().Update(_renderer, _assetManager, _camera);
    _registry->GetSystem<DebugRenderSystem>().Update(_renderer, _camera);

    SDL_SetRenderTarget(_renderer, _engineTexture);

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    _registry->GetSystem<RenderGUISystem>().Update(_registry, _assetManager, _camera, _gameTexure);

    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_RenderCopy(_renderer, _engineTexture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
}


