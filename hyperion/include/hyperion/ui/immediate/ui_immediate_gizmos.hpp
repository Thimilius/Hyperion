//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/render/render_gizmos.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  enum class GizmoMode {
    Local,
    World
  };

  struct GizmoManipulation {
    bool8 in_transformation = false;
    Rendering::RenderGizmoAxisHighlight highlight_axis = Rendering::RenderGizmoAxisHighlight::None;
  };
  
  class UIImmediateGizmos final {
  public:
    static GizmoManipulation Manipulate(
      Rendering::RenderGizmoType type,
      GizmoMode mode,
      EntityManager *entity_manager,
      EntityId entity,
      DerivedTransformComponent *derived_transform,
      LocalTransformComponent *local_transform,
      Ray ray
    );
  private:
    UIImmediateGizmos() = delete;
    ~UIImmediateGizmos() = delete;
  private:
    inline static Vector3 s_offset = Vector3();
    inline static bool8 s_should_transform_x = false;
    inline static bool8 s_should_transform_y = false;
    inline static bool8 s_should_transform_z = false;
  };
  
}
