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
        if (mono_object == nullptr) {
            return nullptr;
        }

        auto it = s_mono_object_to_scripting_objects.find(mono_object);
        if (it != s_mono_object_to_scripting_objects.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoClass *MonoScriptingStorage::GetSpecialClass(MonoSpecialClass mono_special_class) {
        switch (mono_special_class) 	{
            case MonoSpecialClass::Component: return s_special_classes.component_class;
            case MonoSpecialClass::Script: return s_special_classes.script_class;
            default: return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoObject *MonoScriptingStorage::GetOrCreateMonoObject(ScriptingObject *scripting_object) {
        if (scripting_object == nullptr) {
            return nullptr;
        }

        MonoScriptingInstance *mono_scripting_instance = static_cast<MonoScriptingInstance *>(scripting_object->GetScriptingInstance());
        if (mono_scripting_instance) {
            return mono_scripting_instance->GetMonoObject();
        } else {
            Type *type = scripting_object->GetType();
            HYP_ASSERT(s_native_type_to_mono_classes.find(type) != s_native_type_to_mono_classes.end());
            MonoClass *mono_class = s_native_type_to_mono_classes[scripting_object->GetType()];
            MonoObject *mono_object = mono_object_new(MonoScriptingDriver::GetRuntimeDomain()->GetMonoDomain(), mono_class);

            RegisterMonoObject(mono_object, scripting_object);

            return mono_object;
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingStorage::RegisterMonoObject(MonoObject *mono_object, ScriptingObject *scripting_object) {
        HYP_ASSERT(mono_object);
        HYP_ASSERT(scripting_object);
        HYP_ASSERT(scripting_object->GetScriptingInstance() == nullptr);
        HYP_ASSERT(s_mono_object_to_scripting_objects.find(mono_object) == s_mono_object_to_scripting_objects.end());

        MonoScriptingInstance *new_mono_scripting_instance = new MonoScriptingInstance(mono_object);
        scripting_object->SetScriptingInstance(new_mono_scripting_instance);

        s_mono_object_to_scripting_objects[mono_object] = scripting_object;
    }

    //--------------------------------------------------------------
    void MonoScriptingStorage::UnregisterMonoObject(MonoObject *mono_object) {
        HYP_ASSERT(mono_object);
        HYP_ASSERT(s_mono_object_to_scripting_objects.find(mono_object) != s_mono_object_to_scripting_objects.end());

        s_mono_object_to_scripting_objects.erase(mono_object);
    }

    //--------------------------------------------------------------
    void MonoScriptingStorage::RegisterClass(const char *name_space, const char *name, MonoSpecialClass mono_special_class, Type *type) {
        HYP_ASSERT(type);

        MonoClass *mono_class = MonoScriptingDriver::GetCoreAssembly()->FindClass(name_space, name);

        HYP_ASSERT(s_mono_class_to_native_types.find(mono_class) == s_mono_class_to_native_types.end());
        HYP_ASSERT(s_native_type_to_mono_classes.find(type) == s_native_type_to_mono_classes.end());

        s_mono_class_to_native_types[mono_class] = type;
        s_native_type_to_mono_classes[type] = mono_class;

        switch (mono_special_class) 	{
            case MonoSpecialClass::Component: s_special_classes.component_class = mono_class; break;
            case MonoSpecialClass::Script: s_special_classes.script_class = mono_class; break;
        }
    }

    //--------------------------------------------------------------
    Type *MonoScriptingStorage::GetNativeType(MonoClass *mono_class) {
        HYP_ASSERT(mono_class);

        auto it = s_mono_class_to_native_types.find(mono_class);
        if (it != s_mono_class_to_native_types.end()) {
            return it->second;
        } else {
            return nullptr;
        }
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
        // We have to clear out all scripting instances.
        // We need to make a copy, so the instances can remove themselves from the map.
        auto mono_object_to_scripting_objects = s_mono_object_to_scripting_objects;
        for (auto [mono_object, scripting_object] : mono_object_to_scripting_objects) {
            scripting_object->ClearScriptingInstance();
        }
        s_mono_object_to_scripting_objects.clear();

        s_mono_class_to_native_types.clear();
        s_native_type_to_mono_classes.clear();

        s_scripting_types.clear();
    }

}