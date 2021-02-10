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
#include "hyperion/modules/mono/mono_scripting_instance.hpp"

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
        MonoScriptingDriver::RegisterManagedObject(managed_object, native_entity, false);
    }

    MonoObject *Binding_Entity_AddComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        if (Entity *entity = MonoScriptingDriver::GetNativeObjectAs<Entity>(managed_entity)) {
            MonoType *managed_type = mono_reflection_type_get_type(reflection_type);
            HYP_ASSERT(managed_type);
            MonoClass *managed_class = mono_type_get_class(managed_type);
            HYP_ASSERT(managed_class);
            
            // First check that we are actually adding a component type that derives from 'Hyperion.Component'.
            if (!mono_class_is_subclass_of(managed_class, MonoScriptingDriver::GetComponentClass(), false)) {
                // TODO: We need to define a custom exception we can throw here.
                return nullptr;
            }

            // FIXME: Make sure we are not trying to create an abstract class.

            bool is_native_component = MonoScriptingDriver::IsNativeClass(managed_class);
            ComponentType component_type = nullptr;
            if (is_native_component) {
                component_type = MonoScriptingDriver::GetNativeClass(managed_class);
            } else {
                component_type = MonoScriptingDriver::GetOrCreateScriptingType(managed_class);
            }

            // NOTE: For now we prevent from adding the same script more than once.
            if (entity->GetComponent(component_type) != nullptr) {
                return nullptr;
            }

            if (is_native_component) {
                Type native_class = component_type.GetNativeType();
                Component *component = entity->AddComponent(native_class);
                return MonoScriptingDriver::GetOrCreateManagedObject(component, native_class);
            } else {
                Script *script = entity->AddScript(component_type.GetScriptingType());

                bool is_script_component = mono_class_is_subclass_of(managed_class, MonoScriptingDriver::GetScriptClass(), false);
                MonoObject *managed_object = MonoScriptingDriver::CreateManagedObjectFromManagedType(script, managed_class, is_script_component);

                // We have to wait for the actual managed object to be create, so that we can send the OnCreate message.
                script->SendMessage(ScriptingMessage::OnCreate);

                return managed_object;
            }
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_GetComponent(MonoObject *managed_entity, MonoReflectionType *reflection_type) {
        if (Entity *entity = MonoScriptingDriver::GetNativeObjectAs<Entity>(managed_entity)) {
            MonoType *managed_type = mono_reflection_type_get_type(reflection_type);
            HYP_ASSERT(managed_type);
            MonoClass *managed_class = mono_type_get_class(managed_type);
            HYP_ASSERT(managed_class);

            // First check that we are actually searching for a component type that derives from 'Hyperion.Component'.
            if (!mono_class_is_subclass_of(managed_class, MonoScriptingDriver::GetComponentClass(), false)) {
                // TODO: We need to define a custom exception we can throw here.
                return nullptr;
            }

            bool is_native_component = MonoScriptingDriver::IsNativeClass(managed_class);
            ComponentType component_type = nullptr;
            if (is_native_component) {
                component_type = MonoScriptingDriver::GetNativeClass(managed_class);
            } else {
                component_type = MonoScriptingDriver::GetOrCreateScriptingType(managed_class);
            }

            Component *component = entity->GetComponent(component_type);
            if (component == nullptr) {
                return nullptr;
            }

            if (is_native_component) {
                return MonoScriptingDriver::GetOrCreateManagedObject(component, component->GetType());
            } else {
                HYP_ASSERT(component->GetType() == Type::get<Script>());
                return static_cast<MonoScriptingInstance *>(component->GetScriptingInstance())->GetManagedObject();
            }
        } else {
            return nullptr;
        }
    }

    MonoObject *Binding_Entity_CreatePrimitive(EntityPrimitive primitive) {
        Entity *native_entity = Entity::CreatePrimitive(primitive);
        return MonoScriptingDriver::CreateManagedObjectFromNativeType(native_entity, Type::get<Entity>());
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