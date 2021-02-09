#include "hyppch.hpp"

#include "hyperion/modules/mono/bindings/mono_bindings_entity.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

namespace Hyperion::Scripting {

    void Binding_Ctor(MonoObject *object, MonoString *name) {
        Entity *native_entity;
        if (name == nullptr) {
            native_entity = Entity::Create("Entity");
        } else {
            char *native_name = mono_string_to_utf8(name);
            native_entity = Entity::Create(native_name);
            mono_free(native_name);
        }
        MonoScriptingDriver::RegisterObject(object, native_entity);
    }


    void MonoBindingsEntity::Bind() {
        mono_add_internal_call("Hyperion.Entity::Binding_Ctor", Binding_Ctor);
    }

}