#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

struct BoxColliderComponent {
    int width, height;
    glm::vec2 offset;
    int collidedCount;

    BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0, 0)) {
        this->width = width;
        this->height = height;
        this->offset = offset;
        
        collidedCount = 0;
    }
};

#endif