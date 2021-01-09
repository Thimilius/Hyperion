#pragma once

#include "hyperion/core/color.hpp"

namespace Hyperion::Rendering {

    enum class ClearMask {
        None,

        Color   = BIT(0),
        Depth   = BIT(1),
        Stencil = BIT(2)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ClearMask);

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Clear(ClearMask clear_mask, Color color) = 0;
    };

}