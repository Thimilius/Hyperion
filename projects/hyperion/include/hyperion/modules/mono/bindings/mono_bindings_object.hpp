#pragma once

#include <mono/metadata/object.h>

namespace Hyperion {

    class MonoBindingsObject final {
    public:
        static void Bind();
    private:
        static void Binding_ObjectCtor(MonoObject *object, MonoString *name);
        static MonoString *Binding_GetName(MonoObject *object);
        static void Binding_SetName(MonoObject *object, MonoString *name);
        static void Binding_Destroy(MonoObject *object);
        static bool Binding_IsNativeAlive(MonoObject *object);
    private:
        MonoBindingsObject() = delete;
        ~MonoBindingsObject() = delete;
    };

}