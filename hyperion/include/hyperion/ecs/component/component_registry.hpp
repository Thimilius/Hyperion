//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;
  class WorldManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class ComponentRegistry final {
  public:
    inline static const Array<ComponentInfo> &GetComponentInfos() { return s_component_infos; }

    template<typename T>
    inline static ComponentId GetId() { return GetId(Type::get<T>()); }

    inline static ComponentId GetId(Type type) {
      // NOTE: This method/lookup is probably not as fast as it could be.
      // And it really should because it is pretty much THE hottest path in the engine.
      return s_component_ids.Get(type);
    }

    template<typename T>
    inline static void RegisterComponent() {
      static Type component_type = Type::get<T>();

      if (s_component_ids.Contains(component_type)) {
        HYP_LOG_ERROR("Entity", "Component {} is already registered!", component_type.get_name().to_string());
        return;
      }

      ComponentInfo component_info = {};
      component_info.id = s_component_counter;
      component_info.type = &component_type;
      component_info.element_size = component_type.get_sizeof();
      component_info.constructor = [](void *address) { return static_cast<void *>(new(address) T()); };
      component_info.destructor = [](const void *instance) { static_cast<const T *>(instance)->~T(); };

      s_component_infos.Add(component_info);
      s_component_ids.Insert(component_type, s_component_counter);
      
      s_component_counter++;
    }
  private:
    static void RegisterTypes();
    static void RegisterCoreComponents();
  private:
    inline static Map<Type, ComponentId> s_component_ids;
    inline static Array<ComponentInfo> s_component_infos;

    inline static ComponentId s_component_counter = 0;
  private:
    friend class Hyperion::Engine;
    friend class Hyperion::WorldManager;
  };

}
