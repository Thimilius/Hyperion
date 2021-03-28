//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_storage.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    ScriptingObject *MonoScriptingStorage::GetScriptingObject(MonoObject *mono_object) {
        auto it = s_mono_object_to_scripting_objects.find(mono_object);
        if (it != s_mono_object_to_scripting_objects.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoObject *MonoScriptingStorage::GetOrCreateMonoObject(ScriptingObject *scripting_object, Type *type) {
        MonoScriptingInstance *mono_scripting_instance = static_cast<MonoScriptingInstance *>(scripting_object->GetScriptingInstance());
        if (mono_scripting_instance) {
            return mono_scripting_instance->GetMonoObject();
        } else {
            MonoClass *mono_class = s_native_type_to_mono_classes[type];
            MonoObject *mono_object = mono_object_new(MonoScriptingDriver::GetRuntimeDomain()->GetMonoDomain(), mono_class);

            MonoScriptingInstance *new_mono_scripting_instance = new MonoScriptingInstance(mono_object);
            scripting_object->SetScriptingInstance(new_mono_scripting_instance);

            s_mono_object_to_scripting_objects[mono_object] = scripting_object;

            return mono_object;
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingStorage::RegisterClass(const char *name_space, const char *name, Type *type) {
        HYP_ASSERT(type);

        MonoClass *mono_class = MonoScriptingDriver::GetCoreAssembly()->FindClass(name_space, name);

        HYP_ASSERT(s_mono_class_to_native_types.find(mono_class) == s_mono_class_to_native_types.end());
        HYP_ASSERT(s_native_type_to_mono_classes.find(type) == s_native_type_to_mono_classes.end());

        s_mono_class_to_native_types[mono_class] = type;
        s_native_type_to_mono_classes[type] = mono_class;
    }

    //--------------------------------------------------------------
    MonoScriptingType *MonoScriptingStorage::GetOrCreateScriptingType(MonoClass *mono_class) {
        HYP_ASSERT(mono_class);

        auto it = s_scripting_types.find(mono_class);
        if (it != s_scripting_types.end()) {
            return it->second.get();
        } else {
            MonoScriptingType *mono_scripting_type = new MonoScriptingType(mono_class);
            s_scripting_types[mono_class].reset(mono_scripting_type);
            return mono_scripting_type;
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingStorage::Clear() {
        s_mono_object_to_scripting_objects.clear();

        s_mono_class_to_native_types.clear();
        s_native_type_to_mono_classes.clear();

        s_scripting_types.clear();
    }

}