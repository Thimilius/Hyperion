#include "hyppch.hpp"

#include "hyperion/core/object.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/entity_component.hpp"

namespace Hyperion {

    void Object::Destroy(Object *object) {
        if (object) {
            // Notify entity if on of its components gets destroyed
            if (object->IsBase(EntityComponent::GetStaticType())) {
                EntityComponent *entity_component = (EntityComponent *)object;
                entity_component->GetEntity()->OnEvent({ EntityEventType::ComponentDestroyed, entity_component });
            }

            delete object;
        }
    }

}