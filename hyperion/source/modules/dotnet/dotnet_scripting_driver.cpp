﻿//----------------- Precompiled Header Include -----------------
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

  ManagedBindings g_managed_bindings;
  Map<World *, void *> g_object_mappings;
  void *g_type_world;
  
  //--------------------------------------------------------------
  void DotnetScriptingDriver::Initialize(const ScriptingSettings &settings) {
    LibraryHandle hostfxr = OperatingSystem::LoadLibrary(settings.runtime_host_path);
    auto init_func = OperatingSystem::GetFunctionPointer<hostfxr_initialize_for_runtime_config_fn>(hostfxr, "hostfxr_initialize_for_runtime_config");
    auto get_delegate_func = OperatingSystem::GetFunctionPointer<hostfxr_get_runtime_delegate_fn>(hostfxr, "hostfxr_get_runtime_delegate");
    auto close_func = OperatingSystem::GetFunctionPointer<hostfxr_close_fn>(hostfxr, "hostfxr_close");

    auto config_path = StringUtils::Utf8ToUtf16(settings.library_path + settings.core_library_name + ".runtimeconfig.json");
    
    hostfxr_handle context;
    int result = init_func(config_path.c_str(), nullptr, &context);
    assert(result == 0);

    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer;
    result = get_delegate_func(context, hdt_load_assembly_and_get_function_pointer, reinterpret_cast<void **>(&load_assembly_and_get_function_pointer));
    assert(result == 0);

    auto library_path = StringUtils::Utf8ToUtf16(settings.library_path + settings.core_library_name + ".dll");
    auto dotnet_type = L"Hyperion.Bootstrapper, Hyperion";
    auto dotnet_type_method = L"Bootstrap";
    // <SnippetLoadAndGet>
    // Function pointer to managed delegate
    component_entry_point_fn bootstrap = nullptr;
    int rc = load_assembly_and_get_function_pointer(
      library_path.c_str(),
      dotnet_type,
      dotnet_type_method,
      nullptr /*delegate_type_name*/,
      nullptr,
      reinterpret_cast<void **>(&bootstrap)
    );
    // </SnippetLoadAndGet>
    assert(rc == 0 && bootstrap != nullptr && "Failure: load_assembly_and_get_function_pointer()");

    close_func(context);

    //
    // STEP 4: Run managed code
    //
    struct BootstrapArguments {
      NativeBindings native_function_pointers;
      
      void (*function_pointers_callback)(ManagedBindings *);
    };
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

    if (bootstrap(&args, sizeof(args))) {
      HYP_PANIC_MESSAGE("Dotnet", "Failed to bootstrap core assembly!");
    }
      
    g_type_world = g_managed_bindings.get_type_by_name("Hyperion.World");
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