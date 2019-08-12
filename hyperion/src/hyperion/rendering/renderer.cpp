#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

namespace Hyperion::Rendering {

    void CRenderer::Begin() {

    }

    void CRenderer::End() {

    }

    void CRenderer::Submit(const PRef<CShader> &shader, const PRef<CVertexArray> &vertex_array) {
        shader->Bind();
        vertex_array->Bind();

        CRenderCommand::DrawIndexed(vertex_array);
    }

}