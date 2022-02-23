//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class OpenGLDebugGroup {
  public:
    OpenGLDebugGroup(const String &message);
    ~OpenGLDebugGroup();
  };

}