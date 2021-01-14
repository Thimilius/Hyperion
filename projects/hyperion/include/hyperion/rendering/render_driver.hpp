#pragma once

#include "hyperion/core/color.hpp"

namespace Hyperion::Rendering {

    enum class ClearFlags {
        None,

        Color   = BIT(0),
        Depth   = BIT(1),
        Stencil = BIT(2)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ClearFlags);

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
    };

}