//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/ui/ui_event.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
  class UIElement;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  struct UIImmediateMesh {
    Mesh *mesh;
    Material *material;
    Texture *texture;

    Color color;
    bool8 enable_blending;
    
    uint32 render_texture_attachment_index;
  };
  
  class UIImmediate final {
  public:
    static void Begin();
    static void DrawRect(Vector2 position, Vector2 size, Color color);
    static void DrawText(const String &text, Font *font, Vector2 position, UI::TextAlignment alignment, Color color);
    static void End();
  private:
    UIImmediate() = delete;
    ~UIImmediate() = delete;
  private:
    static void Flush(Material *material = nullptr, Texture *texture = nullptr);
  private:
    inline static MeshBuilder s_mesh_builder;
    inline static Array<UIImmediateMesh> s_meshes;
  };
  
}