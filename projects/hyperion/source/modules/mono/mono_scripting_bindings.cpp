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
#include "hyperion/modules/mono/mono_scripting_driver.hpp"
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
            mono_raise_exception(MonoScriptingStorage::GetException(MonoSpecialExceptionClass::ObjectDestroyed));
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
            mono_raise_exception(MonoScriptingStorage::GetException(MonoSpecialExceptionClass::InvalidComponentType));
            return false;
        } else {
            return true;
        }
    }

    //--------------------------------------------------------------
    void Binding_Behaviour_IsActiveAndEnabled(MonoObject *mono_behaviour, bool *is_active_and_enabled) {
        Behaviour *behaviour = GetScriptingObjectAs<Behaviour>(mono_behaviour);
        *is_active_and_enabled = behaviour->IsActiveAndEnabled();
    }

    //--------------------------------------------------------------
    void Binding_Behaviour_IsEnabled(MonoObject *mono_behaviour, bool *is_enabled) {
        Behaviour *behaviour = GetScriptingObjectAs<Behaviour>(mono_behaviour);
        *is_enabled = behaviour->IsEnabled();
    }

    //--------------------------------------------------------------
    void Binding_Behaviour_SetEnabled(MonoObject *mono_behaviour, bool *is_enabled) {
        Behaviour *behaviour = GetScriptingObjectAs<Behaviour>(mono_behaviour);
        behaviour->SetEnabled(*is_enabled);
    }

    //--------------------------------------------------------------
    void Binding_BoxCollider_GetOrigin(MonoObject *mono_box_collider, Vec3 *origin) {
        BoxCollider *box_collider = GetScriptingObjectAs<BoxCollider>(mono_box_collider);
        *origin = box_collider->GetOrigin();
    }

    //--------------------------------------------------------------
    void Binding_BoxCollider_SetOrigin(MonoObject *mono_box_collider, Vec3 *origin) {
        BoxCollider *box_collider = GetScriptingObjectAs<BoxCollider>(mono_box_collider);
        box_collider->SetOrigin(*origin);
    }

    //--------------------------------------------------------------
    void Binding_BoxCollider_GetSize(MonoObject *mono_box_collider, Vec3 *size) {
        BoxCollider *box_collider = GetScriptingObjectAs<BoxCollider>(mono_box_collider);
        *size = box_collider->GetSize();
    }

    //--------------------------------------------------------------
    void Binding_BoxCollider_SetSize(MonoObject *mono_box_collider, Vec3 *size) {
        BoxCollider *box_collider = GetScriptingObjectAs<BoxCollider>(mono_box_collider);
        box_collider->SetSize(*size);
    }

    //--------------------------------------------------------------
    void Binding_Collider_GetBoundingBox(MonoObject *mono_collider, BoundingBox *bounding_box) {
        Collider *collider = GetScriptingObjectAs<Collider>(mono_collider);
        *bounding_box = collider->GetBoundingBox();
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
                MonoCustomAttrInfo *custom_attribute_info = mono_custom_attrs_from_class(mono_component_class);

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
    void Binding_SphereCollider_GetRadius(MonoObject *mono_sphere_collider, float32 *radius) {
        SphereCollider *sphere_collider = GetScriptingObjectAs<SphereCollider>(mono_sphere_collider);
        *radius = sphere_collider->GetRadius();
    }

    //--------------------------------------------------------------
    void Binding_SphereCollider_SetRadius(MonoObject *mono_sphere_collider, float32 *radius) {
        SphereCollider *sphere_collider = GetScriptingObjectAs<SphereCollider>(mono_sphere_collider);
        sphere_collider->SetRadius(*radius);
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
    MonoString *Binding_World_GetName(MonoObject *mono_world) {
        World *world = GetScriptingObjectAs<World>(mono_world);
        return MonoManagedString(world->GetName()).GetMonoString();
    }

    //--------------------------------------------------------------
    void Binding_World_SetName(MonoObject *mono_world, MonoString *name) {
        World *world = GetScriptingObjectAs<World>(mono_world);
        world->SetName(MonoManagedString(name).GetString());
    }

    //--------------------------------------------------------------
    MonoArray *Binding_World_GetRootEntities(MonoObject *mono_world) {
        World *world = GetScriptingObjectAs<World>(mono_world);
        const Vector<Entity *> &root_entities = world->GetRootEntites();

        // TODO: Move all array related things into own MonoManagedArray class.
        MonoDomain *mono_domain = MonoScriptingDriver::GetRuntimeDomain()->GetMonoDomain();
        MonoClass *mono_entity_class = MonoScriptingStorage::GetSpecialClass(MonoSpecialClass::Entity);
        MonoArray *mono_array = mono_array_new(mono_domain, mono_entity_class, root_entities.size());

        for (uint32 i = 0; i < root_entities.size(); i++) 		{
            mono_array_set(mono_array, MonoObject *, i, MonoScriptingStorage::GetOrCreateMonoObject(root_entities[i]));
        }

        return mono_array;
    }

    //--------------------------------------------------------------
    void MonoScriptingBindings::Bind() {
        // Behaviour
        {
            mono_add_internal_call("Hyperion.Behaviour::Binding_IsActiveAndEnabled", Binding_Behaviour_IsActiveAndEnabled);
            mono_add_internal_call("Hyperion.Behaviour::Binding_IsEnabled", Binding_Behaviour_IsEnabled);
            mono_add_internal_call("Hyperion.Behaviour::Binding_SetEnabled", Binding_Behaviour_SetEnabled);
        }
        
        // BoxCollider
        {
            mono_add_internal_call("Hyperion.BoxCollider::Binding_GetOrigin", Binding_BoxCollider_GetOrigin);
            mono_add_internal_call("Hyperion.BoxCollider::Binding_SetOrigin", Binding_BoxCollider_SetOrigin);
            mono_add_internal_call("Hyperion.BoxCollider::Binding_GetSize", Binding_BoxCollider_GetSize);
            mono_add_internal_call("Hyperion.BoxCollider::Binding_SetSize", Binding_BoxCollider_SetSize);
        }

        // Collider
        {
            mono_add_internal_call("Hyperion.Collider::Binding_GetBoundingBox", Binding_Collider_GetBoundingBox);
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

        // Input
        {
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyDown", Input::IsKeyDown);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyHold", Input::IsKeyHold);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsKeyUp);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsMouseButtonDown);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsMouseButtonHold);
            mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Input::IsKeyUp);
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

        // SphereCollider
        {
            mono_add_internal_call("Hyperion.SphereCollider::Binding_GetRadius", Binding_SphereCollider_GetRadius);
            mono_add_internal_call("Hyperion.SphereCollider::Binding_SetRadius", Binding_SphereCollider_SetRadius);
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

        // World
        {
            mono_add_internal_call("Hyperion.World::Binding_GetName", Binding_World_GetName);
            mono_add_internal_call("Hyperion.World::Binding_SetName", Binding_World_SetName);
            mono_add_internal_call("Hyperion.World::Binding_GetRootEntities", Binding_World_GetRootEntities);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingBindings::RegisterClasses() {
        MonoScriptingStorage::RegisterNativeClass<Behaviour>("Hyperion", "Behaviour");
        MonoScriptingStorage::RegisterNativeClass<BoxCollider>("Hyperion", "BoxCollider");
        MonoScriptingStorage::RegisterNativeClass<Collider>("Hyperion", "Collider");
        MonoScriptingStorage::RegisterNativeClass<Component>("Hyperion", "Component", MonoSpecialClass::Component);
        MonoScriptingStorage::RegisterNativeClass<Entity>("Hyperion", "Entity", MonoSpecialClass::Entity);
        MonoScriptingStorage::RegisterNativeClass<Object>("Hyperion", "Object");
        MonoScriptingStorage::RegisterNativeClass<Script>("Hyperion", "Script", MonoSpecialClass::Script);
        MonoScriptingStorage::RegisterNativeClass<SphereCollider>("Hyperion", "SphereCollider");
        MonoScriptingStorage::RegisterNativeClass<Transform>("Hyperion", "Transform");
        MonoScriptingStorage::RegisterNativeClass<World>("Hyperion", "World");
    }

}