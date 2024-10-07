#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event {
public:
    CollisionEvent(const Entity& entityA, const Entity& entityB) : entityA(entityA), entityB(entityB) {}

    Entity entityA;
    Entity entityB;
};

#endif