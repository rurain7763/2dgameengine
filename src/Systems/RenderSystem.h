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
        // zIndex 순으로 정렬
        struct Entry {
            TransformComponent* transform;
            SpriteComponent* sprite;
        };

        std::vector<Entry> sortedEntities;
        for(auto& entity : GetEntities()) {
            Entry entry = {
                .transform = &entity.GetComponent<TransformComponent>(),
                .sprite = &entity.GetComponent<SpriteComponent>()
            };
            sortedEntities.push_back(entry);
        }
        std::sort(sortedEntities.begin(), sortedEntities.end(), [](const Entry& a, const Entry& b) { 
            return a.sprite->zIndex < b.sprite->zIndex; 
        });
        
        // 그리기
        for(Entry& entry : sortedEntities) {
            auto& transform = *entry.transform;
            auto& sprite = *entry.sprite;

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