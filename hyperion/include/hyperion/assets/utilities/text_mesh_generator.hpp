//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct TextMeshGenerationSettings {
    String text;
    Font *font = nullptr;
    UI::TextAlignment alignment = UI::TextAlignment::MiddleCenter;
    Color color = Color::White();

    Rect rect = Rect();
    Vector2 scale = Vector2(1.0f, 1.0f);

    Vector2 offset;
  };

  class TextMeshGenerator final {
  public:
    static Mesh *GenerateMesh(const TextMeshGenerationSettings &settings);
    static void GenerateMesh(const TextMeshGenerationSettings &settings, MeshBuilder &mesh_builder);
  private:
    TextMeshGenerator() = delete;
    ~TextMeshGenerator() = delete;
  };

}
