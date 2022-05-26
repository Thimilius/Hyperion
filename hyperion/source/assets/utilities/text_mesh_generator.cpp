//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/text_mesh_generator.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Mesh *TextMeshGenerator::GenerateMesh(const TextMeshGenerationSettings &settings) {
    MeshBuilder mesh_builder;
    GenerateMesh(settings, mesh_builder);
    return mesh_builder.CreateMesh();
  }

  //--------------------------------------------------------------
  void TextMeshGenerator::GenerateMesh(const TextMeshGenerationSettings &settings, MeshBuilder &mesh_builder) {
    Font *font = settings.font;
    Color color = settings.color;
    float32 scale_x = settings.scale.x;
    float32 scale_y = settings.scale.y;

    uint32 codepoint_offset = 0;
    Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(settings.text);

    // TODO: We could probably save some time if we were to compute all line sizes and the full text size in one go.
    TextSize full_text_size = font->GetTextSize(codepoints, 0, scale_x, false);
    TextSize first_line_text_size = font->GetTextSize(codepoints, 0, scale_x, true);
    Vector2 position = GetPosition(settings.alignment, first_line_text_size, full_text_size, settings.rect);

    for (uint32 codepoint : codepoints) {
      codepoint_offset++;

      // We first handle the special characters.
      switch (codepoint) {
        case ' ': position.x += font->GetSpecialGlyphs().space.advance * scale_x;
          continue;
        case '\t': position.x += font->GetSpecialGlyphs().space.advance * 4 * scale_x;
          continue; // Tab is equivalent to 4 whitespaces.
        case '\r': continue; // Carriage return gets just straight up ignored. 
        case '\n': {
          TextSize next_line_text_size = font->GetTextSize(codepoints, codepoint_offset, scale_x, true);
          Vector2 new_position = GetPosition(settings.alignment, next_line_text_size, full_text_size, settings.rect);
          position.x = new_position.x;
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

      Vector3 corners[4] = {
        Vector3(x_pos, y_pos + height, 0.0f),
        Vector3(x_pos + width, y_pos + height, 0.0f),
        Vector3(x_pos + width, y_pos, 0.0f),
        Vector3(x_pos, y_pos, 0.0f)
      };
      Vector2 uvs[4] = {
        element.uv_top_left,
        element.uv_top_right,
        element.uv_bottom_right,
        element.uv_bottom_left,
      };
      mesh_builder.AddQuad(corners, color, uvs);

      position.x += glyph.advance * scale_x;
    }
  }

  //--------------------------------------------------------------
  Vector2 TextMeshGenerator::GetPosition(UI::TextAlignment text_alignment, TextSize line_size, TextSize full_size, Rect rect) {
    Vector2 position = Vector2();

    switch (text_alignment) {
      case UI::TextAlignment::TopLeft: {
        position = Vector2(rect.position.x, rect.GetMax().y - line_size.height);
        break;
      }
      case UI::TextAlignment::TopCenter: {
        position = Vector2(rect.GetCenter().x - (line_size.width / 2.0f), rect.GetMax().y - line_size.height);
        break;
      }
      case UI::TextAlignment::TopRight: {
        Vector2 max = rect.GetMax();
        position = Vector2(max.x - line_size.width, max.y - line_size.height);
        break;
      }
      case UI::TextAlignment::MiddleLeft: {
        position = Vector2(rect.position.x, rect.GetCenter().y + (full_size.height / 2.0f) - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::MiddleCenter: {
        Vector2 center = rect.GetCenter();
        position = Vector2(center.x - (line_size.width / 2.0f), center.y + (full_size.height / 2.0f) - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::MiddleRight: {
        position = Vector2(rect.GetMax().x - line_size.width, rect.GetCenter().y + (full_size.height / 2.0f) - line_size.height  + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::BottomLeft: {
        position = Vector2(rect.position.x, rect.position.y + full_size.height - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::BottomCenter: {
        position = Vector2(rect.GetCenter().x - (line_size.width / 2.0f), rect.position.y + full_size.height - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::BottomRight: {
        position = Vector2(rect.GetMax().x - line_size.width, rect.position.y + full_size.height - line_size.height + line_size.baseline_offset);
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    return Vector3(position, 0.0f);
  }

}
