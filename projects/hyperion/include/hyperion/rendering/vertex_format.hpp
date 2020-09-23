#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    struct VertexMesh {
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

    struct VertexUI {
        Vec3 position;
        Color color;
        Vec2 uv;

        static BufferLayout GetBufferLayout() {
            return BufferLayout({
                BufferElement("a_position", BufferDataType::Float3),
                BufferElement("a_color", BufferDataType::Float4),
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