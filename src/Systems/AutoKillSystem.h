#ifndef AUTOKILLSYSTEM_H
#define AUTOKILLSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/AutoKillComponent.h"

class AutoKillSystem : public System {
public:
    AutoKillSystem() {
        RequireComponent<AutoKillComponent>();
    }

    void Update() {
        for(auto entity : GetEntities()) {
            auto& autoKillComponent = entity.GetComponent<AutoKillComponent>();
            if(SDL_GetTicks() >= autoKillComponent.lifeTime + autoKillComponent.startTime) {
                entity.Kill();
            }
        }
    }
};

#endif