#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/ScriptComponent.h"
#include "../Components/TransformComponent.h"

#include "../../libs/sol/sol.hpp"

class ScriptSystem : public System {
public:
    ScriptSystem() {
        RequireComponent<ScriptComponent>();
    }

    void CreateLuaBindings(sol::state& lua) {
        lua["log_info"] = Logger::Log;

        // lua가 알 수 있게 user_type을 등록
        lua.new_usertype<Entity>(
            "entity", 
            "get_id", &Entity::GetID,
            "destroy", &Entity::Kill,
            "has_tag", &Entity::HasTag,
            "belongs_to_group", &Entity::BelongsToGroup
        );

        // lua가 알 수 있게 c++ function을 등록
        lua.set_function("get_position", GetEntityPosition);
        lua.set_function("get_velocity", GetEntityVelocity);
        lua.set_function("set_position", SetEntityPosition);
        lua.set_function("set_velocity", SetEntityVelocity);
        lua.set_function("set_rotation", SetEntityRotation);
        lua.set_function("set_projectile_velocity", SetProjectileVelocity);
        lua.set_function("set_animation_frame", SetEntityAnimationFrame);
    }

    void Update(float deltaTime) {
        for(auto entity : GetEntities()) {
            auto& script = entity.GetComponent<ScriptComponent>();
            script.func(entity, deltaTime, SDL_GetTicks());
        }
    }

private:
    static std::tuple<double, double> GetEntityPosition(Entity entity) {
        if (entity.HasComponent<TransformComponent>()) {
            const auto transform = entity.GetComponent<TransformComponent>();
            return std::make_tuple(transform.position.x, transform.position.y);
        } else {
            Logger::Err("Trying to get the position of an entity that has no transform component");
            return std::make_tuple(0.0, 0.0);
        }
    }

    static std::tuple<double, double> GetEntityVelocity(Entity entity) {
        if (entity.HasComponent<RigidBodyComponent>()) {
            const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
            return std::make_tuple(rigidbody.velocity.x, rigidbody.velocity.y);
        } else {
            Logger::Err("Trying to get the velocity of an entity that has no rigidbody component");
            return std::make_tuple(0.0, 0.0);
        }
    }

    static void SetEntityPosition(Entity entity, double x, double y) {
        if (entity.HasComponent<TransformComponent>()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            transform.position.x = x;
            transform.position.y = y;
        } else {
            Logger::Err("Trying to set the position of an entity that has no transform component");
        }
    }

    static void SetEntityVelocity(Entity entity, double x, double y) {
        if (entity.HasComponent<RigidBodyComponent>()) {
            auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
            rigidbody.velocity.x = x;
            rigidbody.velocity.y = y;
        } else {
            Logger::Err("Trying to set the velocity of an entity that has no rigidbody component");
        }
    }

    static void SetEntityRotation(Entity entity, double angle) {
        if (entity.HasComponent<TransformComponent>()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            transform.rotation = angle;
        } else {
            Logger::Err("Trying to set the rotation of an entity that has no transform component");
        }
    }

    static void SetEntityAnimationFrame(Entity entity, int frame) {
        if (entity.HasComponent<AnimationComponent>()) {
            auto& animation = entity.GetComponent<AnimationComponent>();
            animation.currentFrame = frame;
        } else {
            Logger::Err("Trying to set the animation frame of an entity that has no animation component");
        }
    }

    static void SetProjectileVelocity(Entity entity, double x, double y) {
        if (entity.HasComponent<ProjectileEmitterComponent>()) {
            auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
            projectileEmitter.projectileVelocity.x = x;
            projectileEmitter.projectileVelocity.y = y;
        } else {
            Logger::Err("Trying to set the projectile velocity of an entity that has no projectile emitter component");
        }
    }
};

#endif