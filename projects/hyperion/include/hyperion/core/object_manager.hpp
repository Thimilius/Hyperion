#pragma once

#include "hyperion/core/object.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion {
    class Engine;
    class Object;
    class WorldManager;
}

namespace Hyperion {

    class ObjectManager final {
    private:
        ObjectManager() = delete;
        ~ObjectManager() = delete;

        static void LateUpdate();

        static void Destroy(Object *object);
        static void DestroyImmediate(Object *object);
    private:
        inline static Set<Object *> s_objects_to_destroy;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Object;
        friend class Hyperion::WorldManager;
    };

}