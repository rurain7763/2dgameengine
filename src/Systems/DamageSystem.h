#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"

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

        LOG("damage system recieved %d entity and %d entity are collided!", a.GetID(), b.GetID());
    }

    void OnProjectileHitPlayer(Entity& player, Entity& projectile) {
        auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();
        if(!projectileComponent.isFriendly) {
            auto& health = player.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.percentDamage;
            if(health.healthPercentage <= 0) {
                player.Kill();
            }
            projectile.Kill();
        }
    }

    void OnProjectileHitEnemy(Entity& enemy, Entity& projectile) {
        auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();
        if(projectileComponent.isFriendly) {
            auto& health = enemy.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.percentDamage;
            if(health.healthPercentage <= 0) {
                enemy.Kill();
            }
            projectile.Kill();
        }
    }
};

#endif