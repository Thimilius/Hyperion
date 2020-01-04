#include "hyppch.hpp"

#include "hyperion/rendering/immediate_renderer.hpp"

#include "hyperion/assets/asset_library.hpp"

namespace Hyperion::Rendering {

    void ImmediateRenderer::Begin(const Ref<Camera> &camera) {
        s_state.transform.view = camera->GetViewMatrix();
        s_state.transform.projection = camera->GetProjectionMatrix();
        s_state.transform.view_projection = camera->GetViewProjectionMatrix();
    }

    void ImmediateRenderer::Draw(PrimitiveType primitive_type, const Ref<VertexArray> &vertex_array, u32 vertex_count) {
        s_immediate.shader->Bind();
        s_immediate.shader->SetMat4("u_transform.view", s_state.transform.view);
        s_immediate.shader->SetMat4("u_transform.projection", s_state.transform.projection);

        vertex_array->Bind();

        RenderEngine::Draw(primitive_type, vertex_count, 0);
    }

    void ImmediateRenderer::DrawCube(Vec3 center, Vec3 size, Color color) {
        Vec3 half_size = size * 0.5f;

        // Forward
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);

        // Right
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);

        // Back
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);

        // Left
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);

        // Up
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);

        // Down
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);

        Flush(PrimitiveType::Triangles);
    }

    void ImmediateRenderer::DrawLine(Vec3 a, Vec3 b, Color color) {
        AddVertex(a, color);
        AddVertex(b, color);

        Flush(PrimitiveType::Lines);
    }

    void ImmediateRenderer::Init() {
        s_immediate.shader = AssetLibrary::GetShader("immediate");
        s_immediate.vertex_buffer = VertexBuffer::Create(nullptr, sizeof(s_immediate.data_buffer), BufferUsage::DynamicDraw);
        s_immediate.vertex_buffer->SetLayout(VertexImmediate::GetBufferLayout());
        s_immediate.vertex_array = VertexArray::Create();
        s_immediate.vertex_array->AddVertexBuffer(s_immediate.vertex_buffer);
    }

    void ImmediateRenderer::AddVertex(Vec3 position, Color color) {
        u32 vertex_offset = s_state.vertex_offset;
        HYP_ASSERT_MESSAGE(vertex_offset < 2000, "Immediate vertex buffer is full!");

        s_immediate.data_buffer[vertex_offset].position = position;
        s_immediate.data_buffer[vertex_offset].color = color;

        s_state.vertex_offset++;
    }

    void ImmediateRenderer::End() {
        
    }

    void ImmediateRenderer::Flush(PrimitiveType type) {
        // We do not need to draw anything if no verticies were added
        if (s_state.vertex_offset == 0) {
            return;
        }

        s_immediate.shader->Bind();
        s_immediate.shader->SetMat4("u_transform.view", s_state.transform.view);
        s_immediate.shader->SetMat4("u_transform.projection", s_state.transform.projection);

        s_immediate.vertex_buffer->SetData(0, s_state.vertex_offset * sizeof(VertexImmediate), (u8*)s_immediate.data_buffer);
        s_immediate.vertex_array->Bind();

        RenderEngine::Draw(type, s_state.vertex_offset, 0);

        s_state.vertex_offset = 0;
    }

}