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

    struct ColorBlock {
        Color normal_color = Color::White();
        Color highlight_color = Color::White();
        Color pressed_color = Color::White();
        Color disabled_color = Color::White();
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

}