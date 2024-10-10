#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <string>
#include <SDL2/SDL.h>

struct TextLabelComponent {
    std::string text;
    std::string assetID;
    SDL_Color color;
    bool isFixed;

    TextLabelComponent(
        const std::string& text = "", 
        const std::string& assetID = "", 
        const SDL_Color& color = {255, 255, 255},
        bool isFixed = true) 
    {
        this->text = text;
        this->assetID = assetID;
        this->color = color;
        this->isFixed = isFixed;
    }
};

#endif