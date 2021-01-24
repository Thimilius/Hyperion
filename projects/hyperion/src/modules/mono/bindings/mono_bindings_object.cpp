#include "hyppch.hpp"

#include "hyperion/modules/mono/bindings/mono_bindings_object.hpp"

#include <mono/jit/jit.h>

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

namespace Hyperion {

    void MonoBindingsObject::Bind() {
        mono_add_internal_call("Hyperion.Object::Binding_ObjectCtor", Binding_ObjectCtor);
        mono_add_internal_call("Hyperion.Object::Binding_GetName", Binding_GetName);
        mono_add_internal_call("Hyperion.Object::Binding_SetName", Binding_SetName);
        mono_add_internal_call("Hyperion.Object::Binding_Destroy", Binding_Destroy);
        mono_add_internal_call("Hyperion.Object::Binding_IsNativeAlive", Binding_IsNativeAlive);
    }

    void MonoBindingsObject::Binding_ObjectCtor(MonoObject *object, MonoString *name) {
        Object *native_object;
        if (name == nullptr) {
            native_object = Object::Create();
        } else {
            char *native_name = mono_string_to_utf8(name);
            native_object = Object::Create(native_name);
            mono_free(native_name);
        }
        MonoScriptingDriver::RegisterObject(object, native_object);
    }

    MonoString *MonoBindingsObject::Binding_GetName(MonoObject *object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(object)) {
            return mono_string_new(MonoScriptingDriver::GetDomain(), native_object->GetName().c_str());
        } else {
            return nullptr;
        }
    }

    void MonoBindingsObject::Binding_SetName(MonoObject *object, MonoString *name) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(object)) {
            char *native_name = mono_string_to_utf8(name);
            native_object->SetName(native_name);
            mono_free(native_name);
        }
    }

    void MonoBindingsObject::Binding_Destroy(MonoObject *object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(object)) {
            MonoScriptingDriver::UnregisterObject(object);
            Object::Destroy(native_object);
        }
    }

    bool MonoBindingsObject::Binding_IsNativeAlive(MonoObject *object) {
        return MonoScriptingDriver::IsRegisterdObject(object);
    }

}