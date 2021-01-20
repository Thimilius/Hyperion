#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class ShaderStageFlags {
        Unknown,

        Vertex   = BIT(0),
        Fragment = BIT(1)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ShaderStageFlags);

}