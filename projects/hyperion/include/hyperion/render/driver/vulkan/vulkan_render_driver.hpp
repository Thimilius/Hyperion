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

    void RecordCommandBuffer(VkCommandBuffer command_buffer, uint32 image_index);
  private:
    VulkanRenderContext *m_context;

    uint32 m_current_frame_index = 0;
  private:
    friend class VulkanRenderContext;
  };

}
