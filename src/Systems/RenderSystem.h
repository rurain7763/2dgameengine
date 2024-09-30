#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>

class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager) {
        for(auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect srcRt = {
                sprite.srcRectX,
                sprite.srcRectY,
                sprite.width,
                sprite.height
            };

            SDL_Rect dstRt = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            SDL_RenderCopyEx(
                renderer, 
                assetManager->GetTexture(sprite.assetID),
                &srcRt,
                &dstRt,
                transform.rotation,
                NULL,
                SDL_FLIP_NONE
            );
        }
    }
};

#endif