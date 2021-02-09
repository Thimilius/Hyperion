#include "hyppch.hpp"

#include "hyperion/modules/mono/bindings/mono_bindings_object.hpp"

#include <mono/jit/jit.h>

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

namespace Hyperion::Scripting {

    MonoString *Binding_GetName(MonoObject *object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(object)) {
            return mono_string_new(MonoScriptingDriver::GetDomain(), native_object->GetName().c_str());
        } else {
            return nullptr;
        }
    }

    void Binding_SetName(MonoObject *object, MonoString *name) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(object)) {
            char *native_name = mono_string_to_utf8(name);
            native_object->SetName(native_name);
            mono_free(native_name);
        }
    }

    void Binding_Destroy(MonoObject *object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(object)) {
            MonoScriptingDriver::UnregisterObject(object);
            Object::Destroy(native_object);
        }
    }

    bool Binding_IsNativeAlive(MonoObject *object) {
        return MonoScriptingDriver::IsRegisterdObject(object);
    }

    void MonoBindingsObject::Bind() {
        mono_add_internal_call("Hyperion.Object::Binding_GetName", Binding_GetName);
        mono_add_internal_call("Hyperion.Object::Binding_SetName", Binding_SetName);
        mono_add_internal_call("Hyperion.Object::Binding_Destroy", Binding_Destroy);
        mono_add_internal_call("Hyperion.Object::Binding_IsNativeAlive", Binding_IsNativeAlive);
    }

}