#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Logger/Logger.h"

class CollisionSystem : public System {
public:
    CollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update() {
        auto& entities = GetEntities();
        for(int i = 0; i < entities.size(); i++) {
            auto& transformA = entities[i].GetComponent<TransformComponent>();
            auto& boxColliderA = entities[i].GetComponent<BoxColliderComponent>();

            // y축이 스크린 좌표계이므로 반전임을 주의!
            float lA = transformA.position.x + boxColliderA.offset.x;
            float rA = lA + boxColliderA.width;
            float tA = transformA.position.y + boxColliderA.offset.y;
            float bA = tA + boxColliderA.height;

            for(int j = i + 1; j < entities.size(); j++) {
                auto& transformB = entities[j].GetComponent<TransformComponent>();
                auto& boxColliderB = entities[j].GetComponent<BoxColliderComponent>();

                // y축이 스크린 좌표계이므로 반전임을 주의!
                float lB = transformB.position.x + boxColliderB.offset.x;
                float rB = lB + boxColliderB.width;
                float tB = transformB.position.y + boxColliderB.offset.y;
                float bB = tB + boxColliderB.height;

                if(AABB(lA, rA, tA, bA, lB, rB, tB, bB)) {
                    LOG("id %d entity and id %d entity both are collided", entities[i].GetID(), entities[j].GetID());
                }
            }
        }
    }

    bool AABB(float lA, float rA, float tA, float bA, float lB, float rB, float tB, float bB) const {
        return rA > lB && lA < rB && bA > tB && tA < bB;
    }
};

#endif
