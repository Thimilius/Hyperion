//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class CameraClearMode {
    Nothing,
    Depth,
    Color,
    Skybox
  };

  enum class CameraProjectionMode {
    Perspective,
    Orthographic
  };

  struct CameraViewport {
    int32 x;
    int32 y;
    int32 width;
    int32 height;
  };

  struct CameraViewportClipping {
    float32 x;
    float32 y;
    float32 width;
    float32 height;
  };

}
