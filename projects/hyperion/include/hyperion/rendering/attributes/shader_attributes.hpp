#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class ShaderStageFlags {
        None,

        Vertex   = BIT(0),
        Fragment = BIT(1),
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ShaderStageFlags);

    enum class ShaderLightMode {
        Unknown,

        None,
        Forward,
    };

    struct ShaderAttributes {
        ShaderLightMode light_mode = ShaderLightMode::None;
    };

}