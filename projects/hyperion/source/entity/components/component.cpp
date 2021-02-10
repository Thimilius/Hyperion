#include "hyppch.hpp"

#include "hyperion/entity/components/component.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world_manager.hpp"

namespace Hyperion {

    ComponentType::ComponentType(const Type &native_type)
        : m_is_native_type(true), m_native_type(native_type), m_scripting_type(nullptr) { }

    ComponentType::ComponentType(Scripting::ScriptingType *scripting_type)
        : m_is_native_type(false), m_native_type(Type::get<Component>()), m_scripting_type(scripting_type) { }

    bool ComponentType::IsDerivedFrom(const ComponentType &base) const {
        if (m_is_native_type != base.m_is_native_type) {
            return false;
        }

        if (m_is_native_type) {
            return m_native_type.is_derived_from(base.m_native_type);
        } else {
            return m_scripting_type->IsDerivedFrom(base.m_scripting_type);
        }
    }

    bool ComponentType::operator==(const ComponentType &other) const {
        if (m_is_native_type != other.m_is_native_type) {
            return false;
        } else {
            if (m_is_native_type) {
                return m_native_type == other.m_native_type;
            } else {
                return m_scripting_type == other.m_scripting_type;
            }
        }
    }

    bool ComponentType::operator!=(const ComponentType &other) const {
        return !(*this == other);
    }

    Transform *Component::GetTransform() const {
        return m_entity->GetTransform();
    }

    World *Component::GetWorld() const {
        return m_entity->GetWorld();
    }

    void Component::RegisterForUpdate() {
        WorldManager::RegisterComponentForUpdate(this);
    }

    void Component::UnregisterForUpdate() {
        WorldManager::UnregisterComponentForUpdate(this);
    }

    void Component::OnDestroy() {
        GetEntity()->DispatchMessage({ EntityMessageType::ComponentDestroyed, this });

        UnregisterForUpdate();
    }

}
