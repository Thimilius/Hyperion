//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/component_registry.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void ComponentRegistry::Initialize() {
    Array<MetaType> component_types;
    MetaRegistry::Resolve([&component_types](MetaType type) {
      if (!type.IsDefaultConstructable()) {
        return;
      }

      if (type.IsDerivedFrom<IComponent>()) {
        component_types.Add(type);
      }
    });

    // We collected all component types beforehand so that we can loop over them in reverse order.
    // We do this because the first reflected component is going to be at the end and we want to register it as the first.
    for (auto it = component_types.rbegin(); it != component_types.rend(); ++it) {
      MetaType component_type = *it;

      ComponentInfo component_info;
      component_info.id = s_component_counter;
      component_info.type = component_type;
      component_info.element_size = component_type.GetSize();
      component_info.destructor = component_type.GetTrivialDestructor();
      component_info.constructor = component_type.GetInPlaceConstructor();

      s_component_infos.Add(component_info);
      s_component_ids.Insert(component_type, s_component_counter);

      HYP_LOG_INFO("Entity", "Registered component: {} - {} with element size: {}", s_component_counter, component_type.GetName(), component_type.GetSize());

      s_component_counter++;
    }
  }

}