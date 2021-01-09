#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Clear(ClearMask clear_mask, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GetGLClearMask(clear_mask));
    }

    u32 OpenGLRenderDriver::GetGLClearMask(ClearMask clear_mask) {
        u32 result = 0;

        if ((clear_mask & ClearMask::Color) == ClearMask::Color) {
            result |= GL_COLOR_BUFFER_BIT;
        }
        if ((clear_mask & ClearMask::Depth) == ClearMask::Depth) {
            result |= GL_DEPTH_BUFFER_BIT;
        }
        if ((clear_mask & ClearMask::Stencil) == ClearMask::Stencil) {
            result |= GL_STENCIL_BUFFER_BIT;
        }

        return result;
    }

}