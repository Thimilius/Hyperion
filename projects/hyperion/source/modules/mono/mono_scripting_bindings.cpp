#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_bindings.hpp"

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

namespace Hyperion::Scripting {

    MonoObject *Binding_Component_GetEntity(MonoObject *managed_component) {
        if (Component *component = MonoScriptingDriver::GetNativeObjectAs<Component>(managed_component)) {
            return MonoScriptingDriver::GetOrCreateManagedObject(component->GetEntity(), Type::get<Entity>());
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_GetTransform(MonoObject *managed_entity) {
        if (Entity *entity = MonoScriptingDriver::GetNativeObjectAs<Entity>(managed_entity)) {
            return MonoScriptingDriver::GetOrCreateManagedObject(entity->GetTransform(), Type::get<Transform>());
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_GetWorld(MonoObject *managed_entity) {
        if (Entity *entity = MonoScriptingDriver::GetNativeObjectAs<Entity>(managed_entity)) {
            return MonoScriptingDriver::GetOrCreateManagedObjectRaw(entity->GetWorld(), Type::get<World>());
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
        MonoScriptingDriver::RegisterManagedObject(managed_object, native_entity);
    }

    MonoObject *Binding_Entity_AddComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        if (Entity *entity = MonoScriptingDriver::GetNativeObjectAs<Entity>(managed_entity)) {
            MonoType *type = mono_reflection_type_get_type(reflection_type);
            HYP_ASSERT(type);
            MonoClass *managed_class = mono_type_get_class(type);
            HYP_ASSERT(managed_class);
            
            Type native_class = MonoScriptingDriver::GetNativeClass(managed_class);
            Component *component = entity->AddComponent(native_class);
            return MonoScriptingDriver::GetOrCreateManagedObject(component, native_class);
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_GetComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        if (Entity *entity = MonoScriptingDriver::GetNativeObjectAs<Entity>(managed_entity)) {
            MonoType *type = mono_reflection_type_get_type(reflection_type);
            HYP_ASSERT(type);
            MonoClass *managed_class = mono_type_get_class(type);
            HYP_ASSERT(managed_class);

            Type native_class = MonoScriptingDriver::GetNativeClass(managed_class);
            Component *component = entity->GetComponent(native_class);
            if (component != nullptr) {
                return MonoScriptingDriver::GetOrCreateManagedObject(component, component->GetType());
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_CreatePrimitive(EntityPrimitive primitive) {
        Entity *native_entity = Entity::CreatePrimitive(primitive);
        return MonoScriptingDriver::CreateManagedObject(native_entity, Type::get<Entity>());
    }

    MonoString *Binding_Object_GetName(MonoObject *managed_object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObjectAs<Object>(managed_object)) {
            return mono_string_new(MonoScriptingDriver::GetDomain(), native_object->GetName().c_str());
        } else {
            return nullptr;
        }
    }

    void Binding_Object_SetName(MonoObject *managed_object, MonoString *managed_name) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObjectAs<Object>(managed_object)) {
            char *native_name = mono_string_to_utf8(managed_name);
            native_object->SetName(native_name);
            mono_free(native_name);
        }
    }

    void Binding_Object_Destroy(MonoObject *managed_object) {
        if (Object *native_object = MonoScriptingDriver::GetNativeObjectAs<Object>(managed_object)) {
            Object::Destroy(native_object);
        }
    }

    bool Binding_Object_IsNativeAlive(MonoObject *managed_object) {
        return MonoScriptingDriver::IsRegisterdObject(managed_object);
    }

    MonoObject *Binding_Renderer_GetMaterial(MonoObject *managed_renderer) {
        if (Renderer *renderer = MonoScriptingDriver::GetNativeObjectAs<Renderer>(managed_renderer)) {
            Material *material = renderer->GetMaterial();
            return MonoScriptingDriver::GetOrCreateManagedObject(material, Type::get<Material>());
        } else {
            return nullptr;
        }
    }

    void Binding_Transform_GetPosition(MonoObject *managed_transform, Vec3 *position) {
        if (Transform *transform = MonoScriptingDriver::GetNativeObjectAs<Transform>(managed_transform)) {
            *position = transform->GetPosition();
        }
    }

    void Binding_Transform_SetPosition(MonoObject *managed_transform, Vec3 *position) {
        if (Transform *transform = MonoScriptingDriver::GetNativeObjectAs<Transform>(managed_transform)) {
            transform->SetPosition(*position);
        }
    }

    void MonoScriptingBindings::Bind() {
        // Register classes
        {
            MonoScriptingDriver::RegisterClass(Type::get<Behaviour>(), "Hyperion", "Behaviour");
            MonoScriptingDriver::RegisterClass(Type::get<BoxCollider>(), "Hyperion", "BoxCollider");
            MonoScriptingDriver::RegisterClass(Type::get<Camera>(), "Hyperion", "Camera");
            MonoScriptingDriver::RegisterClass(Type::get<Collider>(), "Hyperion", "Collider");
            MonoScriptingDriver::RegisterClass(Type::get<Component>(), "Hyperion", "Component");
            MonoScriptingDriver::RegisterClass(Type::get<Entity>(), "Hyperion", "Entity");
            MonoScriptingDriver::RegisterClass(Type::get<Object>(), "Hyperion", "Object");
            MonoScriptingDriver::RegisterClass(Type::get<Material>(), "Hyperion", "Material");
            MonoScriptingDriver::RegisterClass(Type::get<MeshRenderer>(), "Hyperion", "MeshRenderer");
            MonoScriptingDriver::RegisterClass(Type::get<Renderer>(), "Hyperion", "Renderer");
            MonoScriptingDriver::RegisterClass(Type::get<SphereCollider>(), "Hyperion", "SphereCollider");
            MonoScriptingDriver::RegisterClass(Type::get<Transform>(), "Hyperion", "Transform");
            MonoScriptingDriver::RegisterClass(Type::get<World>(), "Hyperion", "World");
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
            mono_add_internal_call("Hyperion.Time::Binding_GetElapsedTime", Time::GetTime);
            mono_add_internal_call("Hyperion.Time::Binding_GetDeltaTime", Time::GetDeltaTime);
            mono_add_internal_call("Hyperion.Time::Binding_GetFixedDeltaTime", Time::GetFixedDeltaTime);
        }

        // Transform
        {
            mono_add_internal_call("Hyperion.Transform::Binding_GetPosition", Binding_Transform_GetPosition);
            mono_add_internal_call("Hyperion.Transform::Binding_SetPosition", Binding_Transform_SetPosition);
        }

        // Renderer
        {
            mono_add_internal_call("Hyperion.Renderer::Binding_GetMaterial", Binding_Renderer_GetMaterial);
        }
    }

}