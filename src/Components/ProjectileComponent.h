#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <SDL2/SDL.h>

struct ProjectileComponent {
    bool isFriendly;
    int percentDamage;
    int duration;
    int startTime;

    ProjectileComponent(bool isFriendly = false, int percentDamage = 0, int duration = 0) {
        this->isFriendly = isFriendly;
        this->percentDamage = percentDamage;
        this->duration = duration;
        startTime = SDL_GetTicks();
    }
};

#endif