#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"

#include "SDL2/SDL.h"

class KeyboardMovementSystem : public System {
public:
    KeyboardMovementSystem() {

    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->Subscribe<KeyboardMovementSystem, KeyPressedEvent>(this, &KeyboardMovementSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event) {
        LOG("%s key was pressed!", SDL_GetKeyName(event.keyCode));
    }
};

#endif