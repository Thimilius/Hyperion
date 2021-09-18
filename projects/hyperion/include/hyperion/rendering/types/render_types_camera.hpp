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
        uint32 x;
        uint32 y;
        uint32 width;
        uint32 height;
    };

    struct CameraViewportClipping {
        float32 x;
        float32 y;
        float32 width;
        float32 height;
    };

}