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

}