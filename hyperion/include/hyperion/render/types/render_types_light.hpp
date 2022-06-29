//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class LightType {
    Directional,
    Point,
    Spot
  };

  enum class LightShadows {
    None,
    SoftShadows
  };

}
