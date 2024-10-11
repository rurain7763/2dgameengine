#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Events/CollisionEvent.h"

#include <glm/glm.hpp>

class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->Subscribe<MovementSystem, CollisionEvent>(this, &MovementSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        Entity& a = event.entityA;
        Entity& b = event.entityB;

        if(a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
            OnEnemyHitObstacle(a, b);
        } else if(b.BelongsToGroup("enemies") && a.BelongsToGroup("obstacles")) {
            OnEnemyHitObstacle(b, a);
        }
    }

    void OnEnemyHitObstacle(Entity& enemy, Entity& obstacle) {
        if(!enemy.HasComponent<RigidBodyComponent>()) return;

        auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();
        SpriteComponent* sprite = nullptr;
        if(enemy.HasComponent<SpriteComponent>()) {
            sprite = &enemy.GetComponent<SpriteComponent>();
        }
        if(rigidBody.velocity.x != 0) {
            rigidBody.velocity.x *= -1;
            if(sprite) {
                sprite->flip = sprite->flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            }
        }
        if(rigidBody.velocity.y != 0) {
            rigidBody.velocity.y *= -1;
            if(sprite) {
                sprite->flip = sprite->flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
            }
        }
    }

    void Update(float deltaTime, glm::vec2& mapSize) {
        for(auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

            transform.position += rigidbody.velocity * deltaTime;

            if(entity.HasTag("player")) {
                auto& sprite = entity.GetComponent<SpriteComponent>();
                transform.position.x = glm::clamp(transform.position.x, 0.f, mapSize.x - sprite.width * transform.scale.x);
                transform.position.y = glm::clamp(transform.position.y, 0.f, mapSize.y - sprite.height * transform.scale.y);
            } else {
                bool isEntityOutSideTheMap = 
                    transform.position.x < 0 ||
                    transform.position.x > mapSize.x ||
                    transform.position.y < 0 ||
                    transform.position.y > mapSize.y;

                if(isEntityOutSideTheMap) {
                    entity.Kill();
                }
            }
        }
    }
};

#endif