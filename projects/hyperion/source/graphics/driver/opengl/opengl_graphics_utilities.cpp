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

}