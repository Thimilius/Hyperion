#include "hyppch.hpp"

#include "hyperion/entity/object_system.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {

    void ObjectSystem::LateUpdate() {
        for (Object *object : s_objects_to_destroy) {
            DestroyImmediate(object);
        }
        s_objects_to_destroy.clear();
    }

    void ObjectSystem::Destroy(Object *object) {
        s_objects_to_destroy.push_back(object);
    }

    void ObjectSystem::DestroyImmediate(Object *object) {
        HYP_ASSERT_MESSAGE(object->GetType() != Transform::GetStaticType(), "Destroying a transform component is not allowed");

        object->OnDestroy();

        delete object;
    }

}