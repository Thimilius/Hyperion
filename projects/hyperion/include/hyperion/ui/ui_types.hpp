//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  enum class ScalingMode {
    ScaleWithScreenSize,
    ConstantPixelSize
  };

  enum class Corner {
    TopRight,
    BottomRight,
    BottomLeft,
    TopLeft
  };

  enum class AnchorPreset {
    TopLeft,
    TopCenter,
    TopRight,

    MiddleLeft,
    MiddleCenter,
    MiddleRight,

    BottomLeft,
    BottomCenter,
    BottomRight,

    TopStretchHorizontal,
    MiddleStretchHorizontal,
    BottomStretchHorizontal,

    LeftStretchVertical,
    CenterStretchVertical,
    RightStretchVertical,

    StretchAll,
  };

  enum class Visibility {
    Visible,
    Hidden
  };

  struct ColorBlock {
    Color normal_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    Color highlighted_color = Color(0.85f, 0.85f, 0.85f, 1.0f);
    Color pressed_color = Color(0.75f, 0.75f, 0.75f, 1.0f);
    Color selected_color = Color(0.9f, 0.9f, 0.9f, 1.0f);
    Color disabled_color = Color(0.6f, 0.6f, 0.6f, 0.5f);
  };

  struct Shadow {
    bool8 enabled = false;
    Color color = Color::Black();
    Vector2 offset = Vector2(1.0f, -1.0f);
  };

  enum class SelectionState {
    Normal,
    Highlighted,
    Pressed,
    Selected,
    Disabled,
  };

  enum class LayoutType {
    None,
    Horizontal,
    Vertical,
    Grid,
  };

  enum class ChildAlignment {
    TopLeft,
    TopCenter,
    TopRight,

    MiddleLeft,
    MiddleCenter,
    MiddleRight,

    BottomLeft,
    BottomCenter,
    BottomRight,
  };

  enum class TextAlignment {
    TopLeft,
    TopCenter,
    TopRight,

    MiddleLeft,
    MiddleCenter,
    MiddleRight,

    BottomLeft,
    BottomCenter,
    BottomRight,
  };

}