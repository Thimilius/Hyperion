//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class VulkanRenderDriver final : public RenderDriver {
  public:
    void Initialize() override;
    void HandleAssets(RenderAssetContext &asset_context) override;
    void Render(RenderFrame *render_frame) override;
    void Shutdown() override;
  };

}