#include "hyppch.hpp"

#include "hyperion/core/object.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion {

    void Object::Destroy(Object *object) {
        if (object) {
            HYP_ASSERT_MESSAGE(object->GetType() != TransformComponent::GetStaticType(), "Destroying a transform component is not allowed");

            // Notify entity if on of its components gets destroyed
            if (object->IsBase(EntityComponent::GetStaticType())) {
                EntityComponent *entity_component = (EntityComponent *)object;
                entity_component->GetEntity()->OnEvent({ EntityEventType::ComponentDestroyed, entity_component });
            }

            delete object;
        }
    }

}