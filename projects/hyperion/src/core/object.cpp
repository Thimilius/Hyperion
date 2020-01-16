#include "hyppch.hpp"

#include "hyperion/core/object.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/entity_component.hpp"

namespace Hyperion {

    void Object::Destroy(Object *object) {
        if (!object->m_destroyed) {
            // Notify entity if on of its components gets destroyed
            if (object->IsBase(EntityComponent::GetStaticType())) {
                EntityComponent *entity_component = (EntityComponent *)object;
                entity_component->GetEntity()->OnComponentDestroyed(entity_component);
            }

            delete object;
            object->m_destroyed = true;
        }
    }

}