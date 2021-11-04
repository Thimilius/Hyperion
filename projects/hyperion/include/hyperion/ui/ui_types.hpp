//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    enum class ScalingMode {
        ScaleWithScreenSize,
        ConstantPixelSize
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
        Color highlight_color = Color(0.9f, 0.9f, 0.9f, 1.0f);
        Color pressed_color = Color(0.75f, 0.75f, 0.75f, 1.0f);
        Color disabled_color = Color(0.6f, 0.6f, 0.6f, 0.5f);
    };

    struct Shadow {
        bool enabled = false;
        Color color = Color::Black();
        Vector2 offset = Vector2(1.0f, -1.0f);
    };

    enum class SelectionState {
        Normal,
        Highlighted,
        Pressed,
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