#pragma once

#include "hyperion/entity/object.hpp"

namespace Hyperion {

    class ObjectManager {
    private:
        inline static Vector<Object *> s_objects_to_destroy;
    private:
        ObjectManager() = delete;
        ~ObjectManager() = delete;

        static void LateUpdate();
        static void Destroy(Object *object);
        static void DestroyImmediate(Object *object);
        
        friend class Object;
        friend class Engine;
    };

}