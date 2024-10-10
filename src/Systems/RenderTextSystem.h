#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Logger/Logger.h"

#include <SDL2/SDL.h>

class RenderTextSystem : public System {
public:
    RenderTextSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<TextLabelComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetMg, SDL_Rect& camera) {
        for(auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& label = entity.GetComponent<TextLabelComponent>();

            TTF_Font* font = assetMg->GetFont(label.assetID);
            SDL_Surface* surface = TTF_RenderText_Blended(font, label.text.c_str(), label.color);

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int textureW = 0, textureH = 0;
            SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);

            SDL_Rect dstRt = {
                .x = static_cast<int>(transform.position.x - (label.isFixed ? 0 : camera.x)),
                .y = static_cast<int>(transform.position.y - (label.isFixed ? 0 : camera.y)),
                .w = static_cast<int>(textureW * transform.scale.x),
                .h = static_cast<int>(textureH * transform.scale.y)
            };

            SDL_RenderCopy(renderer, texture, NULL, &dstRt);
        }
    }
};

#endif