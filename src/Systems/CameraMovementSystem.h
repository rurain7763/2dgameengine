#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"

#include <SDL2/SDL.h>

class CameraMovementSystem : public System {
public:
    CameraMovementSystem() {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(SDL_Rect& camera, glm::vec2& mapSize) {
        const float halfW = camera.w / 2;
        const float halfH = camera.h / 2;

        for(auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();

            if(transform.position.x > halfW && transform.position.x < mapSize.x - halfW) {
                camera.x = transform.position.x - halfW;
            }

            if(transform.position.y > halfH && transform.position.y < mapSize.y - halfH) {
                camera.y = transform.position.y - halfH;
            }
        }
    }
};

#endif