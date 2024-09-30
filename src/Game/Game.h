#ifndef GAME_H
#define GAME_H

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
    struct SDL_Window* _window;
    struct SDL_Renderer* _renderer;

    bool _isRunning;
    int _windowWidth, _windowHeight;

    int _prevFrameMilliSecs;

    class Registry* _registry;
};

#endif