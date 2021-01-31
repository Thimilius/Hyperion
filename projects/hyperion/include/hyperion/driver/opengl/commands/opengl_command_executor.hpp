#pragma once

#include "hyperion/driver/opengl/commands/opengl_commands.hpp"

namespace Hyperion::Rendering {
    class OpenGLRenderDriver;
}

namespace Hyperion::Rendering {

    class OpenGLCommandExecutor final {
    public:
        static uint64 Execute(OpenGLRenderDriver *opengl_render_driver, OpenGLCommandType opengl_command_type, void *opengl_command);
    private:
        OpenGLCommandExecutor() = delete;
        ~OpenGLCommandExecutor() = delete;
    };

}