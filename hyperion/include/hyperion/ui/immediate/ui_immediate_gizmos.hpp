//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/transform_components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  enum class GizmoMode {
    TranslateX,
    TranslateY,
    TranslateZ,
  };
  
  class UIImmediateGizmos final {
  public:
    static bool8 Manipulate(
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
