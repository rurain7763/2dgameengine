#include "ECS.h"
#include "../Logger/Logger.h"

// Entity
Entity Entity::CreateEntity() const {
    return _registry->CreateEntity();
}

void Entity::Kill() const {
    _registry->KillEntity(*this);
}

void Entity::Tag(const std::string& tag) const {
    _registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const {
    return _registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string& group) const {
    _registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group) const {
    return _registry->EntityBelongsToGroup(*this, group);
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
    int entityID = -1;

    if (_freeEntityIDs.empty()) {
        entityID = _numEntities++;
        if(entityID >= _entityComponentSigs.size()) {
            _entityComponentSigs.resize(entityID + 1);
        }
    } else {
        entityID = _freeEntityIDs.front();
        _freeEntityIDs.pop_front();
    }

    Entity ret(this, entityID);
    _entitiesToAdded.insert(ret);
    LOG("Entity create with id = %d", entityID);

    return ret;
}

void Registry::KillEntity(const Entity& entity) {
    _entitiesToBeKilled.insert(entity);
    LOG("Entity %d killed!", entity.GetID());
}

void Registry::AddEntityToSystem(const Entity& entity) {
    const int entityID = entity.GetID();
    const Signature& sig = _entityComponentSigs[entityID];
    for(auto& pair : _systems) {
        const Signature& systemSig = pair.second->GetSignature();
        bool isInterested = (sig & systemSig) == systemSig;
        if(isInterested) {
            pair.second->AddEnity(entity);
        }
    }
}

void Registry::RemoveEntityFromSystem(const Entity& entity) {
    const int entityID = entity.GetID();
    const Signature& sig = _entityComponentSigs[entityID];
    for(auto& pair : _systems) {
        const Signature& systemSig = pair.second->GetSignature();
        bool isInterested = (sig & systemSig) == systemSig;
        if(isInterested) {
            pair.second->RemoveEntity(entity);
        }
    }
}

void Registry::TagEntity(const Entity& entity, const std::string& tag) {
    _entityPerTag.emplace(tag, entity);
    _tagPerEntities.emplace(entity.GetID(), tag);
}

bool Registry::EntityHasTag(const Entity& entity, const std::string& tag) const {
    auto pair = _tagPerEntities.find(entity.GetID());
    if(pair == _tagPerEntities.end()) return false;
    return pair->second == tag;
}

Entity Registry::GetEntityByTag(const std::string& tag) const {
    return _entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(const Entity& entity) {
    auto pair = _tagPerEntities.find(entity.GetID());
    if(pair != _tagPerEntities.end()) {
        _entityPerTag.erase(pair->second);
        _tagPerEntities.erase(pair->first);
    }
}

void Registry::GroupEntity(const Entity& entity, const std::string& group) {
    if(_entitiesPerGroup.find(group) == _entitiesPerGroup.end()) {
        _entitiesPerGroup.insert({group, std::set<Entity>()});
    }
    _entitiesPerGroup[group].insert(entity);

    _groupPerEntity.emplace(entity.GetID(), group);
}

bool Registry::EntityBelongsToGroup(const Entity& entity, const std::string& group) const {
    auto pair = _groupPerEntity.find(entity.GetID());
    if(pair == _groupPerEntity.end()) return false;
    return pair->second == group;
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const {
    std::vector<Entity> ret;

    auto pair = _entitiesPerGroup.find(group);
    if(pair != _entitiesPerGroup.end()) {
        for(auto& entity : pair->second) {
            ret.push_back(entity);
        }
    }

    return ret;
}

void Registry::RemoveEntityGroup(const Entity& entity) {
    auto pair = _groupPerEntity.find(entity.GetID());
    if(pair != _groupPerEntity.end()) {
        _entitiesPerGroup[pair->second].erase(entity);
        _groupPerEntity.erase(pair->first);
    }
}

void Registry::Update() {
    for(auto& entity : _entitiesToAdded) {
        AddEntityToSystem(entity);
    }
    _entitiesToAdded.clear();

    for(auto& entity : _entitiesToBeKilled) {
        const int entityID = entity.GetID();

        RemoveEntityFromSystem(entity);

        for(auto& pool : _componentPools) {
            if(pool) {
                pool->Remove(entityID);
            }
        }

        _entityComponentSigs[entityID].reset();
        _freeEntityIDs.push_back(entityID);

        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
    }
    _entitiesToBeKilled.clear();
}

void Registry::Clear() {
    _entityComponentSigs.clear();
    _freeEntityIDs.clear();
    _entitiesToAdded.clear();
    _entitiesToBeKilled.clear();
    _entityPerTag.clear();
    _tagPerEntities.clear();
    _entitiesPerGroup.clear();
    _groupPerEntity.clear();
    _systems.clear();
    _componentPools.clear();
}