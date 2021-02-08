#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class ShaderType {
        None,

        Vertex,
        Fragment,
    };

    enum class ShaderLightMode {
        Unknown,

        None,
        Forward,
    };

    struct ShaderAttributes {
        ShaderLightMode light_mode = ShaderLightMode::None;
    };

}