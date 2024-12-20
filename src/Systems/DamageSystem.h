#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/AudioComponent.h"
#include "../Components/AutoKillComponent.h"

class DamageSystem : public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->Subscribe<DamageSystem, CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        Entity& a = event.entityA;
        Entity& b = event.entityB;

        if(a.BelongsToGroup("projectiles") && b.HasTag("player")) {
            OnProjectileHitPlayer(b, a);
        } else if(b.BelongsToGroup("projectiles") && a.HasTag("player")) {
            OnProjectileHitPlayer(a, b);
        } else if(a.BelongsToGroup("enemies") && b.BelongsToGroup("projectiles")) {
            OnProjectileHitEnemy(a, b);
        } else if(b.BelongsToGroup("enemies") && a.BelongsToGroup("projectiles")) {
            OnProjectileHitEnemy(b, a);
        }
    }

    void OnProjectileHitPlayer(Entity& player, Entity& projectile) {
        auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();
        if(!projectileComponent.isFriendly) {
            auto& health = player.GetComponent<HealthComponent>();
            health.curHealthPercentage -= projectileComponent.percentDamage;
            if(health.curHealthPercentage <= 0) {
                player.Kill();

                Entity explosion = player.CreateEntity();
                explosion.AddComponent<TransformComponent>(player.GetComponent<TransformComponent>().position, glm::vec2(1, 1), 0);
                explosion.AddComponent<AudioComponent>("explosion-audio", true, 128);
                explosion.AddComponent<AutoKillComponent>(1000);
            }
            projectile.Kill();
        }
    }

    void OnProjectileHitEnemy(Entity& enemy, Entity& projectile) {
        auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();
        if(projectileComponent.isFriendly) {
            auto& health = enemy.GetComponent<HealthComponent>();
            health.curHealthPercentage -= projectileComponent.percentDamage;
            if(health.curHealthPercentage <= 0) {
                enemy.Kill();

                Entity explosion = enemy.CreateEntity();
                explosion.AddComponent<TransformComponent>(enemy.GetComponent<TransformComponent>().position, glm::vec2(1, 1), 0);
                explosion.AddComponent<AudioComponent>("explosion-audio", true, 128);
                explosion.AddComponent<AutoKillComponent>(1000);
            }
            projectile.Kill();
        }
    }
};

#endif