#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

struct ProjectileEmitterComponent {
    glm::vec2 projectileVelocity;
    int repeatFrequnecy;
    int projectileDuration; // projectile life time
    int percentDamage;
    bool isFriendly;
    int lastEmissionTime;

    ProjectileEmitterComponent(
        glm::vec2 projectileVelocity = glm::vec2(0), 
        int repeatFrequency = 0,
        int projectileDuration = 10000,
        int percentDamage = 10,
        bool isFriendly = false) 
    {
        this->projectileVelocity = projectileVelocity;
        this->repeatFrequnecy = repeatFrequency;
        this->projectileDuration = projectileDuration;
        this->percentDamage = percentDamage;
        this->isFriendly = isFriendly;
        lastEmissionTime = SDL_GetTicks();
    }
};

#endif