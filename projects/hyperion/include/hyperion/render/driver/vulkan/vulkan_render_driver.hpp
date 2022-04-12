//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/render_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
  class VulkanRenderContext;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class VulkanRenderDriver final : public RenderDriver {
  public:
    void Initialize() override;
    void HandleAssets(RenderAssetContext &asset_context) override;
    void Render(RenderFrame *render_frame) override;
    void Shutdown() override;
  private:
    void Setup(VulkanRenderContext *context);
  private:
    VulkanRenderContext *m_context;
  private:
    friend class VulkanRenderContext;
  };

}