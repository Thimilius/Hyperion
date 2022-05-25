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
  
  struct NativeBindings {
    LogBindings log_bindings;
    WorldManagerBindings world_manager_bindings;
    WorldBindings world_bindings;
  };
  
  struct ManagedBindings {
    void (*engine_initialize)();
    void (*engine_update)();
    void (*engine_shutdown)();

    void *(*get_type_by_name)(const char *);
    void *(*create_managed_object)(void *, void *);
  };

  struct BootstrapArguments {
    NativeBindings native_function_pointers;
    void (*function_pointers_callback)(ManagedBindings *);
  };
  
  ManagedBindings g_managed_bindings;
  Map<World *, void *> g_object_mappings;
  void *g_type_world;
  
  //--------------------------------------------------------------
  void DotnetScriptingDriver::Initialize() {
    String runtime_host_path = EngineConfig::GetToolsPath() + "/dotnet/host/fxr/6.0.5/hostfxr.dll";
    String managed_libraries_path = EngineConfig::GetManagedLibrariesPath();
    String core_library_name = "Hyperion";
    WideString hostfxr_config_path = StringUtils::Utf8ToUtf16(managed_libraries_path + "/" + core_library_name + ".runtimeconfig.json");
    
    String library_path = managed_libraries_path + "/" + core_library_name + ".dll";
    WideString library_path_wide = StringUtils::Utf8ToUtf16(library_path);
    String dotnet_type = "Hyperion.Bootstrapper, Hyperion";
    WideString dotnet_type_wide = StringUtils::Utf8ToUtf16(dotnet_type);
    String dotnet_type_method = "Bootstrap";
    WideString dotnet_type_method_wide = StringUtils::Utf8ToUtf16(dotnet_type_method);

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
    
    int32 (*bootstrap_func)(BootstrapArguments *) = nullptr;
    result = load_assembly_and_get_function_pointer_func(
      library_path_wide.c_str(),
      dotnet_type_wide.c_str(),
      dotnet_type_method_wide.c_str(),
      UNMANAGEDCALLERSONLY_METHOD,
      nullptr,
      reinterpret_cast<void **>(&bootstrap_func)
    );
    if (result) {
      close_func(hostfxr_context);
      HYP_PANIC_MESSAGE("Scripting", "Failed to load assembly {} and get function pointer for {}-{}!", library_path, dotnet_type, dotnet_type_method);
    }

    close_func(hostfxr_context);
    
    
    BootstrapArguments args = { };
    args.native_function_pointers.log_bindings.log_trace = [](const char *message) {
      HYP_TRACE(message);
    };
    args.native_function_pointers.log_bindings.log_info = [](const char *message) {
      HYP_INFO(message);
    };
    args.native_function_pointers.log_bindings.log_warn = [](const char *message) {
      HYP_WARN(message);
    };
    args.native_function_pointers.log_bindings.log_error = [](const char *message) {
      HYP_ERROR(message);
    };
    args.native_function_pointers.world_manager_bindings.get_active_world = []() {
      World *world = WorldManager::GetActiveWorld();
      auto it = g_object_mappings.Find(world);
      if (it == g_object_mappings.end()) {
        void *handle = g_managed_bindings.create_managed_object(g_type_world, world);
        g_object_mappings.Insert(world, handle); 
        return handle;
      } else {
        return it->second;
      }
    };
    args.native_function_pointers.world_bindings.get_name = [](void *native_handle) {
      World *world = static_cast<World *>(native_handle);
      return world ? world->GetName().c_str() : nullptr;
    };
    args.native_function_pointers.world_bindings.set_name = [](void *native_handle, const char *name) {
      World *world = static_cast<World *>(native_handle);
      world->SetName(String(name));
    };
    args.function_pointers_callback = [](ManagedBindings *pointers) { g_managed_bindings = *pointers; };

    if (bootstrap_func(&args)) {
      HYP_PANIC_MESSAGE("Dotnet", "Failed to bootstrap core assembly!");
    }
      
    g_type_world = g_managed_bindings.get_type_by_name("Hyperion.World");

    HYP_LOG_INFO("Scripting", "Initialized .Net6.0 runtime.");
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::PostInitialize() {
    g_managed_bindings.engine_initialize();
  }
  
  //--------------------------------------------------------------
  void DotnetScriptingDriver::Update() {
    g_managed_bindings.engine_update();
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::Shutdown() {
    g_managed_bindings.engine_shutdown();
  }

}
