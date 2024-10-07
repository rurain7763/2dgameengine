#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem : public System {
public:
    CollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update(std::unique_ptr<EventBus>& eventBus) {
        auto& entities = GetEntities();

        for(auto& entity : entities) {
            auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
            boxCollider.collidedCount = 0;
        }

        for(int i = 0; i < entities.size(); i++) {
            const Entity& a = entities[i];

            auto& transformA = a.GetComponent<TransformComponent>();
            auto& boxColliderA = a.GetComponent<BoxColliderComponent>();

            // y축이 스크린 좌표계이므로 반전임을 주의!
            float lA = transformA.position.x + boxColliderA.offset.x;
            float rA = lA + boxColliderA.width;
            float tA = transformA.position.y + boxColliderA.offset.y;
            float bA = tA + boxColliderA.height;

            for(int j = i + 1; j < entities.size(); j++) {
                const Entity& b = entities[j];

                auto& transformB = b.GetComponent<TransformComponent>();
                auto& boxColliderB = b.GetComponent<BoxColliderComponent>();

                // y축이 스크린 좌표계이므로 반전임을 주의!
                float lB = transformB.position.x + boxColliderB.offset.x;
                float rB = lB + boxColliderB.width;
                float tB = transformB.position.y + boxColliderB.offset.y;
                float bB = tB + boxColliderB.height;

                if(AABB(lA, rA, tA, bA, lB, rB, tB, bB)) {
                    boxColliderA.collidedCount++; 
                    boxColliderB.collidedCount++;
                    eventBus->EmitEvent<CollisionEvent>(a, b);
                }
            }
        }
    }

    bool AABB(float lA, float rA, float tA, float bA, float lB, float rB, float tB, float bB) const {
        return rA > lB && lA < rB && bA > tB && tA < bB;
    }
};

#endif
