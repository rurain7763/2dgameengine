#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ScriptComponent.h"

#include "../../libs/sol/sol.hpp"

class ScriptSystem : public System {
public:
    ScriptSystem() {
        RequireComponent<ScriptComponent>();
    }

    void Update() {
        for(auto entity : GetEntities()) {
            auto& script = entity.GetComponent<ScriptComponent>();
            script.func();
        }
    }
};

#endif