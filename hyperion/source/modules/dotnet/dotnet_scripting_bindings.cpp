//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/dotnet/dotnet_scripting_bindings.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/ecs/world/world_manager.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  //--------------------------------------------------------------
  void DotnetScriptingBindings::Initialize() {
    s_core_bootstrap_arguments.native_bindings.core.exception = [](ManagedString message) {
      HYP_LOG_ERROR("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log.log_trace = [](ManagedString message) {
      HYP_LOG_TRACE("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log.log_info = [](ManagedString message) {
      HYP_LOG_INFO("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log.log_warn = [](ManagedString message) {
      HYP_LOG_WARN("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log.log_error = [](ManagedString message) {
      HYP_LOG_ERROR("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.time.get_time = []() {
      return Time::GetTime();
    };
    s_core_bootstrap_arguments.native_bindings.time.get_delta_time = []() {
      return Time::GetDeltaTime();
    };
    s_core_bootstrap_arguments.native_bindings.input.is_key_down = [](KeyCode key_code) {
      return Input::IsKeyDown(key_code);
    };
    s_core_bootstrap_arguments.native_bindings.input.is_key_hold = [](KeyCode key_code) {
      return Input::IsKeyHold(key_code);
    };
    s_core_bootstrap_arguments.native_bindings.input.is_key_up = [](KeyCode key_code) {
      return Input::IsKeyUp(key_code);
    };
    s_core_bootstrap_arguments.native_bindings.world_manager.get_active_world = []() {
      World *world = WorldManager::GetActiveWorld();
      return GetOrCreateManagedObject(GetSpecialType(SpecialType::World), world);
    };
    s_core_bootstrap_arguments.native_bindings.world.get_name = [](NativeHandle native_handle) {
      World *world = static_cast<World *>(native_handle);
      return world ? world->GetName().c_str() : nullptr;
    };
    s_core_bootstrap_arguments.native_bindings.world.set_name = [](NativeHandle native_handle, ManagedString name) {
      World *world = static_cast<World *>(native_handle);
      world->SetName(String(name));
    };
    s_core_bootstrap_arguments.native_bindings.world.get_entity_manager = [](NativeHandle native_handle) {
      World *world = static_cast<World *>(native_handle);
      EntityManager *entity_manager = world->GetEntityManager();
      return GetOrCreateManagedObject(GetSpecialType(SpecialType::EntityManager), entity_manager);
    };
    s_core_bootstrap_arguments.native_bindings.entity_manager.get_entity_count = [](NativeHandle native_handle) {
      EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
      return static_cast<uint32>(entity_manager->GetEntityCount());
    };
    s_core_bootstrap_arguments.native_bindings.entity_manager.create_entity = [](NativeHandle native_handle, EntityId *id) {
      EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
      *id = entity_manager->CreateEntity(); 
    };
    s_core_bootstrap_arguments.native_bindings.entity_manager.destroy_entity = [](NativeHandle native_handle, EntityId id) {
      EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
      entity_manager->DestroyEntity(id);
    };
    s_core_bootstrap_arguments.native_bindings.entity_manager.has_component = [](NativeHandle native_handle, ManagedHandle type_handle, EntityId id) {
      ComponentId component_id = GetComponentIdForManagedType(type_handle);
      if (component_id == ComponentInfo::INVALID_ID) {
        return false;
      } else {
        EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
        return entity_manager->HasComponent(component_id, id);
      }
    };
    s_core_bootstrap_arguments.native_bindings.entity_manager.remove_component = [](NativeHandle native_handle, ManagedHandle type_handle, EntityId id) {
      ComponentId component_id = GetComponentIdForManagedType(type_handle);
      if (component_id != ComponentInfo::INVALID_ID) {
        EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
        entity_manager->RemoveComponent(component_id, id);
      }
    };
    s_core_bootstrap_arguments.native_bindings.name_component.get_name = [](NativeHandle native_handle, EntityId id) {
      EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
      NameComponent *name_component = entity_manager->GetComponent<NameComponent>(id);
      return name_component->name.c_str();
    };
    s_core_bootstrap_arguments.native_bindings.name_component.set_name = [](NativeHandle native_handle, EntityId id, ManagedString name) {
      EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
      NameComponent *name_component = entity_manager->GetComponent<NameComponent>(id);
      name_component->name = name;
    };

    // This is the callback were we get the pointers to the managed bindings.
    s_core_bootstrap_arguments.managed_bindings_callback = [](CoreManagedBindings *core_managed_bindings) {
      s_core_managed_bindings = *core_managed_bindings;

      // This is also the point where we can grab references to types in the core assembly.
      RegisterSpecialType(SpecialType::World, "Hyperion.Ecs.World");
      RegisterSpecialType(SpecialType::EntityManager, "Hyperion.Ecs.EntityManager");

      RegisterComponentType<NameComponent>("Hyperion.Ecs.NameComponent");
    };
  }

  //--------------------------------------------------------------
  void DotnetScriptingBindings::UnloadMappings() {
    // This cleanup is very important as we would otherwise leak a lot of memory.
    // Every GCHandle we ever allocated needs to be freed now.
    
    // We need to free the managed object (GCHandle).
    for (auto [native_handle, managed_handle] : s_object_mappings) {
      s_core_managed_bindings.destroy_managed_object(managed_handle);
    }
    s_object_mappings.Clear();

    // Types also need to be cleared.
    for (uint64 i = 0; i < static_cast<uint64>(SpecialType::Last); i++) {
      s_core_managed_bindings.destroy_type(s_special_types[i]);
      s_special_types[i] = nullptr;
    }
    
    for (auto [managed_handle, component_id] : s_managed_component_types_to_id) {
      s_core_managed_bindings.destroy_type(managed_handle);
    }
    s_id_to_managed_component_types.Clear();
    s_managed_component_types_to_id.Clear();
  }

  //--------------------------------------------------------------
  ManagedHandle DotnetScriptingBindings::GetOrCreateManagedObject(ManagedHandle type, NativeHandle native_handle) {
    auto it = s_object_mappings.Find(native_handle);
    if (it == s_object_mappings.end()) {
      ManagedHandle managed_handle = s_core_managed_bindings.create_managed_object(type, native_handle);
      s_object_mappings.Insert(native_handle, managed_handle);
      return managed_handle;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingBindings::DestroyManagedObject(NativeHandle native_handle, ManagedHandle managed_handle) {
    s_core_managed_bindings.destroy_managed_object(managed_handle);
    s_object_mappings.Remove(native_handle);
  }

  //--------------------------------------------------------------
  ComponentId DotnetScriptingBindings::GetComponentIdForManagedType(ManagedHandle component_type_handle) {
    auto it = s_managed_component_types_to_id.Find(component_type_handle);
    if (it == s_managed_component_types_to_id.end()) {
      HYP_LOG_ERROR("Scripting", "Failed to find component id for managed type!");
      return ComponentInfo::INVALID_ID;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  ManagedHandle DotnetScriptingBindings::GetManagedTypeForComponentId(ComponentId component_id) {
    auto it = s_id_to_managed_component_types.Find(component_id);
    if (it == s_id_to_managed_component_types.end()) {
      HYP_LOG_ERROR("Scripting", "Failed to find managed type for component id!");
      return nullptr;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingBindings::RegisterSpecialType(SpecialType special_type, const char *full_name) {
    s_special_types[static_cast<uint32>(special_type)] = s_core_managed_bindings.get_type_by_name(full_name);
  }

}
