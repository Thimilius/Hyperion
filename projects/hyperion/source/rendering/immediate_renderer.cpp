//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/immediate_renderer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/rendering/render_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {
    
    //--------------------------------------------------------------
    void ImmediateRenderer::DrawText(Font *font, const String &text, Vec2 position, float32 scale, Color color) {
        Object::Destroy(s_font_mesh);
        s_font_mesh_data.positions.clear();
        s_font_mesh_data.texture0.clear();
        s_font_mesh_data.indices.clear();

        Vector<uint32> codepoints = StringUtils::GetCodepointsUtf8(text);
        uint32 index = 0;
        uint32 index_count = 0;
        for (uint32 codepoint : codepoints) {
            FontAtlasElement element = font->GetElement(codepoint);
            FontGlyph glyph = element.payload;

            float32 x_pos = position.x + glyph.bearing.x * scale;
            float32 y_pos = position.y - (glyph.size.y - glyph.bearing.y) * scale;
            float32 width = glyph.size.x * scale;
            float32 height = glyph.size.y * scale;

            s_font_mesh_data.positions.push_back(Vec3(x_pos, y_pos + height, 0.0f));
            s_font_mesh_data.texture0.push_back(element.uv_top_left);
            s_font_mesh_data.positions.push_back(Vec3(x_pos + width, y_pos + height, 0.0f));
            s_font_mesh_data.texture0.push_back(element.uv_top_right);
            s_font_mesh_data.positions.push_back(Vec3(x_pos + width, y_pos, 0.0f));
            s_font_mesh_data.texture0.push_back(element.uv_bottom_right);
            s_font_mesh_data.positions.push_back(Vec3(x_pos, y_pos, 0.0f));
            s_font_mesh_data.texture0.push_back(element.uv_bottom_left);

            s_font_mesh_data.indices.push_back(index++);
            s_font_mesh_data.indices.push_back(index++);
            s_font_mesh_data.indices.push_back(index++);
            s_font_mesh_data.indices.push_back(index - 3);
            s_font_mesh_data.indices.push_back(index - 1);
            s_font_mesh_data.indices.push_back(index++);

            index_count += 6;

            position.x += glyph.advance * scale;
        }

        Vector<SubMesh> sub_meshes = { { MeshTopology::Triangles, index_count, 0, 0 } };
        s_font_mesh = Mesh::Create(s_font_mesh_data, sub_meshes);

        SetFontCameraData();
        s_font_material->SetVec4("u_color", color);
        s_font_material->SetTexture("u_texture", font->GetTexture());
        s_render_driver->DrawMesh(s_font_mesh->GetResourceId(), Mat4::Identity(), s_font_material->GetResourceId(), 0);
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::Begin(MeshTopology topology) {
        s_current_topology = topology;
        s_current_index = 0;

        s_immediate_mesh_data.positions.clear();
        s_immediate_mesh_data.colors.clear();
        s_immediate_mesh_data.indices.clear(),

        Object::Destroy(s_immediate_mesh);
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::DrawLine(Vec3 start, Vec3 end, Color color) {
        AddVertex(start, color);
        AddVertex(end, color);
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::End() {
        if (s_current_index > 0) {
            Vector<SubMesh> sub_meshes = { { s_current_topology, s_current_index, 0, 0 } };
            s_immediate_mesh = Mesh::Create(s_immediate_mesh_data, sub_meshes);

            s_render_driver->DrawMesh(s_immediate_mesh->GetResourceId(), Mat4::Identity(), s_immediate_material->GetResourceId(), 0);
        }
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::Initialize(IRenderDriver *render_driver) {
        s_render_driver = render_driver;

        String &immediate_source = FileSystem::ReadAllText("data/shaders/immediate.shader");
        s_immediate_material = Material::Create(Shader::Create(immediate_source));

        String &font_source = FileSystem::ReadAllText("data/shaders/font.shader");
        s_font_material = Material::Create(Shader::Create(font_source));
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::AddVertex(Vec3 position, Color color) {
        s_immediate_mesh_data.positions.push_back(position);
        s_immediate_mesh_data.colors.push_back(color);
        s_immediate_mesh_data.indices.push_back(s_current_index++);
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::SetFontCameraData() {
        float32 half_width = static_cast<float32>(Display::GetWidth()) / 2.0f;
        float32 half_height = static_cast<float32>(Display::GetHeight()) / 2.0f;

        CameraData camera_data = { };
        camera_data.projection_mode = CameraProjectionMode::Orthographic;
        
        camera_data.view_matrix = Mat4::Identity();
        camera_data.inverse_view_matrix = camera_data.view_matrix.Inverted();
        
        // We want (0, 0) to be at the center of the screen.
        camera_data.projection_matrix = Mat4::Orthographic(-half_width, half_width, -half_height, half_height, -1, 1);
        camera_data.inverse_projection_matrix = camera_data.projection_matrix.Inverted();

        camera_data.view_projection_matrix = camera_data.projection_matrix;
        camera_data.inverse_view_projection_matrix = camera_data.view_projection_matrix.Inverted();

        s_render_driver->SetCameraData(camera_data);
    }

}