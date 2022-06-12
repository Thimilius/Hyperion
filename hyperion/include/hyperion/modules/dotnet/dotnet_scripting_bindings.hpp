﻿//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"
#include "hyperion/ecs/component/component_registry.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {
  
  using ManagedHandle = void *;
  using NativeHandle = void *;
  using ManagedString = const char *;

  enum class SpecialType {
    World,
    EntityManager,

    Last
  };
  
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

    void (*create_entity)(NativeHandle, EntityId *);
    void (*destroy_entity)(NativeHandle, EntityId);
    
    bool (*has_component)(NativeHandle, ManagedHandle, EntityId);
    void (*remove_component)(NativeHandle, ManagedHandle, EntityId);
  };
  
  struct NameComponentBindings {
    ManagedString (*get_name)(NativeHandle, EntityId);
    void (*set_name)(NativeHandle, EntityId, ManagedString);
  };
  
  struct CoreNativeBindings {
    CoreBindings core;
    LogBindings log;
    WorldManagerBindings world_manager;
    WorldBindings world;
    EntityManagerBindings entity_manager;
    NameComponentBindings name_component;
  };
  
  struct CoreManagedBindings {
    void (*engine_initialize)();
    void (*engine_update)();
    void (*engine_shutdown)();

    ManagedHandle (*get_type_by_name)(ManagedString);
    void (*destroy_type)(ManagedHandle);
    
    ManagedHandle (*create_managed_object)(ManagedHandle, NativeHandle);
    void (*destroy_managed_object)(ManagedHandle);

    void (*on_entity_destroyed)(ManagedHandle, EntityId);
    void (*on_component_removed)(ManagedHandle, ManagedHandle, EntityId);
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

    static ManagedHandle GetOrCreateManagedObject(ManagedHandle type, NativeHandle native_handle);
    static void DestroyManagedObject(NativeHandle native_handle, ManagedHandle managed_handle);
    
    static ManagedHandle GetSpecialType(SpecialType special_type) { return s_special_types[static_cast<uint32>(special_type)]; }
    static ComponentId GetComponentIdForManagedType(ManagedHandle component_type_handle);
    static ManagedHandle GetManagedTypeForComponentId(ComponentId component_id);
  private:
    DotnetScriptingBindings() = delete;
    ~DotnetScriptingBindings() = delete;
  private:
    static void RegisterSpecialType(SpecialType special_type, const char *full_name);

    template<typename T>
    static void RegisterComponentType(const char *full_name) {
      ManagedHandle component_type_handle = s_core_managed_bindings.get_type_by_name(full_name);
      ComponentId component_id = ComponentRegistry::GetId<T>();
      s_managed_component_types_to_id.Insert(component_type_handle, component_id);
      s_id_to_managed_component_types.Insert(component_id, component_type_handle);
    }
  private:
    inline static CoreBootstrapArguments s_core_bootstrap_arguments;
    
    inline static CoreManagedBindings s_core_managed_bindings;

    // This assumes that the pointers to objects on the native side are stable.
    inline static Map<NativeHandle, ManagedHandle> s_object_mappings;

    inline static ManagedHandle s_special_types[static_cast<uint32>(SpecialType::Last)];
    inline static Map<ManagedHandle, ComponentId> s_managed_component_types_to_id;
    inline static Map<ComponentId, ManagedHandle> s_id_to_managed_component_types;
  };
    
}