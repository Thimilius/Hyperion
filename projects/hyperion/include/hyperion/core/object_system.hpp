#pragma once

#include "hyperion/core/object.hpp"

namespace Hyperion {

    class ObjectSystem {
    private:
        inline static Vector<Object *> s_objects_to_destroy;
    private:
        ObjectSystem() = delete;
        ~ObjectSystem() = delete;

        static void LateUpdate();
        static void Destroy(Object *object);
        static void DestroyImmediate(Object *object);
        
        friend class Object;
        friend class Engine;
    };

}