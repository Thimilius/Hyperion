//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/render_stats.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
  class RenderAssetContext;
  class RenderFrame;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class IRenderDriver {
  public:
    virtual ~IRenderDriver() = default;
  public:
    virtual void Initialize() = 0;
    virtual void HandleAssets(RenderAssetContext &asset_context) = 0;
    virtual void Render(RenderFrame *render_frame) = 0;
    virtual void Shutdown() = 0;

    virtual void ResetStats() = 0;
    virtual RenderStats GetStats() const = 0;
  };

  class RenderDriver : public IRenderDriver {
  public:
    virtual ~RenderDriver() = default;
  public:
    inline void ResetStats() override { m_stats = RenderStats(); }
    inline RenderStats GetStats() const override { return m_stats; }
  protected:
    RenderStats m_stats;
  };

}
