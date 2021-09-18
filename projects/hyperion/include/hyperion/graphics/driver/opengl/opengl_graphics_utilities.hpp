#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsUtilities final {
    public:
        static void SetObjectLabel(GLenum type, GLuint object, const GraphicsDeviceObjectDescription &description);
    private:
        OpenGLGraphicsUtilities() = delete;
        ~OpenGLGraphicsUtilities() = delete;
    };

}