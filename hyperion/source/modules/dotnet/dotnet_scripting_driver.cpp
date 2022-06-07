//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/dotnet/dotnet_scripting_driver.hpp"

//---------------------- Library Includes ----------------------
#include <dotnet/hostfxr.h>
#include <dotnet/coreclr_delegates.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/operating_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

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

    // We bootstrap the runtime by providing a callback to get bindings to the managed runtime methods.
    RuntimeBoostrapArguments runtime_arguments = { };
    runtime_arguments.native_bindings.exception = [](ManagedString message) {
      HYP_PANIC_MESSAGE("Scripting", "{}", message);
    };
    runtime_arguments.managed_bindings_callback = [](RuntimeManagedBindings *runtime_managed_bindings) {
      HYP_LOG_INFO("Scripting", "Setting runtime bindings...");
      s_runtime_managed_bindings = *runtime_managed_bindings;
    };
    runtime_bootstrap_func(&runtime_arguments);

    // The bindings have to be setup just once and get exchanged when every time the context gets loaded.
    DotnetScriptingBindings::Initialize();

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
    if (new_state == EngineState::EditorRuntimePaused) {
      return;
    }
    
    if (new_state == EngineState::EditorRuntimePlaying && old_state != EngineState::EditorRuntimePaused) {
      LoadManagedContext();
    } else if (new_state == EngineState::Editor) {
      UnloadManagedContext();
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::Update() {
    if (Engine::GetEngineState() == EngineState::Runtime || Engine::GetEngineState() == EngineState::EditorRuntimePlaying) {
      HYP_PROFILE_SCOPE("DotnetScriptingDriver.EngineUpdate")
      DotnetScriptingBindings::GetManagedBindings()->engine_update();
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
    HYP_PROFILE_SCOPE("DotnetScriptingDriver.LoadManagedContext")
    s_runtime_managed_bindings.load_context(DotnetScriptingBindings::GetBootstrapArguments());

    {
      HYP_PROFILE_SCOPE("DotnetScriptingDriver.EngineInitialize")
      DotnetScriptingBindings::GetManagedBindings()->engine_initialize();
    }
    
    HYP_LOG_INFO("Scripting", "Loaded managed context!");
  }
  
  //--------------------------------------------------------------
  void DotnetScriptingDriver::UnloadManagedContext() {
    HYP_PROFILE_SCOPE("DotnetScriptingDriver.UnloadManagedContext")

    {
      HYP_PROFILE_SCOPE("DotnetScriptingDriver.EngineShutdown")
      DotnetScriptingBindings::GetManagedBindings()->engine_shutdown();
    }

    DotnetScriptingBindings::UnloadMappings();
    
    s_runtime_managed_bindings.unload_context();
    HYP_LOG_INFO("Scripting", "Unloaded managed context!");
  }

}
