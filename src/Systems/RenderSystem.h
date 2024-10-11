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

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera) {
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
                static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            SDL_Texture* texture = assetManager->GetTexture(sprite.assetID);

            if(texture) {
                SDL_SetTextureColorMod(texture, sprite.color.r, sprite.color.g, sprite.color.b);
                SDL_RenderCopyEx(
                    renderer, 
                    texture,    
                    &srcRt,
                    &dstRt,
                    transform.rotation,
                    NULL,
                    sprite.flip
                );
            } else { 
                SDL_SetRenderDrawColor(renderer, sprite.color.r, sprite.color.g, sprite.color.b, 255);
                SDL_RenderFillRect(renderer, &dstRt);
            }
        }
    }
};

#endif