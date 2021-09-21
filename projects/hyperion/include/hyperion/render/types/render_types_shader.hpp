#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class ShaderStageFlags {
        Unknown,

        Vertex = BIT(0),
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

    struct ShaderData {
        ShaderAttributes attributes;
        ShaderStageFlags stage_flags;

        String vertex_source;
        String fragment_source;
    };

}