#include "ECS.h"

// Entity
int Entity::GetID() const {
    return _id;
}

// System
void System::AddEnity(const Entity& entity) {
    _entities.push_back(entity);
}

void System::RemoveEntity(const Entity& entity) {
    for(int i = 0; i < _entities.size(); i++) {
        if(_entities[i] == entity) {
            _entities[i] = _entities.back();
            _entities.pop_back();
            break;
        }
    }
}

const std::vector<Entity>& System::GetEntities() const {
    return _entities;
}

const Signature& System::GetSignature() const {
    return _signature;
}