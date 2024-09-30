#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>

struct SpriteComponent {
public:
    std::string assetID;
    int width, height;
    int srcRectX, srcRectY;

    SpriteComponent(const std::string& assetID = "", int width = 0, int height = 0, int srcRectX = 0, int srcRectY = 0) {
        this->assetID = assetID;
        this->width = width;
        this->height = height;
        this->srcRectX = srcRectX;
        this->srcRectY = srcRectY;
    }
};

#endif