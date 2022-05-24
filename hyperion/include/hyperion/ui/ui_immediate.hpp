﻿//------------------------ Header Guard ------------------------
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

  enum class DockingPosition {
    Full,
    Top,
    Bottom,
    Left,
    Right,
  };

  struct UIImmediateLayout {
    Vector2 full_size;
    Vector2 full_pivot;
    Vector3 full_position;

    Vector2 leftover_size;
    Vector2 leftover_pivot;
    Vector3 leftover_position;

    Array<UIImmediateLayout> children;
  };
  
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
    static void End();

    static void BeginWindow(const String &name, DockingPosition docking_position, float32 extent, Color color, bool8 split = false);
    static void EndWindow();
    
    static void DrawRect(Vector2 position, Vector2 size, Color color);
    static void DrawText(const String &text, Font *font, Vector2 position, UI::TextAlignment alignment, Color color);
  private:
    UIImmediate() = delete;
    ~UIImmediate() = delete;
  private:
    static void Flush(Material *material = nullptr, Texture *texture = nullptr);
  private:
    inline static MeshBuilder s_mesh_builder;
    inline static Array<UIImmediateMesh> s_meshes;

    inline static Array<UIImmediateLayout> s_layout_stack;
  };
  
}