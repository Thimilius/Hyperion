#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/core/color.hpp"

namespace Hyperion::Rendering {

    struct VertexPNU {
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

    struct VertexImmediate {
        Vec3 position;
        Color color;

        static BufferLayout GetBufferLayout() {
            return BufferLayout({
                BufferElement("a_position", ShaderDataType::Float3),
                BufferElement("a_color", ShaderDataType::Float4),
            });
        }
    };

}