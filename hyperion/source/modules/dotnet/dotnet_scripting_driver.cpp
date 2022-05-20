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

  int Function(int arg, const char *string) {
    HYP_LOG_INFO("LUL", "Das isses: {} - {}", arg, string);
    return 7;
  }
  
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
    result = get_delegate_func(
            context,
            hdt_load_assembly_and_get_function_pointer,
            reinterpret_cast<void **>(&load_assembly_and_get_function_pointer));
    assert(result == 0);

    auto library_path = StringUtils::Utf8ToUtf16(settings.library_path + settings.core_library_name + ".dll");
    auto dotnet_type = L"Hyperion.Bootstrapper, Hyperion";
    auto dotnet_type_method = L"Bootstrap";
    // <SnippetLoadAndGet>
    // Function pointer to managed delegate
    component_entry_point_fn hello = nullptr;
    int rc = load_assembly_and_get_function_pointer(
      library_path.c_str(),
      dotnet_type,
      dotnet_type_method,
      nullptr /*delegate_type_name*/,
      nullptr,
      reinterpret_cast<void **>(&hello)
    );
    // </SnippetLoadAndGet>
    assert(rc == 0 && hello != nullptr && "Failure: load_assembly_and_get_function_pointer()");

    close_func(context);
    
    //
    // STEP 4: Run managed code
    //
    struct lib_args
    {
      const char_t *message;
      int number;
      int (*function)(int, const char *);  
    };
    for (int i = 0; i < 3; ++i)
    {
      // <SnippetCallManaged>
      lib_args args
      {
        L"from host!",
        i,
        Function,
      };

      hello(&args, sizeof(args));
      // </SnippetCallManaged>
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingDriver::Shutdown() {
    
  }

}
