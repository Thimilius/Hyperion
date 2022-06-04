//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {
  
  using ManagedHandle = void *;
  using NativeHandle = void *;
  using TypeHandle = void *;
  using ManagedString = const char *;

  struct CoreBindings {
    void (*exception)(ManagedString);
  };
  
  struct LogBindings {
    void (*log_trace)(ManagedString);
    void (*log_info)(ManagedString);
    void (*log_warn)(ManagedString);
    void (*log_error)(ManagedString);
  };

  struct WorldManagerBindings {
    ManagedHandle (*get_active_world)();
  };

  struct WorldBindings {
    ManagedString (*get_name)(NativeHandle);
    void (*set_name)(NativeHandle, ManagedString);
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

    TypeHandle (*get_type_by_name)(ManagedString);
    ManagedHandle (*create_managed_object)(TypeHandle, NativeHandle);
  };

  struct CoreBootstrapArguments {
    CoreNativeBindings native_bindings;
    void (*managed_bindings_callback)(CoreManagedBindings *);
  };
  
  class DotnetScriptingBindings final {
  public:
    inline static CoreBootstrapArguments *GetBootstrapArguments() { return &s_core_bootstrap_arguments; }
    inline static CoreManagedBindings *GetManagedBindings() { return &s_core_managed_bindings; }

    static void Initialize();
  private:
    DotnetScriptingBindings() = delete;
    ~DotnetScriptingBindings() = delete;
  private:
    inline static CoreBootstrapArguments s_core_bootstrap_arguments;
    
    inline static CoreManagedBindings s_core_managed_bindings;

    // This assumes that the pointers to objects on the native side stable.
    inline static Map<NativeHandle, ManagedHandle> s_object_mappings;
    
    inline static TypeHandle s_type_world;
  };
    
}