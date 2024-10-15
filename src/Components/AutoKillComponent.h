#ifndef AUTOKILLCOMPONENT_H
#define AUTOKILLCOMPONENT_H

#include "../ECS/ECS.h"

#include <SDL2/SDL.h>

struct AutoKillComponent {
    int lifeTime;
    int startTime;

    AutoKillComponent(int lifeTime = 0) {
        this->lifeTime = lifeTime;
        this->startTime = SDL_GetTicks();
    }
};

#endif