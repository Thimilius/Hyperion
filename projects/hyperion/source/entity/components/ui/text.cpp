//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/text.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Text::SetText(const String &text) {
        if (m_text == text) {
            return;
        }

        m_text = text;
        SetDirty();
    }

    //--------------------------------------------------------------
    void Text::SetFont(Font *font) {
        if (m_font == font) {
            return;
        }

        m_font = font;
        SetDirty();
    }

    //--------------------------------------------------------------
    void Text::OnCreate() {
        Widget::OnCreate();

        GetWidgetRenderer()->SetMaterial(AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font));
    }

    //--------------------------------------------------------------
    void Text::OnRebuildMesh(MeshBuilder &mesh_builder) {
        Mesh *old_mesh = GetWidgetRenderer()->GetMesh();
        Object::Destroy(old_mesh);

        if (m_font == nullptr || m_text == "") {
            return;
        }

        // We try to center the text by default.
        Vec3 world_corners[4];
        GetRectTransform()->GetWorldCorners(world_corners);
        float32 x_center = (world_corners[2].x + world_corners[0].x) / 2.0f;
        float32 y_center = (world_corners[0].y + world_corners[2].y) / 2.0f;

        float32 scale = GetCanvas()->GetScale();
        float32 font_size = static_cast<float32>(m_font->GetSize()) * scale;
        Vec2 text_size = m_font->GetTextSize(m_text, scale);

        Vec3 position = Vec3(x_center - (text_size.x / 2.0f), y_center - (text_size.y / 2.0f), 0.0f);
        position.x = Math::Round(position.x);
        position.y = Math::Round(position.y);
        position.z = Math::Round(position.z);

        mesh_builder.Clear();
        Mesh *new_mesh = GenerateMeshForText(mesh_builder, m_font, m_text, position, scale, m_color);
        GetWidgetRenderer()->SetMesh(new_mesh);
    }

    //--------------------------------------------------------------
    Mesh *Text::GenerateMeshForText(MeshBuilder &mesh_builder, Font *font, const String &text, Vec2 position, float32 scale, Color color) {
        if (text == "") {
            return nullptr;
        }

        Vec2 intial_position = position;

        MeshData mesh_data;
        Vector<uint32> codepoints = StringUtils::GetCodepointsUtf8(text);
        uint32 index = 0;
        for (uint32 codepoint : codepoints) {
            // We first handle the special characters.
            switch (codepoint) {
                case ' ': position.x += font->GetSpecialGlyphs().space.advance * scale; continue;
                case '\t': position.x += font->GetSpecialGlyphs().space.advance * 4 * scale; continue; // Tab is equivalent to 4 whitespaces.
                case '\r': continue; // Carriage return gets just straight up ignored. 
                case '\n': {
                    position.x = intial_position.x;
                    position.y -= font->GetSize() * scale;
                    continue;
                }
            }

            FontAtlasElement element = font->GetElement(codepoint);
            FontGlyph glyph = element.payload;

            float32 x_pos = position.x + glyph.bearing.x * scale;
            float32 y_pos = position.y - (glyph.size.y - glyph.bearing.y) * scale;
            float32 width = glyph.size.x * scale;
            float32 height = glyph.size.y * scale;

            mesh_builder.AddVertex(Vec3(x_pos, y_pos + height, 0.0f), color, element.uv_top_left);
            mesh_builder.AddVertex(Vec3(x_pos + width, y_pos + height, 0.0f), color, element.uv_top_right);
            mesh_builder.AddVertex(Vec3(x_pos + width, y_pos, 0.0f), color, element.uv_bottom_right);
            mesh_builder.AddVertex(Vec3(x_pos, y_pos, 0.0f), color, element.uv_bottom_left);
            mesh_builder.AddTriangle(index, index + 1, index + 2);
            mesh_builder.AddTriangle(index, index + 2, index + 3);
            
            index += 4;
            position.x += glyph.advance * scale;
        }

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Text *Text::Create() {
        return new Text();
    }

}