#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"

#include "SDL2/SDL.h"

class KeyboardMovementSystem : public System {
public:
    KeyboardMovementSystem() {
        RequireComponent<KeyboardControlledComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->Subscribe<KeyboardMovementSystem, KeyPressedEvent>(this, &KeyboardMovementSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event) {
        for(auto& entity : GetEntities()) {
            auto& keyboardCtrl = entity.GetComponent<KeyboardControlledComponent>();
            auto& sprite = entity.GetComponent<SpriteComponent>();
            auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

            switch(event.keyCode) {
                case SDLK_UP:
                    rigidbody.velocity = keyboardCtrl.upVelocity;
                    sprite.srcRectY = sprite.height * 0;
                    break;
                case SDLK_RIGHT:
                    rigidbody.velocity = keyboardCtrl.rightVelocity;
                    sprite.srcRectY = sprite.height * 1;
                    break;
                case SDLK_DOWN:
                    rigidbody.velocity = keyboardCtrl.downVelocity;
                    sprite.srcRectY = sprite.height * 2;
                    break;
                case SDLK_LEFT:
                    rigidbody.velocity = keyboardCtrl.leftVelocity;
                    sprite.srcRectY = sprite.height * 3;
                    break;
            }
        }
    }
};

#endif