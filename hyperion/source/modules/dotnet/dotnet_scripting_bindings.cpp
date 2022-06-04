//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/dotnet/dotnet_scripting_bindings.hpp"

//---------------------- Project Includes ----------------------
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
    s_core_bootstrap_arguments.native_bindings.world_manager.get_active_world = []() {
      World *world = WorldManager::GetActiveWorld();
      auto it = s_object_mappings.Find(world);
      if (it == s_object_mappings.end()) {
        ManagedHandle managed_handle = s_core_managed_bindings.create_managed_object(s_type_world, world);
        s_object_mappings.Insert(world, managed_handle); 
        return managed_handle;
      } else {
        return it->second;
      }
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
      auto it = s_object_mappings.Find(entity_manager);
      if (it == s_object_mappings.end()) {
        ManagedHandle managed_handle = s_core_managed_bindings.create_managed_object(s_type_entity_manager, entity_manager);
        s_object_mappings.Insert(entity_manager, managed_handle);
        return managed_handle;
      } else {
        return it->second;
      }
    };
    s_core_bootstrap_arguments.native_bindings.entity_manager.get_entity_count = [](NativeHandle native_handle) {
      EntityManager *entity_manager = static_cast<EntityManager *>(native_handle);
      return static_cast<uint32>(entity_manager->GetEntityCount());
    };

    // This is the callback were we get the pointers to the managed bindings.
    s_core_bootstrap_arguments.managed_bindings_callback = [](CoreManagedBindings *core_managed_bindings) {
      s_core_managed_bindings = *core_managed_bindings;

      // This is also the point where we can grab references to types in the core assembly.
      s_type_world = s_core_managed_bindings.get_type_by_name("Hyperion.Ecs.World");
      s_type_entity_manager = s_core_managed_bindings.get_type_by_name("Hyperion.Ecs.EntityManager");
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
    s_core_managed_bindings.destroy_type(s_type_world);
    s_type_world = nullptr;
    s_core_managed_bindings.destroy_type(s_type_entity_manager);
    s_type_entity_manager = nullptr;
  }

}
