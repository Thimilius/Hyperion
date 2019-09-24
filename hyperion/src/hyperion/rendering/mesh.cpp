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

        u16 indicies[6] = {
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

    TRef<CMesh> CMesh::CreateCube(float size) {
        SVertexPNUV verticies[24];

        float half_size = size / 2.0f;

        // Forward and back as seen from the center of the cube!
        {
            // Forward
            verticies[0].position = SVec3(half_size, -half_size, -half_size);
            verticies[0].normal = SVec3::Forward();
            verticies[0].uv = SVec2(0, 0);

            verticies[1].position = SVec3(half_size, half_size, -half_size);
            verticies[1].normal = SVec3::Forward();
            verticies[1].uv = SVec2(0, 1);

            verticies[2].position = SVec3(-half_size, half_size, -half_size);
            verticies[2].normal = SVec3::Forward();
            verticies[2].uv = SVec2(1, 1);

            verticies[3].position = SVec3(-half_size, -half_size, -half_size);
            verticies[3].normal = SVec3::Forward();
            verticies[3].uv = SVec2(1, 0);

            // Right
            verticies[4].position = SVec3(half_size, -half_size, half_size);
            verticies[4].normal = SVec3::Right();
            verticies[4].uv = SVec2(0, 0);

            verticies[5].position = SVec3(half_size, half_size, half_size);
            verticies[5].normal = SVec3::Right();
            verticies[5].uv = SVec2(0, 1);

            verticies[6].position = SVec3(half_size, half_size, -half_size);
            verticies[6].normal = SVec3::Right();
            verticies[6].uv = SVec2(1, 1);

            verticies[7].position = SVec3(half_size, -half_size, -half_size);
            verticies[7].normal = SVec3::Right();
            verticies[7].uv = SVec2(1, 0);

            // Back
            verticies[8].position = SVec3(-half_size, -half_size, half_size);
            verticies[8].normal = SVec3::Back();
            verticies[8].uv = SVec2(0, 0);

            verticies[9].position = SVec3(-half_size, half_size, half_size);
            verticies[9].normal = SVec3::Back();
            verticies[9].uv = SVec2(0, 1);

            verticies[10].position = SVec3(half_size, half_size, half_size);
            verticies[10].normal = SVec3::Back();
            verticies[10].uv = SVec2(1, 1);

            verticies[11].position = SVec3(half_size, -half_size, half_size);
            verticies[11].normal = SVec3::Back();
            verticies[11].uv = SVec2(1, 0);

            // Left
            verticies[12].position = SVec3(-half_size, -half_size, -half_size);
            verticies[12].normal = SVec3::Left();
            verticies[12].uv = SVec2(0, 0);

            verticies[13].position = SVec3(-half_size, half_size, -half_size);
            verticies[13].normal = SVec3::Left();
            verticies[13].uv = SVec2(0, 1);

            verticies[14].position = SVec3(-half_size, half_size, half_size);
            verticies[14].normal = SVec3::Left();
            verticies[14].uv = SVec2(1, 1);

            verticies[15].position = SVec3(-half_size, -half_size, half_size);
            verticies[15].normal = SVec3::Left();
            verticies[15].uv = SVec2(1, 0);

            // Up
            verticies[16].position = SVec3(-half_size, half_size, half_size);
            verticies[16].normal = SVec3::Up();
            verticies[16].uv = SVec2(0, 0);

            verticies[17].position = SVec3(-half_size, half_size, -half_size);
            verticies[17].normal = SVec3::Up();
            verticies[17].uv = SVec2(0, 1);

            verticies[18].position = SVec3(half_size, half_size, -half_size);
            verticies[18].normal = SVec3::Up();
            verticies[18].uv = SVec2(1, 1);

            verticies[19].position = SVec3(half_size, half_size, half_size);
            verticies[19].normal = SVec3::Up();
            verticies[19].uv = SVec2(1, 0);

            // Down
            verticies[20].position = SVec3(-half_size, -half_size, -half_size);
            verticies[20].normal = SVec3::Down();
            verticies[20].uv = SVec2(0, 0);

            verticies[21].position = SVec3(-half_size, -half_size, half_size);
            verticies[21].normal = SVec3::Down();
            verticies[21].uv = SVec2(0, 1);

            verticies[22].position = SVec3(half_size, -half_size, half_size);
            verticies[22].normal = SVec3::Down();
            verticies[22].uv = SVec2(1, 1);

            verticies[23].position = SVec3(half_size, -half_size, -half_size);
            verticies[23].normal = SVec3::Down();
            verticies[23].uv = SVec2(1, 0);
        }

        TRef<CVertexBuffer> vertex_buffer = CVertexBuffer::Create((u8 *)verticies, sizeof(verticies));
        CBufferLayout buffer_layout = CBufferLayout({
           SBufferElement("a_position", EShaderDataType::Float3),
           SBufferElement("a_normal", EShaderDataType::Float3),
           SBufferElement("a_uv", EShaderDataType::Float2),
            });
        vertex_buffer->SetLayout(buffer_layout);

        u16 indicies[] = {
            // Back
            0, 1, 2,
            0, 2, 3,

            // Right
            4, 5, 6,
            4, 6, 7,

            // Forward
            8, 9, 10,
            8, 10, 11,

            // Left
            12, 13, 14,
            12, 14, 15,

            // Up
            16, 17, 18,
            16, 18, 19,

            // Down
            20, 21, 22,
            20, 22, 23
        };
        TRef<CIndexBuffer> index_buffer = CIndexBuffer::Create(indicies, sizeof(indicies));

        TRef<CVertexArray> vertex_array = CVertexArray::Create();
        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        return Create(vertex_array);
    }

}