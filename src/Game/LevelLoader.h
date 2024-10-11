#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"

#include <SDL2/SDL.h>
#include <memory.h>
#include <glm/glm.hpp>

class LevelLoader {
public:
    LevelLoader();
    ~LevelLoader();

    glm::vec2 LoadLevel(
        std::unique_ptr<Registry>& registry, 
        std::unique_ptr<AssetManager>& assetMg, 
        SDL_Renderer* renderer, 
        int windowWidth,
        int windowHeight,
        int level);
};

#endif