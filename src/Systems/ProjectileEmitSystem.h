#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/AudioComponent.h"

#include <SDL2/SDL.h>

class ProjectileEmitSystem : public System {
public:
    ProjectileEmitSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<ProjectileEmitterComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->Subscribe<ProjectileEmitSystem, KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event) {
        switch(event.keyCode) {
            case SDLK_SPACE:
                for(auto& entity : GetEntities()) {
                    if(!entity.HasTag("player")) continue;

                    auto& transform = entity.GetComponent<TransformComponent>();
                    auto& projectileEmit = entity.GetComponent<ProjectileEmitterComponent>();

                    glm::vec2 initPos = transform.position;
                    if(entity.HasComponent<SpriteComponent>()) {
                        auto& sprite = entity.GetComponent<SpriteComponent>();
                        initPos.x += (sprite.width * transform.scale.x) / 2;
                        initPos.y += (sprite.height * transform.scale.y) / 2;
                    }

                    Entity projectile = entity.CreateEntity();
                    projectile.Group("projectiles");
                    projectile.AddComponent<TransformComponent>(initPos, glm::vec2(1, 1), 0);
                    projectile.AddComponent<RigidBodyComponent>(projectileEmit.projectileVelocity * transform.forward);
                    projectile.AddComponent<BoxColliderComponent>(4, 4);
                    projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
                    projectile.AddComponent<ProjectileComponent>(projectileEmit.isFriendly, projectileEmit.percentDamage, projectileEmit.projectileDuration);
                    projectile.AddComponent<AudioComponent>("shoot-audio", true);

                    projectileEmit.lastEmissionTime = SDL_GetTicks();
                }
                break;
        }
    }

    void Update(std::unique_ptr<Registry>& registry) {
        for(auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& projectileEmit = entity.GetComponent<ProjectileEmitterComponent>();

            if(projectileEmit.repeatFrequnecy <= 0) continue;

            if(SDL_GetTicks() - projectileEmit.lastEmissionTime > projectileEmit.repeatFrequnecy) {
                glm::vec2 initPos = transform.position;
                if(entity.HasComponent<SpriteComponent>()) {
                    auto& sprite = entity.GetComponent<SpriteComponent>();
                    initPos.x += (sprite.width * transform.scale.x) / 2;
                    initPos.y += (sprite.height * transform.scale.y) / 2;
                }
                
                Entity projectile = registry->CreateEntity();
                projectile.Group("projectiles");
                projectile.AddComponent<TransformComponent>(initPos, glm::vec2(1, 1), 0);
                projectile.AddComponent<RigidBodyComponent>(projectileEmit.projectileVelocity);
                projectile.AddComponent<BoxColliderComponent>(4, 4);
                projectile.AddComponent<SpriteComponent>("bullet_image", 4, 4, 4);
                projectile.AddComponent<ProjectileComponent>(projectileEmit.isFriendly, projectileEmit.percentDamage, projectileEmit.projectileDuration);

                projectileEmit.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};

#endif