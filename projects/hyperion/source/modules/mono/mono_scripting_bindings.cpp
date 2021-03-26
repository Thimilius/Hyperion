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
#include "hyperion/modules/mono/mono_scripting_helper.hpp"
#include "hyperion/modules/mono/mono_scripting_instance.hpp"
#include "hyperion/modules/mono/managed/mono_managed_string.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoObject *Binding_Component_GetEntity(MonoObject *managed_component) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetTransform(MonoObject *managed_entity) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetWorld(MonoObject *managed_entity) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Entity_Ctor(MonoObject *managed_object, MonoString *managed_name) {
        
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_AddComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_CreatePrimitive(EntityPrimitive primitive) {
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoString *Binding_Object_GetName(MonoObject *managed_object) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Object_SetName(MonoObject *managed_object, MonoString *managed_name) {
    }

    //--------------------------------------------------------------
    void Binding_Object_Destroy(MonoObject *managed_object) {

    }

    //--------------------------------------------------------------
    bool Binding_Object_IsNativeAlive(MonoObject *managed_object) {
        return false;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Renderer_GetMaterial(MonoObject *managed_renderer) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetPosition(MonoObject *managed_transform, Vec3 *position) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_SetPosition(MonoObject *managed_transform, Vec3 *position) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_GetRotation(MonoObject *managed_transform, Quaternion *position) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_SetRotation(MonoObject *managed_transform, Quaternion *position) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_GetEulerAngles(MonoObject *managed_transform, Vec3 *euler_angles) {

    }

    //--------------------------------------------------------------
    void Binding_Transform_SetEulerAngles(MonoObject *managed_transform, Vec3 *euler_angles) {

    }

    //--------------------------------------------------------------
    MonoObject *Binding_Transform_GetParent(MonoObject *managed_transform) {
        return nullptr;
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetParent(MonoObject *managed_transform, MonoObject *managed_parent) {

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

        // Time
        {
            // TODO: We need to intercept the elapsed time call when we are in the editor runtime.
            mono_add_internal_call("Hyperion.Time::Binding_GetElapsedTime", Time::GetTime);
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

        // Renderer
        {
            mono_add_internal_call("Hyperion.Renderer::Binding_GetMaterial", Binding_Renderer_GetMaterial);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingBindings::RegisterClasses() {
        
    }

}