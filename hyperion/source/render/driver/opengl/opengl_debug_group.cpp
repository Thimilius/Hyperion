//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_debug_group.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  OpenGLDebugGroup::OpenGLDebugGroup(const String &message) {
#ifndef HYP_RELEASE
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, static_cast<GLsizei>(message.length()), message.c_str());
#endif
  }

  //--------------------------------------------------------------
  OpenGLDebugGroup::~OpenGLDebugGroup() {
#ifndef HYP_RELEASE
    glPopDebugGroup();
#endif
  }

}
