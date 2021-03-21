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

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoObject *Binding_Component_GetEntity(MonoObject *managed_component) {
        if (Component *component = MonoScriptingHelper::GetNativeObjectAs<Component>(managed_component)) {
            return MonoScriptingDriver::GetOrCreateManagedObject(component->GetEntity(), Type::Get<Entity>());
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetTransform(MonoObject *managed_entity) {
        if (Entity *entity = MonoScriptingHelper::GetNativeObjectAs<Entity>(managed_entity)) {
            return MonoScriptingDriver::GetOrCreateManagedObject(entity->GetTransform(), Type::Get<Transform>());
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetWorld(MonoObject *managed_entity) {
        if (Entity *entity = MonoScriptingHelper::GetNativeObjectAs<Entity>(managed_entity)) {
            return MonoScriptingDriver::GetOrCreateManagedObjectRaw(entity->GetWorld(), Type::Get<World>());
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Binding_Entity_Ctor(MonoObject *managed_object, MonoString *managed_name) {
        Entity *native_entity;
        if (managed_name == nullptr) {
            native_entity = Entity::Create("Entity");
        } else {
            char *native_name = mono_string_to_utf8(managed_name);
            native_entity = Entity::Create(native_name);
            mono_free(native_name);
        }
        MonoScriptingDriver::RegisterManagedObject(managed_object, native_entity, false);
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_AddComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        // TODO: Reimplement.
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_GetComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        // TODO: Reimplement.
        return nullptr;
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Entity_CreatePrimitive(EntityPrimitive primitive) {
        Entity *native_entity = Entity::CreatePrimitive(primitive);
        return MonoScriptingDriver::CreateManagedObjectFromNativeType(native_entity, Type::Get<Entity>());
    }

    //--------------------------------------------------------------
    MonoString *Binding_Object_GetName(MonoObject *managed_object) {
        if (Object *native_object = MonoScriptingHelper::GetNativeObjectAs<Object>(managed_object)) {
            return MonoScriptingHelper::NewString(native_object->GetName());
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Binding_Object_SetName(MonoObject *managed_object, MonoString *managed_name) {
        if (Object *native_object = MonoScriptingHelper::GetNativeObjectAs<Object>(managed_object)) {
            char *native_name = mono_string_to_utf8(managed_name);
            native_object->SetName(native_name);
            mono_free(native_name);
        }
    }

    //--------------------------------------------------------------
    void Binding_Object_Destroy(MonoObject *managed_object) {
        if (Object *native_object = MonoScriptingHelper::GetNativeObjectAs<Object>(managed_object)) {
            if (MonoScriptingInstance *scripting_instance = reinterpret_cast<MonoScriptingInstance *>(native_object->GetScriptingInstance())) {
                scripting_instance->SendMessage(ScriptingMessage::OnDestroy);
            }

            Object::Destroy(native_object);
        }
    }

    //--------------------------------------------------------------
    bool Binding_Object_IsNativeAlive(MonoObject *managed_object) {
        return MonoScriptingDriver::IsRegisterdObject(managed_object);
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Renderer_GetMaterial(MonoObject *managed_renderer) {
        if (Renderer *renderer = MonoScriptingHelper::GetNativeObjectAs<Renderer>(managed_renderer)) {
            Material *material = renderer->GetMaterial();
            return MonoScriptingDriver::GetOrCreateManagedObject(material, Type::Get<Material>());
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetPosition(MonoObject *managed_transform, Vec3 *position) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            *position = transform->GetPosition();
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetPosition(MonoObject *managed_transform, Vec3 *position) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            transform->SetPosition(*position);
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetRotation(MonoObject *managed_transform, Quaternion *position) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            *position = transform->GetRotation();
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetRotation(MonoObject *managed_transform, Quaternion *position) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            transform->SetRotation(*position);
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_GetEulerAngles(MonoObject *managed_transform, Vec3 *euler_angles) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            *euler_angles = transform->GetEulerAngles();
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetEulerAngles(MonoObject *managed_transform, Vec3 *euler_angles) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            transform->SetEulerAngles(*euler_angles);
        }
    }

    //--------------------------------------------------------------
    MonoObject *Binding_Transform_GetParent(MonoObject *managed_transform) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            Transform *parent = transform->GetParent();
            if (parent != nullptr) {
                return MonoScriptingDriver::GetOrCreateManagedObject(parent, Type::Get<Transform>());
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Binding_Transform_SetParent(MonoObject *managed_transform, MonoObject *managed_parent) {
        if (Transform *transform = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_transform)) {
            if (Transform *parent = MonoScriptingHelper::GetNativeObjectAs<Transform>(managed_parent)) {
                transform->SetParent(parent);
            }
        }
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
        MonoScriptingDriver::RegisterClass(Type::Get<Behaviour>(), "Hyperion", "Behaviour");
        MonoScriptingDriver::RegisterClass(Type::Get<BoxCollider>(), "Hyperion", "BoxCollider");
        MonoScriptingDriver::RegisterClass(Type::Get<Camera>(), "Hyperion", "Camera");
        MonoScriptingDriver::RegisterClass(Type::Get<Collider>(), "Hyperion", "Collider");
        MonoScriptingDriver::RegisterClass(Type::Get<Component>(), "Hyperion", "Component");
        MonoScriptingDriver::RegisterClass(Type::Get<Entity>(), "Hyperion", "Entity");
        MonoScriptingDriver::RegisterClass(Type::Get<Object>(), "Hyperion", "Object");
        MonoScriptingDriver::RegisterClass(Type::Get<Material>(), "Hyperion", "Material");
        MonoScriptingDriver::RegisterClass(Type::Get<MeshRenderer>(), "Hyperion", "MeshRenderer");
        MonoScriptingDriver::RegisterClass(Type::Get<Renderer>(), "Hyperion", "Renderer");
        MonoScriptingDriver::RegisterClass(Type::Get<Script>(), "Hyperion", "Script");
        MonoScriptingDriver::RegisterClass(Type::Get<SphereCollider>(), "Hyperion", "SphereCollider");
        MonoScriptingDriver::RegisterClass(Type::Get<Transform>(), "Hyperion", "Transform");
        MonoScriptingDriver::RegisterClass(Type::Get<World>(), "Hyperion", "World");
    }

}