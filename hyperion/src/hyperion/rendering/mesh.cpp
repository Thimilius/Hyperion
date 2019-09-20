#include "hyppch.hpp"

#include "hyperion/rendering/mesh.hpp"

#include "hyperion/platform/opengl/opengl_mesh.hpp"

using namespace Hyperion::Math;

namespace Hyperion::Rendering {

    TRef<CMesh> CMesh::Create(const TRef<CVertexArray> &vertex_array) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLMesh>(vertex_array);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CMesh> CMesh::CreatePlane(float width, float height) {
        float half_width = width / 2.0f;
        float half_height = height / 2.0f;

        // The plane should face up
        SVec3 normal = SVec3::Up();

        SVertexPNUV verticies[4];

        verticies[0].position = SVec3(half_width, 0, half_height);
        verticies[0].normal = normal;
        verticies[0].uv = SVec2(1.0f, 1.0f);

        verticies[1].position = SVec3(half_width, 0, -half_height);
        verticies[1].normal = normal;
        verticies[1].uv = SVec2(1.0f, 0.0f);

        verticies[2].position = SVec3(-half_width, 0, -half_height);
        verticies[2].normal = normal;
        verticies[2].uv = SVec2(0.0f, 0.0f);

        verticies[3].position = SVec3(-half_width, 0, half_height);
        verticies[3].normal = normal;
        verticies[3].uv = SVec2(0.0f, 1.0f);

        TRef<CVertexBuffer> vertex_buffer = CVertexBuffer::Create((u8*)verticies, sizeof(verticies));
        CBufferLayout buffer_layout = CBufferLayout({
           SBufferElement("a_position", EShaderDataType::Float3),
           SBufferElement("a_normal", EShaderDataType::Float3),
           SBufferElement("a_uv", EShaderDataType::Float2),
        });
        vertex_buffer->SetLayout(buffer_layout);

        u32 indicies[6] = {
            0, 2, 1,
            0, 3, 2
        };
        TRef<CIndexBuffer> index_buffer = CIndexBuffer::Create(indicies, sizeof(indicies));

        TRef<CVertexArray> vertex_array = CVertexArray::Create();
        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        return Create(vertex_array);
    }

    TRef<CMesh> CMesh::CreatePlane(Math::SVec2 size) {
        return CreatePlane(size.x, size.y);
    }

}