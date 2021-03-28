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

    enum class MonoSpecialClass {
        None,

        Component,
        Script,
    };

    class MonoScriptingStorage final {
    public:
        static MonoClass *GetSpecialClass(MonoSpecialClass mono_special_class);

        template<typename T> inline static T *GetScriptingObject(MonoObject *mono_object) { return static_cast<T *>(GetScriptingObject(mono_object)); }
        static ScriptingObject *GetScriptingObject(MonoObject *mono_object);
        static MonoObject *GetOrCreateMonoObject(ScriptingObject *scripting_object);
        static void RegisterMonoObject(MonoObject *mono_object, ScriptingObject *scripting_object);
        static void UnregisterMonoObject(MonoObject *mono_object);

        template<typename T> inline static void RegisterClass(const char *name_space, const char *name, MonoSpecialClass mono_special_class = MonoSpecialClass::None) { RegisterClass(name_space, name, mono_special_class, Type::Get<T>()); }
        static void RegisterClass(const char *name_space, const char *name, MonoSpecialClass mono_special_class, Type *type);
        static Type *GetNativeType(MonoClass *mono_class);

        static MonoScriptingType *GetOrCreateScriptingType(MonoClass *mono_class);
    private:
        MonoScriptingStorage() = delete;
        ~MonoScriptingStorage() = delete;
    private:
        static void Clear();
    private:
        inline static struct MonoSepcialClasses {
            MonoClass *component_class;
            MonoClass *script_class;
        } s_special_classes;

        inline static Map<MonoObject *, ScriptingObject *> s_mono_object_to_scripting_objects;

        inline static Map<MonoClass *, Type *> s_mono_class_to_native_types;
        inline static Map<Type *, MonoClass *> s_native_type_to_mono_classes;

        inline static Map<MonoClass *, std::unique_ptr<MonoScriptingType>> s_scripting_types;
    private:
        friend class Hyperion::Scripting::MonoScriptingDriver;
    };

}