//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/text_mesh_generator.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Mesh *TextMeshGenerator::GenerateMesh(const TextMeshGenerationSettings &settings) {
        MeshBuilder mesh_builder;
        return GenerateMesh(settings, mesh_builder);
    }

    //--------------------------------------------------------------
    Mesh *TextMeshGenerator::GenerateMesh(const TextMeshGenerationSettings &settings, MeshBuilder &mesh_builder) {
        mesh_builder.Clear();

        Font *font = settings.font;
        Color color = settings.color;
        float32 scale_x = settings.scale.x;
        float32 scale_y = settings.scale.y;

        TextSize text_size = font->GetTextSize(settings.text, scale_x);
        Vector2 position = GetPosition(settings.alignment, text_size, settings.rect);
        Vector2 intial_position = position;

        Array<uint32> codepoints = StringUtils::GetCodepointsUtf8(settings.text);
        uint32 index = 0;
        for (uint32 codepoint : codepoints) {
            // We first handle the special characters.
            switch (codepoint) {
                case ' ': position.x += font->GetSpecialGlyphs().space.advance * scale_x; continue;
                case '\t': position.x += font->GetSpecialGlyphs().space.advance * 4 * scale_x; continue; // Tab is equivalent to 4 whitespaces.
                case '\r': continue; // Carriage return gets just straight up ignored. 
                case '\n':
                {
                    position.x = intial_position.x;
                    position.y -= font->GetSize() * scale_y;
                    continue;
                }
            }

            FontAtlasElement element = font->GetElement(codepoint);
            FontGlyph glyph = element.payload;

            float32 x_pos = position.x + glyph.bearing.x * scale_x;
            float32 y_pos = position.y - (glyph.size.y - glyph.bearing.y) * scale_y;
            float32 width = glyph.size.x * scale_x;
            float32 height = glyph.size.y * scale_y;

            mesh_builder.AddVertex(Vector3(x_pos, y_pos + height, 0.0f), color, element.uv_top_left);
            mesh_builder.AddVertex(Vector3(x_pos + width, y_pos + height, 0.0f), color, element.uv_top_right);
            mesh_builder.AddVertex(Vector3(x_pos + width, y_pos, 0.0f), color, element.uv_bottom_right);
            mesh_builder.AddVertex(Vector3(x_pos, y_pos, 0.0f), color, element.uv_bottom_left);
            mesh_builder.AddTriangle(index, index + 1, index + 2);
            mesh_builder.AddTriangle(index, index + 2, index + 3);

            index += 4;
            position.x += glyph.advance * scale_x;
        }

        if (settings.rotation != Quaternion::Identity()) {
            // We need to rotate around the center, which involves setting the proper transformation.
            Vector2 center = settings.rect.GetCenter();
            Matrix4x4 transformation = Matrix4x4::Translate(Vector3(center, 0.0f)) * Matrix4x4::Rotate(settings.rotation) * Matrix4x4::Translate(-Vector3(center, 0.0f));
            mesh_builder.Transform(transformation);
        }

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Vector2 TextMeshGenerator::GetPosition(UI::UITextAlignment text_alignment, TextSize text_size, Rect rect) {
        Vector2 position = Vector2();

        switch (text_alignment) {
            case UI::UITextAlignment::TopLeft: {
                position = Vector2(rect.position.x, rect.GetMax().y - text_size.height - text_size.baseline_offset);
                break;
            }
            case UI::UITextAlignment::TopCenter: {
                position = Vector2(rect.GetCenter().x - (text_size.width / 2.0f), rect.GetMax().y - text_size.height - text_size.baseline_offset);
                break;
            }
            case UI::UITextAlignment::TopRight: {
                Vector2 max = rect.GetMax();
                position = Vector2(max.x - text_size.width, max.y - text_size.height - text_size.baseline_offset);
                break;
            }
            case UI::UITextAlignment::MiddleLeft: {
                position = Vector2(rect.position.x, rect.GetCenter().y - (text_size.height / 2.0f));
                break;
            }
            case UI::UITextAlignment::MiddleCenter: {
                Vector2 center = rect.GetCenter();
                position = Vector2(center.x - (text_size.width / 2.0f), center.y - (text_size.height / 2.0f));
                break;
            }
            case UI::UITextAlignment::MiddleRight: {
                position = Vector2(rect.GetMax().x - text_size.width, rect.GetCenter().y - (text_size.height / 2.0f));
                break;
            }
            case UI::UITextAlignment::BottomLeft: {
                position = Vector2(rect.position.x, rect.position.y + text_size.baseline_offset);
                break;
            }
            case UI::UITextAlignment::BottomCenter: {
                position = Vector2(rect.GetCenter().x - (text_size.width / 2.0f), rect.position.y + text_size.baseline_offset);
                break;
            }
            case UI::UITextAlignment::BottomRight: {
                position = Vector2(rect.GetMax().x - text_size.width, rect.position.y + text_size.baseline_offset);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        return Vector3(position, 0.0f);
    }

}