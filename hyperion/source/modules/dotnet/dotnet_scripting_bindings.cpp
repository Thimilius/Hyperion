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
    s_core_bootstrap_arguments.native_bindings.core_bindings.exception = [](ManagedString message) {
      HYP_LOG_ERROR("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log_bindings.log_trace = [](ManagedString message) {
      HYP_LOG_TRACE("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log_bindings.log_info = [](ManagedString message) {
      HYP_LOG_INFO("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log_bindings.log_warn = [](ManagedString message) {
      HYP_LOG_WARN("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.log_bindings.log_error = [](ManagedString message) {
      HYP_LOG_ERROR("Scripting", "{}", message);
    };
    s_core_bootstrap_arguments.native_bindings.world_manager_bindings.get_active_world = []() {
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
    s_core_bootstrap_arguments.native_bindings.world_bindings.get_name = [](NativeHandle native_handle) {
      World *world = static_cast<World *>(native_handle);
      return world ? world->GetName().c_str() : nullptr;
    };
    s_core_bootstrap_arguments.native_bindings.world_bindings.set_name = [](NativeHandle native_handle, ManagedString name) {
      World *world = static_cast<World *>(native_handle);
      world->SetName(String(name));
    };
    s_core_bootstrap_arguments.managed_bindings_callback = [](CoreManagedBindings *core_managed_bindings) {
      s_core_managed_bindings = *core_managed_bindings;

      // This is also the point where we can grab references to types in the core assembly.
      s_type_world = s_core_managed_bindings.get_type_by_name("Hyperion.World");
    };
  }
  
}
