#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/core/math/math.hpp"

namespace Hyperion::Rendering {

    struct SVertexPNU {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;

        static BufferLayout GetBufferLayout() {
            return BufferLayout({
                BufferElement("a_position", ShaderDataType::Float3),
                BufferElement("a_normal", ShaderDataType::Float3),
                BufferElement("a_uv", ShaderDataType::Float2)
            });
        }
    };

}