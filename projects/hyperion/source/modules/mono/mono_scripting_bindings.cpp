//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_bindings.hpp"

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
#include "hyperion/modules/mono/mono_scripting_driver.hpp"
#include "hyperion/modules/mono/mono_scripting_instance.hpp"
#include "hyperion/modules/mono/mono_scripting_storage.hpp"
#include "hyperion/modules/mono/managed/mono_managed_string.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoObject *Binding_Component_GetEntity(MonoObject *mono_component) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetTransform(MonoObject *mono_entity) {
        if (Entity *entity = MonoScriptingStorage::GetScriptingObject<Entity>(mono_entity)) {
            return MonoScriptingStorage::GetOrCreateMonoObject(entity->GetTransform(), entity->GetType());
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetWorld(MonoObject *mono_entity) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Entity_Ctor(MonoObject *mono_entity, MonoString *mono_entity_name) {
        
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_AddComponent(MonoObject *mono_entity, MonoReflectionType *mono_component_reflection_type) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetComponent(MonoObject *mono_entity, MonoReflectionType *mono_component_reflection_type) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_CreatePrimitive(EntityPrimitive primitive) {
        Entity *entity = Entity::CreatePrimitive(primitive);
        return MonoScriptingStorage::GetOrCreateMonoObject(entity, entity->GetType());
    }

    //--------------------------------------------------------------
    MonoString *Binding_Object_GetName(MonoObject *mono_object) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Object_SetName(MonoObject *mono_object, MonoString *managed_name) {
    }

    //--------------------------------------------------------------
    void Binding_Object_Destroy(MonoObject *mono_object) {

    }

    //--------------------------------------------------------------
    bool Binding_Object_IsNativeAlive(MonoObject *mono_object) {
        return false;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Renderer_GetMaterial(MonoObject *mono_renderer) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetPosition(MonoObject *mono_transform, Vec3 *position) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_SetPosition(MonoObject *mono_transform, Vec3 *position) {
        if (Transform *transform = MonoScriptingStorage::GetScriptingObject<Transform>(mono_transform)) {
            transform->SetPosition(*position);
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetRotation(MonoObject *mono_transform, Quaternion *position) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_SetRotation(MonoObject *mono_transform, Quaternion *position) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_GetEulerAngles(MonoObject *mono_transform, Vec3 *euler_angles) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_SetEulerAngles(MonoObject *mono_transform, Vec3 *euler_angles) {

    }

    //--------------------------------------------------------------
    MonoObject *Binding_Transform_GetParent(MonoObject *mono_transform) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetParent(MonoObject *mono_transform, MonoObject *mono_parent_transform) {

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

        // Renderer
        {
            mono_add_internal_call("Hyperion.Renderer::Binding_GetMaterial", Binding_Renderer_GetMaterial);
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
        MonoScriptingStorage::RegisterClass<Entity>("Hyperion", "Entity");
    }

}