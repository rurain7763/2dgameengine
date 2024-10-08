#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>

struct SpriteComponent {
public:
    std::string assetID;
    int width, height;
    int srcRectX, srcRectY;
    bool isFixed;
    int zIndex;

    SpriteComponent(const std::string& assetID = "", int width = 0, int height = 0, int zIndex = 0, int srcRectX = 0, int srcRectY = 0, bool isFixed = false) {
        this->assetID = assetID;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->srcRectX = srcRectX;
        this->srcRectY = srcRectY;
        this->isFixed = isFixed;
    }
};

#endif