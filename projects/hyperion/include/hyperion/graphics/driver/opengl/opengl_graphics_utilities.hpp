#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"
#include "hyperion/graphics/graphics_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsUtilities final {
    public:
        static void SetObjectLabel(GLenum type, GLuint object, const GraphicsDeviceObjectDescription &description);

        static GLenum GetTopology(GraphicsTopology mesh_topology);
    private:
        OpenGLGraphicsUtilities() = delete;
        ~OpenGLGraphicsUtilities() = delete;
    };

}