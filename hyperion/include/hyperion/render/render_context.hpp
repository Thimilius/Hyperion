//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/window.hpp"
#include "hyperion/render/render_context_types.hpp"
#include "hyperion/render/driver/render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class IRenderContext : public INonCopyable {
  public:
    virtual ~IRenderContext() = default;
  public:
    virtual RenderContextProperties GetProperties() const = 0;

    virtual IRenderDriver *GetDriver() = 0;

    virtual void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) = 0;
    virtual void Shutdown() = 0;

    virtual void MakeCurrent(Window *window) = 0;
    virtual void SwapBuffers(Window *window) = 0;
    virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
  };

}
