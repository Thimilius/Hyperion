//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    void OpenGLGraphicsUtilities::SetObjectLabel(GLenum type, GLuint object, const GraphicsDeviceObjectDescription &description) {
#ifdef HYP_DEBUG
        HYP_ASSERT(object != 0);

        glObjectLabel(type, object, static_cast<GLsizei>(description.name.size()), description.name.c_str());
#endif
    }

    //--------------------------------------------------------------
    GLenum OpenGLGraphicsUtilities::GetTopology(GraphicsTopology mesh_topology) {
        switch (mesh_topology) {
            case GraphicsTopology::Points: return GL_POINTS;
            case GraphicsTopology::Lines: return GL_LINES;
            case GraphicsTopology::LineStrip: return GL_LINE_STRIP;
            case GraphicsTopology::Triangles: return GL_TRIANGLES;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}