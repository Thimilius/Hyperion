#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Rendering {

    class CRenderer {
    public:
        static void Begin();
        static void End();

        static void Submit(const TRef<CShader> &shader, const TRef<CVertexArray> &vertex_array);
    };

}