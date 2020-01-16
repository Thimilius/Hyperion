#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    Entity::Entity(const String &name) : Asset(name) { }

    Entity::~Entity() {
        for (auto pair : m_components) {
            delete pair.second;
        }
    }

}