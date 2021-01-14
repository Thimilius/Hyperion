#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Clear(ClearFlags clear_mask, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GetGLClearMask(clear_mask));
    }

    u32 OpenGLRenderDriver::GetGLClearMask(ClearFlags clear_mask) {
        u32 result = 0;

        if ((clear_mask & ClearFlags::Color) == ClearFlags::Color) {
            result |= GL_COLOR_BUFFER_BIT;
        }
        if ((clear_mask & ClearFlags::Depth) == ClearFlags::Depth) {
            result |= GL_DEPTH_BUFFER_BIT;
        }
        if ((clear_mask & ClearFlags::Stencil) == ClearFlags::Stencil) {
            result |= GL_STENCIL_BUFFER_BIT;
        }

        return result;
    }

}