#ifndef DEBUGRENDERSYSTEM_H
#define DEBUGRENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/DebugRenderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

#include <SDL2/SDL.h>

class DebugRenderSystem : public System {
public: 
    DebugRenderSystem() {
        RequireComponent<DebugRenderComponent>();
    }

    void Update(SDL_Renderer* renderer, SDL_Rect& camera) {
        for(auto& entity : GetEntities()) {
            auto& debug = entity.GetComponent<DebugRenderComponent>();
            if(debug.isRenderBoxCollider) {
                RenderBoxCollider(renderer, entity, camera);
            }
        }
    }

    void RenderBoxCollider(SDL_Renderer* renderer, const Entity& entity, SDL_Rect& camera) {
        if(!entity.HasComponent<TransformComponent>() || !entity.HasComponent<BoxColliderComponent>()) return;

        auto& transform = entity.GetComponent<TransformComponent>();
        auto& boxCollider = entity.GetComponent<BoxColliderComponent>();

        SDL_Rect rt;
        rt.x = static_cast<int>(transform.position.x - camera.x + boxCollider.offset.x);
        rt.y = static_cast<int>(transform.position.y - camera.y + boxCollider.offset.y);
        rt.w = boxCollider.width;
        rt.h = boxCollider.height;

        if(boxCollider.collidedCount) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        }

        SDL_RenderDrawRect(renderer, &rt);
    }
};

#endif