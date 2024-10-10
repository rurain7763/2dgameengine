#ifndef HEALTHUISYSTEM_H
#define HEALTHUISYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/HealthUIComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL2/SDL.h>

class HealthUISystem : public System {
public:
    HealthUISystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<HealthComponent>();
        RequireComponent<HealthUIComponent>();
    }

    void Update(std::unique_ptr<Registry>& registry, SDL_Renderer* renderer) {
        for(auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& health = entity.GetComponent<HealthComponent>();
            auto& healthUI = entity.GetComponent<HealthUIComponent>();

            int hpLvColor = 0;
            if(health.healthPercentage > 70) {
                hpLvColor = 2;
            } else if (health.healthPercentage > 30) {
                hpLvColor = 1;
            }

            if(!healthUI.hpTxt.IsValid()) {
                healthUI.hpTxt = registry->CreateEntity();
                healthUI.hpTxt.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(0.5, 0.5), 0);
                healthUI.hpTxt.AddComponent<TextLabelComponent>("0%", "charriot_font", SDL_Color{255, 0, 0}, false);
            }

            auto& hpTxtTransform = healthUI.hpTxt.GetComponent<TransformComponent>();
            auto& hpTxtLabel = healthUI.hpTxt.GetComponent<TextLabelComponent>();

            hpTxtTransform.position = transform.position + healthUI.offset;

            hpTxtLabel.text = std::to_string(health.healthPercentage);
            hpTxtLabel.color = HP_LV_COLOR[hpLvColor];

            if(!healthUI.hpBar.IsValid()) {
                healthUI.hpBar = registry->CreateEntity();
                healthUI.hpBar.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 0);
                healthUI.hpBar.AddComponent<SpriteComponent>("", MAX_BAR_SIZE.x, MAX_BAR_SIZE.y, 5, 0);
            }

            auto& hpBarTransform = healthUI.hpBar.GetComponent<TransformComponent>();
            auto& hpBarSprite = healthUI.hpBar.GetComponent<SpriteComponent>();

            hpBarTransform.position = transform.position + glm::vec2(healthUI.offset.x, healthUI.offset.y + 10);
            hpBarSprite.width = static_cast<int>(MAX_BAR_SIZE.x / 100.f * health.healthPercentage);
            hpBarSprite.color = HP_LV_COLOR[hpLvColor];

            if(health.healthPercentage <= 0) {
                healthUI.hpTxt.Kill();
                healthUI.hpBar.Kill();
            }
        }
    }

private:
    const SDL_Color HP_LV_COLOR[3] = {
        SDL_Color{255, 0, 0}, SDL_Color{255, 255, 0}, SDL_Color{0, 255, 0}
    };

    const glm::vec2 MAX_BAR_SIZE = glm::vec2(30, 4); 
};

#endif