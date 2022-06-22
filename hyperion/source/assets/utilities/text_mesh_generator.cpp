//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/text_mesh_generator.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/utilities/text_utilities.hpp"

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
    HYP_PROFILE_SCOPE("TextMeshGenerator.GenerateMesh")
    
    Font *font = settings.font;
    Color color = settings.color;
    float32 scale_x = settings.scale.x;
    float32 scale_y = settings.scale.y;

    uint32 codepoint_offset = 0;
    Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(settings.text);

    // TODO: We could probably save some time if we were to compute all line sizes and the full text size in one go.
    TextSize full_text_size = TextUtilities::GetTextSize(font, codepoints, 0, scale_x, false);
    TextSize first_line_text_size = TextUtilities::GetTextSize(font, codepoints, 0, scale_x, true);
    Vector2 position = TextUtilities::GetPosition(settings.alignment, first_line_text_size, full_text_size, settings.rect, settings.offset);

    for (uint32 codepoint : codepoints) {
      codepoint_offset++;

      // We first handle the special characters.
      switch (codepoint) {
        case ' ': position.x += static_cast<float32>(font->GetSpecialGlyphs().space.advance) * scale_x;
          continue;
        case '\t': position.x += static_cast<float32>(font->GetSpecialGlyphs().space.advance) * 4 * scale_x;
          continue; // Tab is equivalent to 4 whitespaces.
        case '\r': continue; // Carriage return gets just straight up ignored. 
        case '\n': {
          TextSize next_line_text_size = TextUtilities::GetTextSize(font, codepoints, codepoint_offset, scale_x, true);
          Vector2 new_position = TextUtilities::GetPosition(settings.alignment, next_line_text_size, full_text_size, settings.rect, settings.offset);
          position.x = new_position.x;
          position.y -= static_cast<float32>(font->GetSize()) * scale_y;
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

      position.x += static_cast<float32>(glyph.advance) * scale_x;
    }
  }

}
