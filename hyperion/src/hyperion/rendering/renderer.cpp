#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

namespace Hyperion::Rendering {

    void CRenderer::Begin() {

    }

    void CRenderer::End() {

    }

    void CRenderer::Submit(const TRef<CShader> &shader, const TRef<CVertexArray> &vertex_array) {
        shader->Bind();
        vertex_array->Bind();

        CRenderCommand::DrawIndexed(vertex_array);
    }

}