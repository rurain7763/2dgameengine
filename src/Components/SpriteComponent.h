#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent {
public:
    std::string assetID;
    int width, height;
    int srcRectX, srcRectY;
    bool isFixed;
    int zIndex;
    SDL_RendererFlip flip;
    SDL_Color color;

    SpriteComponent(
        const std::string& assetID = "", 
        int width = 0, 
        int height = 0, 
        int zIndex = 0, 
        int srcRectX = 0, 
        int srcRectY = 0, 
        bool isFixed = false,
        SDL_Color color = {255, 255, 255}) 
    {
        this->assetID = assetID;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->srcRectX = srcRectX;
        this->srcRectY = srcRectY;
        this->isFixed = isFixed;
        this->color = color;
        flip = SDL_FLIP_NONE;
    }
};

#endif