//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    void OpenGLGraphicsUtilities::SetObjectLabel(GLenum type, GLuint object, const GraphicsDeviceObjectDescription &description) {
        HYP_ASSERT(object != 0);

#ifdef HYP_DEBUG
        glObjectLabel(type, object, static_cast<GLsizei>(description.name.size()), description.name.c_str());
#endif
    }

}