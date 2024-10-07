#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../EventBus/Event.h"
#include "SDL2/SDL.h"

class KeyPressedEvent : public Event {
public:
    KeyPressedEvent(SDL_Keycode keyCode) : keyCode(keyCode) {}
    
    SDL_Keycode keyCode;
};

#endif