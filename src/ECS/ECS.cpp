#include "ECS.h"
#include "../Logger/Logger.h"

// Entity
int Entity::GetID() const {
    return _id;
}

// IComponent
int IComponent::g_idCounter = 0;

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

// Registry
Entity Registry::CreateEntity() {
    const int entityID = _numEntities++;
    if(entityID >= _entityComponentSigs.size()) {
        _entityComponentSigs.resize(entityID + 1);
    }
    Entity ret(this, entityID);
    _entitiesToAdded.insert(ret);
    LOG("Entity create with id = %d", entityID);
    return ret;
}

void Registry::AddEntityToSystem(const Entity& entity) {
    const int entityID = entity.GetID();
    const Signature& sig = _entityComponentSigs[entityID];
    for(auto& pair : _systems) {
        const Signature& systemSig = pair.second->GetSignature();
        bool isInterested = (sig & systemSig) == sig;
        if(isInterested) {
            pair.second->AddEnity(entity);
        }
    }
}

void Registry::Update() {
    for(auto& entity : _entitiesToAdded) {
        AddEntityToSystem(entity);
    }
    _entitiesToAdded.clear();
}