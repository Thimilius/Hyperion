#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    enum class UIScalingMode {
        ScaleWithScreenSize,
        ConstantPixelSize
    };

    enum class UIAnchorPreset {
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

    enum class UIVisibility {
        Visible,
        Hidden
    };

    enum class UITextAlignment {
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

    struct UIColorBlock {
        Color normal_color = Color::White();
        Color highlight_color = Color::White();
        Color pressed_color = Color::White();
    };

    struct UIShadow {
        bool enabled = false;
        Color color = Color::Black();
        Vector2 offset = Vector2(1.0f, -1.0f);
    };

}