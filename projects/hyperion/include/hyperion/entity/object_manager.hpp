#pragma once

#include "hyperion/entity/object.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion {

    class ObjectManager {
    private:
        inline static Map<World *, Vector<Component *>> s_components_to_update;
        inline static Vector<Object *> s_objects_to_destroy;
    private:
        ObjectManager() = delete;
        ~ObjectManager() = delete;

        static void Update(f32 delta_time);
        static void LateUpdate();

        static void RegisterComponentForUpdate(Component *component);
        static void UnregisterComponentForUpdate(Component *component);

        static void Destroy(Object *object);
        static void DestroyImmediate(Object *object);

        friend class Object;
        friend class Component;
        friend class Engine;
    };

}