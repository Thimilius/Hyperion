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

    inline static ComponentId GetId(ComponentGuid guid) {
      return s_guid_to_id.Get(guid);
    }

    inline static ComponentInfo GetInfo(ComponentId id) {
      return s_component_infos[id];
    }
    
    inline static ComponentInfo GetInfo(ComponentGuid guid) {
      return GetInfo(GetId(guid));
    }
    
    template<typename T>
    inline static void RegisterComponent(const ComponentGuid &guid) {
      static Type component_type = Type::get<T>();
      
      if (s_component_ids.Contains(component_type)) {
        HYP_LOG_ERROR("Entity", "Component {} is already registered!", component_type.get_name().to_string());
        return;
      }

      ComponentId id = s_component_counter;
      
      ComponentInfo component_info = {};
      component_info.id = id;
      component_info.guid = guid;
      component_info.type = &component_type;
      component_info.element_size = component_type.get_sizeof();
      component_info.constructor = [](void *address) { return static_cast<void *>(new(address) T()); };
      component_info.copy_constructor = [](void *address, const void *instance) {
        return static_cast<void *>(new(address) T(*static_cast<const T *>(instance)));
      };
      component_info.copy_assignment_operator = [](void *destination_instance, void *source_instance) {
        *static_cast<T *>(destination_instance) = *static_cast<T *>(source_instance);
      };
      component_info.move_assignment_operator = [](void *destination_instance, void *source_instance) {
        *static_cast<T *>(destination_instance) = std::move(*static_cast<T *>(source_instance));
      };
      component_info.destructor = [](const void *instance) { static_cast<const T *>(instance)->~T(); };

      s_component_infos.Add(component_info);
      s_component_ids.Insert(component_type, id);
      s_guid_to_id.Insert(guid, id);
      
      s_component_counter++;
    }
  private:
    static void RegisterTypes();
    static void RegisterCoreComponents();
  private:
    inline static Map<Type, ComponentId> s_component_ids;
    inline static Array<ComponentInfo> s_component_infos;
    inline static Map<ComponentGuid, ComponentId> s_guid_to_id;

    inline static ComponentId s_component_counter = 0;
  private:
    friend class Hyperion::Engine;
    friend class Hyperion::WorldManager;
  };

}
