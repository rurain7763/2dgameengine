#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

#include <SDL2/SDL.h>

class ProjectileLifeCycleSystem : public System {
public:
    ProjectileLifeCycleSystem() {
        RequireComponent<ProjectileComponent>();
    }

    void Update() {
        for(auto& entity : GetEntities()) {
            auto& projectile = entity.GetComponent<ProjectileComponent>();
            if(SDL_GetTicks() > projectile.startTime + projectile.duration) {
                entity.Kill();
            }
        }
    }
};

#endif