//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector2.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  template<typename T>
  struct TextureAtlasElement {
    Vector2 uv_top_left;
    Vector2 uv_top_right;
    Vector2 uv_bottom_right;
    Vector2 uv_bottom_left;

    T payload;
  };

}
