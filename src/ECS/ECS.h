#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"

#include <bitset>
#include <vector>
#include <set>
#include <unordered_map>
#include <deque>
#include <typeindex>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity {
public:
    Entity(class Registry* registry, int id) : _registry(registry), _id(id) {}
    Entity(const Entity& other) = default;

    inline int GetID() const { return _id; }

    template<typename T, typename ...TArgs> void AddComponent(TArgs&& ...args) const;
    template<typename T> void RemoveComponent() const;
    template<typename T> bool HasComponent() const;
    template<typename T> T& GetComponent() const;

    Entity CreateEntity() const;
    void Kill() const;

    void Tag(const std::string& tag) const;
    bool HasTag(const std::string& tag) const;

    void Group(const std::string& group) const;
    bool BelongsToGroup(const std::string& group) const;

    Entity& operator=(const Entity& other) = default;
    bool operator==(const Entity& other) const { return _id == other.GetID(); }
    bool operator!=(const Entity& other) const { return _id != other.GetID(); }
    bool operator>(const Entity& other) const { return _id > other.GetID(); }
    bool operator<(const Entity& other) const { return _id < other.GetID(); }

private:
    class Registry* _registry;
    int _id;
};

class IComponent {
protected:
    static int g_idCounter;
};

template <typename T>
class Component : public IComponent {
public:
    static int GetID() {
        static int id = g_idCounter++;
        return id;
    }
};

class System {
public:
    System() = default;
    virtual ~System() = default;

    void AddEnity(const Entity& entity);
    void RemoveEntity(const Entity& entity);
    const std::vector<Entity>& GetEntities() const;
    const Signature& GetSignature() const;

    template <typename T>
    void RequireComponent() {
        const int componentID = Component<T>::GetID();
        _signature.set(componentID, true);
    }

private:
    Signature _signature;
    std::vector<Entity> _entities;
};

class IComponentPool {
public:
    virtual ~IComponentPool() = default;
};

template <typename T>
class ComponentPool : public IComponentPool {
public:
    int Size() const {
        return _components.size();
    }

    void Resize(const int size) {
        _components.resize(size);
    }

    void Set(int idx, const T& component) {
        _components[idx] = component;
    }

    T& Get(int idx) {
        return _components[idx];
    }

    T& operator[](int idx) {
        return static_cast<T&>(_components[idx]);
    }

private:
    std::vector<T> _components;
};

// Entity, Component, System 관리자
class Registry {
public:
    Registry() = default;
    ~Registry() = default;

    Entity CreateEntity();
    void KillEntity(const Entity& entity);

    template <typename T, typename ...TArgs> 
    void AddComponent(const Entity& entity, TArgs&& ...args) {
        const int componentID = Component<T>::GetID();
        const int entityID = entity.GetID();

        if(componentID >= _componentPools.size()) {
            _componentPools.resize(componentID + 1);
        }

        // componentID 값이 순차적으로 들어오리라는 보장없음.
        // 사이즈는 커져있어도 pool이 생성되지 않는 경우가 있음.
        if(!_componentPools[componentID]) {
            _componentPools[componentID] = std::make_shared<ComponentPool<T>>();
        }

        auto pool = std::static_pointer_cast<ComponentPool<T>>(_componentPools[componentID]);

        if(entityID >= pool->Size()) {
            pool->Resize(entityID + 1);
        }

        // 주의 : 이 부분에서 컴파일 에러 발생 시 기본 생성자가 없는 것이므로 확인할것
        T newComponent(std::forward<TArgs>(args)...);

        pool->Set(entityID, newComponent);
        _entityComponentSigs[entityID].set(componentID, true);

        LOG("Component id = %d was added to entity id %d", componentID, entityID);
    }

    template <typename T>
    void RemoveComponent(const Entity& entity) {
        const int componentID = Component<T>::GetID();
        const int entityID = entity.GetID();
        _entityComponentSigs[entityID].set(componentID, false);
        LOG("Component id = %d was removed from entity id %d", componentID, entityID);
    }

    template <typename T>
    bool HasComponent(const Entity& entity) const {
        const int componentID = Component<T>::GetID();
        const int entityID = entity.GetID();
        return _entityComponentSigs[entityID].test(componentID);
    }

    template <typename T>
    T& GetComponent(const Entity& entity) const {
        const int componentID = Component<T>::GetID();
        const int entityID = entity.GetID();
        auto pool = std::static_pointer_cast<ComponentPool<T>>(_componentPools[componentID]);
        return pool->Get(entityID);
    }

    template<typename T, typename ...TArgs>
    void AddSystem(TArgs&& ...args) {
        auto newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);
        _systems.insert({std::type_index(typeid(T)), newSystem});
    }

    template<typename T>
    void RemoveSystem() {
        auto system = _systems.find(std::type_index(typeid(T)));
        _systems.erase(system);
    }

    template<typename T>
    bool HasSystem() const {
        auto system = _systems.find(std::type_index(typeid(T)));
        return system != _systems.end();
    }

    template<typename T>
    T& GetSystem() const {
        auto system = _systems.find(std::type_index(typeid(T)));
        return *(std::static_pointer_cast<T>(system->second));
    }

    void AddEntityToSystem(const Entity& entity);
    void RemoveEntityFromSystem(const Entity& entity);

    void TagEntity(const Entity& entity, const std::string& tag);
    bool EntityHasTag(const Entity& entity, const std::string& tag) const;
    Entity GetEntityByTag(const std::string& tag) const;
    void RemoveEntityTag(const Entity& entity);

    void GroupEntity(const Entity& entity, const std::string& group);
    bool EntityBelongsToGroup(const Entity& entity, const std::string& group) const;
    std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
    void RemoveEntityGroup(const Entity& entity);

    void Update();

private:
    int _numEntities = 0;
    std::deque<int> _freeEntityIDs;
    std::set<Entity> _entitiesToAdded;
    std::set<Entity> _entitiesToBeKilled;

    std::vector<std::shared_ptr<IComponentPool>> _componentPools;
    std::vector<Signature> _entityComponentSigs;
    std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;

    std::unordered_map<std::string, Entity> _entityPerTag;
    std::unordered_map<int, std::string> _tagPerEntities;

    std::unordered_map<std::string, std::set<Entity>> _entitiesPerGroup;
    std::unordered_map<int, std::string> _groupPerEntity;
};

template<typename T, typename ...TArgs> void Entity::AddComponent(TArgs&& ...args) const {
    _registry->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template<typename T> void Entity::RemoveComponent() const {
    _registry->RemoveComponent<T>(*this);
}

template<typename T> bool Entity::HasComponent() const {
    return _registry->HasComponent<T>(*this);
}

template<typename T> T& Entity::GetComponent() const {
    return _registry->GetComponent<T>(*this);
}

#endif