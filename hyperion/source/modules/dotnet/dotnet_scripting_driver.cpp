//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/dotnet/dotnet_scripting_driver.hpp"

//---------------------- Library Includes ----------------------
#include <dotnet/hostfxr.h>
#include <dotnet/coreclr_delegates.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/operating_system.hpp"
#include "hyperion/ecs/world/world_manager.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  struct CoreBindings {
    void (*exception)(const char *);
  };
  
  struct LogBindings {
    void (*log_trace)(const char *);
    void (*log_info)(const char *);
    void (*log_warn)(const char *);
    void (*log_error)(const char *);
  };

  struct WorldManagerBindings {
    void *(*get_active_world)();
  };

  struct WorldBindings {
    const char *(*get_name)(void *);
    void (*set_name)(void *, const char *);
  };
  
  struct CoreNativeBindings {
    CoreBindings core_bindings;
    LogBindings log_bindings;
    WorldManagerBindings world_manager_bindings;
    WorldBindings world_bindings;
  };
  
  struct CoreManagedBindings {
    void (*engine_initialize)();
    void (*engine_update)();
    void (*engine_shutdown)();

    void *(*get_type_by_name)(const char *);
    void *(*create_managed_object)(void *, void *);
  };

  struct CoreBootstrapArguments {
    CoreNativeBindings native_bindings;
    void (*managed_bindings_callback)(CoreManagedBindings *);
  };
  
  struct RuntimeNativeBindings {
    void (*exception)(const char *);
  };

  struct RuntimeManagedBindings {
    void (*load_context)(CoreBootstrapArguments *);
    void (*unload_context)();
  };

  struct RuntimeBoostrapArguments {
    RuntimeNativeBindings native_bindings;
    void (*managed_bindings_callback)(RuntimeManagedBindings *);
  };

  RuntimeManagedBindings g_runtime_managed_bindings;

  CoreManagedBindings g_core_managed_bindings;
  CoreBootstrapArguments g_core_bootstrap_arguments;
  Map<World *, void *> g_object_mappings;
  void *g_type_world;
  
  //--------------------------------------------------------------
  void DotnetScriptingDriver::Initialize() {
    String runtime_host_path = EngineConfig::GetToolsPath() + "/dotnet/host/fxr/6.0.5/hostfxr.dll";
    String managed_libraries_path = EngineConfig::GetManagedLibrariesPath();
    String runtime_library_name = "Hyperion.Runtime";
    WideString hostfxr_config_path = StringUtils::Utf8ToUtf16(managed_libraries_path + "/" + runtime_library_name + ".runtimeconfig.json");
    
    String library_path = managed_libraries_path + "/" + runtime_library_name + ".dll";
    WideString library_path_wide = StringUtils::Utf8ToUtf16(library_path);
    String runtime_type = "Hyperion.Runtime, " + runtime_library_name;
    WideString dotnet_type_wide = StringUtils::Utf8ToUtf16(runtime_type);
    String runtime_bootstrap_method = "Bootstrap";
    WideString dotnet_type_method_wide = StringUtils::Utf8ToUtf16(runtime_bootstrap_method);

    int32 result = 0;
    
    LibraryHandle hostfxr = OperatingSystem::LoadLibrary(runtime_host_path);
    auto init_func = OperatingSystem::GetFunctionPointer<hostfxr_initialize_for_runtime_config_fn>(hostfxr, "hostfxr_initialize_for_runtime_config");
    auto get_delegate_func = OperatingSystem::GetFunctionPointer<hostfxr_get_runtime_delegate_fn>(hostfxr, "hostfxr_get_runtime_delegate");
    auto close_func = OperatingSystem::GetFunctionPointer<hostfxr_close_fn>(hostfxr, "hostfxr_close");
    
    hostfxr_handle hostfxr_context;
    result = init_func(hostfxr_config_path.c_str(), nullptr, &hostfxr_context);
    if (result) {
      HYP_PANIC_MESSAGE("Scripting", "Failed to initialized hostfxr runtime!");
    }

    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer_func;
    result = get_delegate_func(
      hostfxr_context,
      hdt_load_assembly_and_get_function_pointer,
      reinterpret_cast<void **>(&load_assembly_and_get_function_pointer_func)
    );
    if (result) {
      close_func(hostfxr_context);
      HYP_PANIC_MESSAGE("Scripting", "Failed to load function from hostfxr: load_assembly_and_get_function_pointer!");
    }
    
    int32 (*runtime_bootstrap_func)(RuntimeBoostrapArguments *) = nullptr;
    result = load_assembly_and_get_function_pointer_func(
      library_path_wide.c_str(),
      dotnet_type_wide.c_str(),
      dotnet_type_method_wide.c_str(),
      UNMANAGEDCALLERSONLY_METHOD,
      nullptr,
      reinterpret_cast<void **>(&runtime_bootstrap_func)
    );
    if (result) {
      close_func(hostfxr_context);
      HYP_PANIC_MESSAGE("Scripting", "Failed to load assembly {} and get function pointer for {}-{}!", library_path, runtime_type, runtime_bootstrap_method);
    }

    close_func(hostfxr_context);

    // The native bindings can be cached once globally and do not have to be recreated on context reload.
    g_core_bootstrap_arguments.native_bindings.core_bindings.exception = [](const char *message) {
      HYP_LOG_ERROR("Scripting", "{}", message);
    };
    g_core_bootstrap_arguments.native_bindings.log_bindings.log_trace = [](const char *message) {
      HYP_LOG_TRACE("Scripting", "{}", message);
    };
    g_core_bootstrap_arguments.native_bindings.log_bindings.log_info = [](const char *message) {
      HYP_LOG_INFO("Scripting", "{}", message);
    };
    g_core_bootstrap_arguments.native_bindings.log_bindings.log_warn = [](const char *message) {
      HYP_LOG_WARN("Scripting", "{}", message);
    };
    g_core_bootstrap_arguments.native_bindings.log_bindings.log_error = [](const char *message) {
      HYP_LOG_ERROR("Scripting", "{}", message);
    };
    g_core_bootstrap_arguments.native_bindings.world_manager_bindings.get_active_world = []() {
      World *world = WorldManager::GetActiveWorld();
      auto it = g_object_mappings.Find(world);
      if (it == g_object_mappings.end()) {
        void *handle = g_core_managed_bindings.create_managed_object(g_type_world, world);
        g_object_mappings.Insert(world, handle); 
        return handle;
      } else {
        return it->second;
      }
    };
    g_core_bootstrap_arguments.native_bindings.world_bindings.get_name = [](void *native_handle) {
      World *world = static_cast<World *>(native_handle);
      return world ? world->GetName().c_str() : nullptr;
    };
    g_core_bootstrap_arguments.native_bindings.world_bindings.set_name = [](void *native_handle, const char *name) {
      World *world = static_cast<World *>(native_handle);
      world->SetName(String(name));
    };
    g_core_bootstrap_arguments.managed_bindings_callback = [](CoreManagedBindings *core_managed_bindings) {
      g_core_managed_bindings = *core_managed_bindings;

      // This is also the point where we can grab references to types in the core assembly.
      g_type_world = g_core_managed_bindings.get_type_by_name("Hyperion.World");
    };

    // We bootstrap the runtime by providing a callback to get bindings to the managed runtime methods.
    RuntimeBoostrapArguments runtime_arguments = { };
    runtime_arguments.native_bindings.exception = [](const char *message) {
      HYP_PANIC_MESSAGE("Scripting", "{}", message);
    };
    runtime_arguments.managed_bindings_callback = [](RuntimeManagedBindings *runtime_managed_bindings) {
      HYP_LOG_INFO("Scripting", "Setting runtime bindings...");
      g_runtime_managed_bindings = *runtime_managed_bindings;
    };
    runtime_bootstrap_func(&runtime_arguments);

    HYP_LOG_INFO("Scripting", "Initialized .Net6.0 runtime!");
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::PostInitialize() {
    if (Engine::GetEngineState() == EngineState::Runtime) {
      LoadManagedContext();
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::OnEngineModeChanged(EngineState old_state, EngineState new_state) {
    // We do not reload the context in paused state.
    if (old_state == EngineState::EditorRuntimePaused || new_state == EngineState::EditorRuntimePaused) {
      return;
    }
    
    if (new_state == EngineState::EditorRuntimePlaying) {
      LoadManagedContext();
    } else if (new_state == EngineState::Editor) {
      UnloadManagedContext();
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::Update() {
    if (Engine::GetEngineState() == EngineState::Runtime || Engine::GetEngineState() == EngineState::EditorRuntimePlaying) {
      g_core_managed_bindings.engine_update();
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::Shutdown() {
    if (Engine::GetEngineState() != EngineState::Editor) {
      UnloadManagedContext();
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::LoadManagedContext() {
    g_runtime_managed_bindings.load_context(&g_core_bootstrap_arguments);
    HYP_LOG_INFO("Scripting", "Loaded managed context!");

    g_core_managed_bindings.engine_initialize();
  }
  
  //--------------------------------------------------------------
  void DotnetScriptingDriver::UnloadManagedContext() {
    g_core_managed_bindings.engine_shutdown();
    
    g_runtime_managed_bindings.unload_context();
    HYP_LOG_INFO("Scripting", "Unloaded managed context!");
  }

}
