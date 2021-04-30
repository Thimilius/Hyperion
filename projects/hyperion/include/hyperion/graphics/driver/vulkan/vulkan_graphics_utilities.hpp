#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/graphics/graphics_blending_state.hpp"
#include "hyperion/graphics/graphics_rasterizer_state.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsUtilities final {
    public:
        static VkPolygonMode GetPolygonMode(GraphicsPolygonMode polygon_mode);
        static VkCullModeFlags GetCullingMode(GraphicsCullingMode culling_mode);
        static VkFrontFace GetCullingFrontFaceMode(GraphicsCullingFrontFaceMode culling_front_face_mode);

        static VkBlendFactor GetBlendingFactor(GraphicsBlendingFactor blending_factor);
        static VkBlendOp GetBlendingOperator(GraphicsBlendingOperator blending_operator);

        static void ReportError(VkResult result);
    private:
        VulkanGraphicsUtilities() = delete;
        ~VulkanGraphicsUtilities() = delete;
    };

}

#define HYP_VULKAN_CHECK(x)                                                   \
    do {                                                                      \
        VkResult result = (x);                                                \
        if (result != VK_SUCCESS) {                                           \
            HYP_DEBUG_BREAK;                                                  \
            Hyperion::Graphics::VulkanGraphicsUtilities::ReportError(result); \
        }                                                                     \
    } while (false)