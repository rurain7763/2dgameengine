#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity {
public:
    Entity(int id) : _id(id) {}
    Entity(const Entity& other) = default;
    int GetID() const;

    Entity& operator=(const Entity& other) = default;
    bool operator==(const Entity& other) const { return _id == other.GetID(); }
    bool operator!=(const Entity& other) const { return _id != other.GetID(); }

private:
    int _id;
};

class IComponent {
protected:
    static int idCounter;
};

template <typename T>
class Component : public IComponent {
public:
    static int GetID() {
        static int id = idCounter++;
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

class Registry {
};

#endif