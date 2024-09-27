#ifndef GAME_H
#define GAME_H

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Run();
    void Destroy();
    
private:
    void ProcessInput();
    void Update();
    void Render();

private:
    struct SDL_Window* _window;
    struct SDL_Renderer* _renderer;

    bool _isRunning;
};

#endif