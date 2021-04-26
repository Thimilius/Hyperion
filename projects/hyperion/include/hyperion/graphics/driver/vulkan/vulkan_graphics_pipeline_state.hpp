#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_pipeline_state.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class VulkanGraphicsDevice;
}

namespace Hyperion::Graphics {

    class VulkanGraphicsPipelineState : public GraphicsPipelineState {
    public:
        VulkanGraphicsPipelineState(VulkanGraphicsDevice *device, const GraphicsPipelineStateDescription &description);
    private:
        VkPipeline m_pipeline;
    };

}