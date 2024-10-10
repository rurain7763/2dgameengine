#ifndef HEALTHUICOMPONENT_H
#define HEALTHUICOMPONENT_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/SpriteComponent.h"

#include <glm/glm.hpp>

struct HealthUIComponent {
    glm::vec2 offset;

    Entity hpTxt;
    Entity hpBar;

    HealthUIComponent(const glm::vec2& offset = glm::vec2(0)) {
        this->offset = offset;
    }
};

#endif