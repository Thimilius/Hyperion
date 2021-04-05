//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_bindings.hpp"

//---------------------- Library Includes ----------------------
#include <mono/metadata/exception.h>

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/object/object.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/modules/mono/mono_scripting_storage.hpp"
#include "hyperion/modules/mono/managed/mono_managed_string.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    template<typename T>
    T *GetScriptingObjectAs(MonoObject *mono_object) {
        T *scripting_object = MonoScriptingStorage::GetScriptingObject<T>(mono_object);
        if (scripting_object != nullptr) {
            return scripting_object;
        } else {
            mono_raise_exception(mono_get_exception_invalid_operation("Trying to access an object that was already destroyed"));
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoClass *GetMonoClassFromReflectionType(MonoReflectionType *mono_reflection_type) {
        MonoType *mono_type = mono_reflection_type_get_type(mono_reflection_type);
        HYP_ASSERT(mono_type);
        MonoClass *mono_class = mono_type_get_class(mono_type);
        HYP_ASSERT(mono_class);

        // First check that we are actually searching for a component type that derives from 'Hyperion.Component'.
        if (!mono_class_is_subclass_of(mono_class, MonoScriptingStorage::GetSpecialClass(MonoSpecialClass::Component), false)) {
            // TODO: We need to define a custom exception we can throw here.
            return nullptr;
        } else {
            return mono_class;
        }
    }

    //--------------------------------------------------------------
    bool ValidateComponentMonoClass(MonoClass *mono_class) {
        if (!mono_class_is_subclass_of(mono_class, MonoScriptingStorage::GetSpecialClass(MonoSpecialClass::Script), false)) {
            mono_raise_exception(mono_get_exception_argument("type", "The requested component must inherit from 'Hyperion.Script'"));
            return false;
        } else {
            return true;
        }
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Component_GetEntity(MonoObject *mono_component) {
        Component *component = GetScriptingObjectAs<Component>(mono_component);
        return MonoScriptingStorage::GetOrCreateMonoObject(component->GetEntity());
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Component_GetTransform(MonoObject *mono_component) {
        Component *component = GetScriptingObjectAs<Component>(mono_component);
        return MonoScriptingStorage::GetOrCreateMonoObject(component->GetTransform());
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetTransform(MonoObject *mono_entity) {
        Entity *entity = GetScriptingObjectAs<Entity>(mono_entity);
        return MonoScriptingStorage::GetOrCreateMonoObject(entity->GetTransform());
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetWorld(MonoObject *mono_entity) {
        Entity *entity = GetScriptingObjectAs<Entity>(mono_entity);
        return MonoScriptingStorage::GetOrCreateMonoObject(entity->GetWorld());
    }

    //--------------------------------------------------------------
    void Binding_Entity_Ctor(MonoObject *mono_entity, MonoString *mono_entity_name) {
        EntityCreationParameters entity_creation_parameters = EntityCreationParameters();
        if (mono_entity_name != nullptr) {
            entity_creation_parameters.name = MonoManagedString(mono_entity_name).GetString();
        }
        Entity *entity = Entity::Create(entity_creation_parameters);
        MonoScriptingStorage::RegisterMonoObject(mono_entity, entity);
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_AddComponent(MonoObject *mono_entity, MonoReflectionType *mono_component_reflection_type) {
        Entity *entity = GetScriptingObjectAs<Entity>(mono_entity);
        MonoClass *mono_component_class = GetMonoClassFromReflectionType(mono_component_reflection_type);
        Type *type = MonoScriptingStorage::GetNativeType(mono_component_class);
        if (type != nullptr) {
            return MonoScriptingStorage::GetOrCreateMonoObject(entity->AddComponent(type));
        } else {
            if (ValidateComponentMonoClass(mono_component_class)) {
                // TODO: Do validation like required and multiple components. Maybe we can should do that in the managed side?
                Script *script = entity->AddComponent<Script>();
                MonoObject *mono_object = MonoScriptingStorage::CreateMonoObject(script, mono_component_class);
                MonoScriptingInstance *mono_scripting_instance = static_cast<MonoScriptingInstance *>(script->GetScriptingInstance());
                return mono_object;
            } else {
                return nullptr;
            }
        }
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetComponent(MonoObject *mono_entity, MonoReflectionType *mono_component_reflection_type) {
        Entity *entity = GetScriptingObjectAs<Entity>(mono_entity);
        MonoClass *mono_component_class = GetMonoClassFromReflectionType(mono_component_reflection_type);
        Type *type = MonoScriptingStorage::GetNativeType(mono_component_class);
        if (type != nullptr) {
            return MonoScriptingStorage::GetOrCreateMonoObject(entity->GetComponent(type));
        } else {
            if (ValidateComponentMonoClass(mono_component_class)) {
                Vector<Script *> scripts = entity->GetComponents<Script>();
                for (Script *script : scripts) {
                    MonoClass *mono_class = static_cast<MonoScriptingType *>(script->GetScriptingInstance()->GetScriptingType())->GetMonoClass();
                    if (mono_class_is_assignable_from(mono_class, mono_component_class)) {
                        return MonoScriptingStorage::GetOrCreateMonoObject(script);
                    }
                }
                return nullptr;
            } else {
                return nullptr;
            }
        }
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_CreatePrimitive(EntityPrimitive primitive) {
        Entity *entity = Entity::CreatePrimitive(primitive);
        return MonoScriptingStorage::GetOrCreateMonoObject(entity);
    }

    //--------------------------------------------------------------
    MonoString *Binding_Object_GetName(MonoObject *mono_object) {
        Object *object = GetScriptingObjectAs<Object>(mono_object);
        return MonoManagedString(object->GetName()).GetMonoString();
    }

    //--------------------------------------------------------------
    void Binding_Object_SetName(MonoObject *mono_object, MonoString *managed_name) {
        Object *object = GetScriptingObjectAs<Object>(mono_object);
        object->SetName(MonoManagedString(managed_name).GetString());
    }

    //--------------------------------------------------------------
    void Binding_Object_Destroy(MonoObject *mono_object) {
        Object *object = GetScriptingObjectAs<Object>(mono_object);
        Object::Destroy(object);
    }

    //--------------------------------------------------------------
    bool Binding_Object_IsNativeAlive(MonoObject *mono_object) {
        return MonoScriptingStorage::GetScriptingObject(mono_object) != nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Script_Ctor(MonoObject *mono_script) {
        // TODO: Add validation check that we do not call this constructor through new.
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetPosition(MonoObject *mono_transform, Vec3 *position) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        *position = transform->GetPosition();
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetPosition(MonoObject *mono_transform, Vec3 *position) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        transform->SetPosition(*position);
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetRotation(MonoObject *mono_transform, Quaternion *rotation) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        *rotation = transform->GetRotation();
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetRotation(MonoObject *mono_transform, Quaternion *rotation) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        transform->SetRotation(*rotation);
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetEulerAngles(MonoObject *mono_transform, Vec3 *euler_angles) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        *euler_angles = transform->GetEulerAngles();
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetEulerAngles(MonoObject *mono_transform, Vec3 *euler_angles) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        transform->SetEulerAngles(*euler_angles);
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Transform_GetParent(MonoObject *mono_transform) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        return MonoScriptingStorage::GetOrCreateMonoObject(transform->GetParent());
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetParent(MonoObject *mono_transform, MonoObject *mono_parent_transform) {
        Transform *transform = GetScriptingObjectAs<Transform>(mono_transform);
        Transform *parent_transform = GetScriptingObjectAs<Transform>(mono_parent_transform);
        transform->SetParent(parent_transform);
    }

    //--------------------------------------------------------------
    void MonoScriptingBindings::Bind() {
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
            mono_add_internal_call("Hyperion.Component::Binding_GetTransform", Binding_Component_GetTransform);
        }

        // Entity
        {
            mono_add_internal_call("Hyperion.Entity::Binding_GetTransform", Binding_Entity_GetTransform);
            mono_add_internal_call("Hyperion.Entity::Binding_GetWorld", Binding_Entity_GetWorld);
            mono_add_internal_call("Hyperion.Entity::Binding_Ctor", Binding_Entity_Ctor);
            mono_add_internal_call("Hyperion.Entity::Binding_AddComponent", Binding_Entity_AddComponent);
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

        // Script
        {
            mono_add_internal_call("Hyperion.Script::Binding_Ctor", Binding_Script_Ctor);
        }

        // Time
        {
            mono_add_internal_call("Hyperion.Time::Binding_GetElapsedTime", Time::GetTimeSinceEngineModeChange);
            mono_add_internal_call("Hyperion.Time::Binding_GetDeltaTime", Time::GetDeltaTime);
            mono_add_internal_call("Hyperion.Time::Binding_GetFixedDeltaTime", Time::GetFixedDeltaTime);
        }

        // Transform
        {
            mono_add_internal_call("Hyperion.Transform::Binding_GetPosition", Binding_Transform_GetPosition);
            mono_add_internal_call("Hyperion.Transform::Binding_SetPosition", Binding_Transform_SetPosition);
            mono_add_internal_call("Hyperion.Transform::Binding_GetRotation", Binding_Transform_GetRotation);
            mono_add_internal_call("Hyperion.Transform::Binding_SetRotation", Binding_Transform_SetRotation);
            mono_add_internal_call("Hyperion.Transform::Binding_GetEulerAngles", Binding_Transform_GetEulerAngles);
            mono_add_internal_call("Hyperion.Transform::Binding_SetEulerAngles", Binding_Transform_SetEulerAngles);
            mono_add_internal_call("Hyperion.Transform::Binding_GetParent", Binding_Transform_GetParent);
            mono_add_internal_call("Hyperion.Transform::Binding_SetParent", Binding_Transform_SetParent);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingBindings::RegisterClasses() {
        MonoScriptingStorage::RegisterClass<Behaviour>("Hyperion", "Behaviour");
        MonoScriptingStorage::RegisterClass<BoxCollider>("Hyperion", "BoxCollider");
        MonoScriptingStorage::RegisterClass<Collider>("Hyperion", "Collider");
        MonoScriptingStorage::RegisterClass<Component>("Hyperion", "Component", MonoSpecialClass::Component);
        MonoScriptingStorage::RegisterClass<Entity>("Hyperion", "Entity");
        MonoScriptingStorage::RegisterClass<Object>("Hyperion", "Object");
        MonoScriptingStorage::RegisterClass<Script>("Hyperion", "Script", MonoSpecialClass::Script);
        MonoScriptingStorage::RegisterClass<SphereCollider>("Hyperion", "SphereCollider");
        MonoScriptingStorage::RegisterClass<Transform>("Hyperion", "Transform");
        MonoScriptingStorage::RegisterClass<World>("Hyperion", "World");
    }

}