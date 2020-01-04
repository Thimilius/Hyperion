#include "hyppch.hpp"

#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    GraphicsContext::GraphicsContext() {
        HYP_ASSERT_MESSAGE(!s_instance, "Trying to create graphics context more than once!");
        s_instance = this;
    }

}