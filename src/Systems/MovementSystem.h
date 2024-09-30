#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"

class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void Update(float deltaTime) {
        for(auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

            transform.position += rigidbody.velocity * deltaTime;
            LOG("Entity id %d is now at x : %f y : %f", entity.GetID(), transform.position.x, transform.position.y);
        }
    }
};

#endif