//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {
  
  using ManagedHandle = void *;
  using NativeHandle = void *;
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

    ManagedHandle (*get_entity_manager)(NativeHandle);
  };

  struct EntityManagerBindings {
    uint32 (*get_entity_count)(NativeHandle);
  };
  
  struct CoreNativeBindings {
    CoreBindings core;
    LogBindings log;
    WorldManagerBindings world_manager;
    WorldBindings world;
    EntityManagerBindings entity_manager;
  };
  
  struct CoreManagedBindings {
    void (*engine_initialize)();
    void (*engine_update)();
    void (*engine_shutdown)();

    ManagedHandle (*get_type_by_name)(ManagedString);
    void (*destroy_type)(ManagedHandle);
    
    ManagedHandle (*create_managed_object)(ManagedHandle, NativeHandle);
    void (*destroy_managed_object)(ManagedHandle);
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
    static void UnloadMappings();
  private:
    DotnetScriptingBindings() = delete;
    ~DotnetScriptingBindings() = delete;
  private:
    static ManagedHandle GetOrCreateManagedObject(ManagedHandle type, NativeHandle native_handle);
  private:
    inline static CoreBootstrapArguments s_core_bootstrap_arguments;
    
    inline static CoreManagedBindings s_core_managed_bindings;

    // This assumes that the pointers to objects on the native side are stable.
    inline static Map<NativeHandle, ManagedHandle> s_object_mappings;
    
    inline static ManagedHandle s_type_world;
    inline static ManagedHandle s_type_entity_manager;
  };
    
}