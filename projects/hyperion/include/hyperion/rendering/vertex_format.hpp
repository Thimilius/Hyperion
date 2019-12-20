#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/math/math.hpp"

namespace Hyperion::Rendering {

    struct SVertexPNU {
        Math::SVec3 position;
        Math::SVec3 normal;
        Math::SVec2 uv;

        static CBufferLayout GetBufferLayout() {
            return CBufferLayout({
                SBufferElement("a_position", EShaderDataType::Float3),
                SBufferElement("a_normal", EShaderDataType::Float3),
                SBufferElement("a_uv", EShaderDataType::Float2)
            });
        }
    };

}