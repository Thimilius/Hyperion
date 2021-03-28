#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_instance.hpp"
#include "hyperion/scripting/scripting_object.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Scripting {
    class MonoScriptingDriver;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingStorage final {
    public:
        template<typename T> inline static T *GetScriptingObject(MonoObject *mono_object) { return static_cast<T *>(GetScriptingObject(mono_object)); }
        static ScriptingObject *GetScriptingObject(MonoObject *mono_object);
        static MonoObject *GetOrCreateMonoObject(ScriptingObject *scripting_object, Type *type);

        template<typename T> inline static void RegisterClass(const char *name_space, const char *name) { RegisterClass(name_space, name, Type::Get<T>()); }
        static void RegisterClass(const char *name_space, const char *name, Type *type);

        static MonoScriptingType *GetOrCreateScriptingType(MonoClass *mono_class);
    private:
        MonoScriptingStorage() = delete;
        ~MonoScriptingStorage() = delete;
    private:
        static void Clear();
    private:
        inline static Map<MonoObject *, ScriptingObject *> s_mono_object_to_scripting_objects;

        inline static Map<MonoClass *, Type *> s_mono_class_to_native_types;
        inline static Map<Type *, MonoClass *> s_native_type_to_mono_classes;

        inline static Map<MonoClass *, std::unique_ptr<MonoScriptingType>> s_scripting_types;
    private:
        friend class Hyperion::Scripting::MonoScriptingDriver;
    };

}