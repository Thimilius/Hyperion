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
                BufferElement("a_position", BufferDataType::Float3),
                BufferElement("a_normal", BufferDataType::Float3),
                BufferElement("a_uv", BufferDataType::Float2)
            });
        }
    };

    struct VertexImmediate {
        Vec3 position;
        Color color;

        static BufferLayout GetBufferLayout() {
            return BufferLayout({
                BufferElement("a_position", BufferDataType::Float3),
                BufferElement("a_color", BufferDataType::Float4),
            });
        }
    };

    struct VertexFont {
        Vec2 position;
        Vec2 uv;

        static BufferLayout GetBufferLayout() {
            return BufferLayout({
                BufferElement("a_position", BufferDataType::Float2),
                BufferElement("a_uv", BufferDataType::Float2),
            });
        }
    };

}