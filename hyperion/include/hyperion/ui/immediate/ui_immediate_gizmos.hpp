﻿//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/render/render_gizmos.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  enum class GizmoMode {
    TranslateX,
    TranslateY,
    TranslateZ,
  };

  struct GizmoManipulation {
    bool8 in_transformation = false;
    Rendering::RenderGizmoAxisHighlight highlight_axis = Rendering::RenderGizmoAxisHighlight::None;
  };
  
  class UIImmediateGizmos final {
  public:
    static GizmoManipulation Manipulate(
      DerivedTransformComponent *derived_transform,
      LocalTransformComponent *local_transform,
      DerivedTransformComponent *camera_transform,
      Ray ray
    );
  private:
    UIImmediateGizmos() = delete;
    ~UIImmediateGizmos() = delete;
  };
  
}