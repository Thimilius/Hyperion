#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_bindings.hpp"

#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/object/object.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

namespace Hyperion::Scripting {

    MonoObject *Binding_Component_GetEntity(MonoObject *managed_object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            Component *component = static_cast<Component *>(native_object);
            MonoObject *managed_entity = MonoScriptingDriver::GetManagedObject(component->GetEntity());
            HYP_ASSERT(managed_entity);
            return managed_entity;
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_GetTransform(MonoObject *managed_object, MonoString *managed_name) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            Entity *entity = static_cast<Entity *>(native_object);
            return MonoScriptingDriver::GetOrCreateManagedObject(entity->GetTransform(), Type::get<Transform>());
        } else {
            return nullptr;
        }
    }


    void Binding_Entity_Ctor(MonoObject *managed_object, MonoString *managed_name) {
        Entity *native_entity;
        if (managed_name == nullptr) {
            native_entity = Entity::Create("Entity");
        } else {
            char *native_name = mono_string_to_utf8(managed_name);
            native_entity = Entity::Create(native_name);
            mono_free(native_name);
        }
        MonoScriptingDriver::RegisterObject(managed_object, native_entity);
    }

    MonoObject *Binding_Entity_GetComponent(MonoObject *managed_object, MonoReflectionType *reflection_type) {
        MonoType *type = mono_reflection_type_get_type(reflection_type);
        HYP_ASSERT(type);
        MonoClass *managed_class = mono_type_get_class(type);
        HYP_ASSERT(managed_class);

        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            Entity *entity = static_cast<Entity *>(native_object);
            Type native_class = MonoScriptingDriver::GetNativeClass(managed_class);
            Component *component = entity->GetComponent(native_class);
            MonoObject *result = MonoScriptingDriver::GetOrCreateManagedObject(component, native_class);
            return result;
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_CreatePrimitive(EntityPrimitive primitive) {
        Entity *native_entity = Entity::CreatePrimitive(primitive);
        return MonoScriptingDriver::CreateManagedObject(Type::get<Entity>(), native_entity);
    }

    MonoString *Binding_Object_GetName(MonoObject *managed_object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            return mono_string_new(MonoScriptingDriver::GetDomain(), native_object->GetName().c_str());
        } else {
            return nullptr;
        }
    }

    void Binding_Object_SetName(MonoObject *managed_object, MonoString *managed_name) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            char *native_name = mono_string_to_utf8(managed_name);
            native_object->SetName(native_name);
            mono_free(native_name);
        }
    }

    void Binding_Object_Destroy(MonoObject *managed_object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            // NOTE: This type check here and acting accordingly feels kinda like a hack.
            // A better architecture would probably involve storing some kind of scripting representation directly inside an Object.

            // When we are destroying entities we also need to unregister all previously registered components.
            if (native_object->GetType() == Type::get<Entity>()) {
                Entity *entity = static_cast<Entity *>(native_object);
                const Map<Type, Component *> &components = entity->GetComponents();
                for (auto [component_type, component] : components) {
                    MonoObject *managed_component = MonoScriptingDriver::GetManagedObject(component);
                    if (managed_component) {
                        MonoScriptingDriver::UnregisterObject(managed_component);
                    }
                }
            }
            MonoScriptingDriver::UnregisterObject(managed_object);

            Object::Destroy(native_object);
        }
    }

    bool Binding_Object_IsNativeAlive(MonoObject *managed_object) {
        return MonoScriptingDriver::IsRegisterdObject(managed_object);
    }

    void Binding_Transform_GetPosition(MonoObject *managed_object, Vec3 *position) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            Transform *transform = static_cast<Transform *>(native_object);
            *position = transform->GetPosition();
        }
    }

    void Binding_Transform_SetPosition(MonoObject *managed_object, Vec3 *position) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObject(managed_object)) {
            Transform *transform = static_cast<Transform *>(native_object);
            transform->SetPosition(*position);
        }
    }

    void MonoScriptingBindings::Bind() {
        // Register classes
        {
            MonoScriptingDriver::RegisterClass(Type::get<Component>(), "Hyperion", "Component");
            MonoScriptingDriver::RegisterClass(Type::get<Entity>(), "Hyperion", "Entity");
            MonoScriptingDriver::RegisterClass(Type::get<Object>(), "Hyperion", "Object");
            MonoScriptingDriver::RegisterClass(Type::get<Transform>(), "Hyperion", "Transform");
        }

        // Input
        {
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyDown", Input::IsKeyDown);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyHold", Input::IsKeyHold);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsKeyUp);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsMouseButtonDown);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsMouseButtonHold);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsKeyUp);
        }

        // Component
        {
            mono_add_internal_call("Hyperion.Component::Binding_GetEntity", Binding_Component_GetEntity);
        }

        // Entity
        {
            mono_add_internal_call("Hyperion.Entity::Binding_GetTransform", Binding_Entity_GetTransform);
            mono_add_internal_call("Hyperion.Entity::Binding_Ctor", Binding_Entity_Ctor);
            mono_add_internal_call("Hyperion.Entity::Binding_GetComponent", Binding_Entity_GetComponent);
            mono_add_internal_call("Hyperion.Entity::Binding_CreatePrimitive", Binding_Entity_CreatePrimitive);
        }

        // Object
        {
            mono_add_internal_call("Hyperion.Object::Binding_GetName", Binding_Object_GetName);
            mono_add_internal_call("Hyperion.Object::Binding_SetName", Binding_Object_SetName);
            mono_add_internal_call("Hyperion.Object::Binding_Destroy", Binding_Object_Destroy);
            mono_add_internal_call("Hyperion.Object::Binding_IsNativeAlive", Binding_Object_IsNativeAlive);
        }

        // Time
        {
            mono_add_internal_call("Hyperion.Time::Binding_GetElapsedTime", Time::GetTime);
            mono_add_internal_call("Hyperion.Time::Binding_GetDeltaTime", Time::GetDeltaTime);
            mono_add_internal_call("Hyperion.Time::Binding_GetFixedDeltaTime", Time::GetFixedDeltaTime);
        }

        // Transform
        {
            mono_add_internal_call("Hyperion.Transform::Binding_GetPosition", Binding_Transform_GetPosition);
            mono_add_internal_call("Hyperion.Transform::Binding_SetPosition", Binding_Transform_SetPosition);
        }
    }

}