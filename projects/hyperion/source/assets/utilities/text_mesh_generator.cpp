//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/text_mesh_generator.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Mesh *TextMeshGenerator::GenerateMesh(const TextMeshGenerationSettings &settings) {
        return GenerateMesh(settings, MeshBuilder());
    }

    //--------------------------------------------------------------
    Mesh *TextMeshGenerator::GenerateMesh(const TextMeshGenerationSettings &settings, MeshBuilder &mesh_builder) {
        mesh_builder.Clear();

        Font *font = settings.font;
        Color color = settings.color;
        float32 scale = settings.scale;

        Vec2 position = GetPosition(settings.alignment, font->GetTextSize(settings.text, scale), settings.rect);
        Vec2 intial_position = position;

        MeshData mesh_data;
        Vector<uint32> codepoints = StringUtils::GetCodepointsUtf8(settings.text);
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
    Vec2 TextMeshGenerator::GetPosition(TextAlignment text_alignment, Vec2 text_size, Rect rect) {
        Vec2 position = Vec2();

        switch (text_alignment) {
            case TextAlignment::TopLeft: {
                position = Vec2(rect.position.x, rect.GetMax().y - text_size.y);
                break;
            }
            case TextAlignment::TopCenter: {
                position = Vec2(rect.GetCenter().x - (text_size.x / 2.0f), rect.GetMax().y - text_size.y);
                break;
            }
            case TextAlignment::TopRight: {
                Vec2 max = rect.GetMax();
                position = Vec2(max.x - text_size.x, max.y - text_size.y);
                break;
            }
            case TextAlignment::MiddleLeft: {
                position = Vec2(rect.position.x, rect.GetCenter().y - (text_size.y / 2.0f));
                break;
            }
            case TextAlignment::MiddleCenter: {
                Vec2 center = rect.GetCenter();
                position = Vec2(center.x - (text_size.x / 2.0f), center.y - (text_size.y / 2.0f));
                break;
            }
            case TextAlignment::MiddleRight: {
                position = Vec2(rect.GetMax().x - text_size.x, rect.GetCenter().y - (text_size.y / 2.0f));
                break;
            }
            case TextAlignment::BottomLeft: {
                position = Vec2(rect.position.x, rect.position.y);
                break;
            }
            case TextAlignment::BottomCenter: {
                position = Vec2(rect.GetCenter().x - (text_size.x / 2.0f), rect.position.y);
                break;
            }
            case TextAlignment::BottomRight: {
                position = Vec2(rect.GetMax().x - text_size.x, rect.position.y);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        position.x = Math::Round(position.x);
        position.y = Math::Round(position.y);

        return Vec3(position, 0.0f);
    }

}