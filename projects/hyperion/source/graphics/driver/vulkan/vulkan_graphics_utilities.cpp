//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VkPolygonMode VulkanGraphicsUtilities::GetPolygonMode(GraphicsPolygonMode polygon_mode) {
        switch (polygon_mode) 	{
            case GraphicsPolygonMode::Fill: return VK_POLYGON_MODE_FILL;
            case GraphicsPolygonMode::Line: return VK_POLYGON_MODE_LINE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return VK_POLYGON_MODE_FILL;
        }
    }

    //--------------------------------------------------------------
    VkCullModeFlags VulkanGraphicsUtilities::GetCullingMode(GraphicsCullingMode culling_mode) {
        switch (culling_mode) 	{
            case GraphicsCullingMode::None: return VK_CULL_MODE_NONE;
            case GraphicsCullingMode::Front: return VK_CULL_MODE_FRONT_BIT;
            case GraphicsCullingMode::Back: return VK_CULL_MODE_BACK_BIT;
            case GraphicsCullingMode::FrontAndBack: return VK_CULL_MODE_FRONT_AND_BACK;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return VK_CULL_MODE_NONE;
        }
    }

    //--------------------------------------------------------------
    VkFrontFace VulkanGraphicsUtilities::GetCullingFrontFaceMode(GraphicsCullingFrontFaceMode culling_front_face_mode) {
        switch (culling_front_face_mode) 	{
            case GraphicsCullingFrontFaceMode::Clockwise: return VK_FRONT_FACE_CLOCKWISE;
            case GraphicsCullingFrontFaceMode::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return VK_FRONT_FACE_CLOCKWISE;
        }
    }

    //--------------------------------------------------------------
    VkBlendFactor VulkanGraphicsUtilities::GetBlendingFactor(GraphicsBlendingFactor blending_factor) {
        switch (blending_factor) 	{
            case GraphicsBlendingFactor::Zero: return VK_BLEND_FACTOR_ZERO;
            case GraphicsBlendingFactor::One: return VK_BLEND_FACTOR_ONE;
            case GraphicsBlendingFactor::SourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
            case GraphicsBlendingFactor::SourceColor: return VK_BLEND_FACTOR_SRC_COLOR;
            case GraphicsBlendingFactor::DestinationAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
            case GraphicsBlendingFactor::DestinationColor: return VK_BLEND_FACTOR_DST_COLOR;
            case GraphicsBlendingFactor::InverseSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            case GraphicsBlendingFactor::InverseSourceColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            case GraphicsBlendingFactor::InverseDestinationAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            case GraphicsBlendingFactor::InverseDestinationColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return VK_BLEND_FACTOR_ZERO;
        }
    }

    //--------------------------------------------------------------
    VkBlendOp VulkanGraphicsUtilities::GetBlendingOperator(GraphicsBlendingOperator blending_operator) {
        switch (blending_operator) 	{
            case GraphicsBlendingOperator::Add: return VK_BLEND_OP_ADD;
            case GraphicsBlendingOperator::Subtract:  return VK_BLEND_OP_SUBTRACT;
            case GraphicsBlendingOperator::ReverseSubract:  return VK_BLEND_OP_REVERSE_SUBTRACT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return VK_BLEND_OP_ADD;
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsUtilities::ReportError(VkResult result) {
        const char *error_message = nullptr;
        switch (result) {
            case VK_NOT_READY: error_message = "Not ready"; break;
            case VK_TIMEOUT: error_message = "Timeout"; break;
            case VK_EVENT_SET: error_message = "Event set"; break;
            case VK_EVENT_RESET: error_message = "Event reset"; break;
            case VK_INCOMPLETE: error_message = "Incomplete"; break;
            case VK_ERROR_OUT_OF_HOST_MEMORY: error_message = "Out of host memory"; break;
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: error_message = "Out of device memory"; break;
            case VK_ERROR_INITIALIZATION_FAILED: error_message = "Initialization failed"; break;
            case VK_ERROR_DEVICE_LOST: error_message = "Device lost"; break;
            case VK_ERROR_MEMORY_MAP_FAILED: error_message = "Memory map failed"; break;
            case VK_ERROR_LAYER_NOT_PRESENT: error_message = "Layer not present"; break;
            case VK_ERROR_EXTENSION_NOT_PRESENT: error_message = "Extension not present"; break;
            case VK_ERROR_FEATURE_NOT_PRESENT: error_message = "Feature not present"; break;
            case VK_ERROR_INCOMPATIBLE_DRIVER: error_message = "Incompatible driver"; break;
            case VK_ERROR_TOO_MANY_OBJECTS: error_message = "Too many objects"; break;
            case VK_ERROR_FORMAT_NOT_SUPPORTED: error_message = "Format not supported"; break;
            case VK_ERROR_FRAGMENTED_POOL: error_message = "Fragmented pool"; break;
            case VK_ERROR_UNKNOWN: error_message = "Unknown"; break;
            case VK_ERROR_OUT_OF_POOL_MEMORY: error_message = "Out of pool memory"; break;
            case VK_ERROR_INVALID_EXTERNAL_HANDLE: error_message = "Invalid external handle"; break;
            case VK_ERROR_FRAGMENTATION: error_message = "Fragmentation"; break;
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: error_message = "Invalid opaque capture address"; break;
            case VK_ERROR_SURFACE_LOST_KHR: error_message = "Surface lost"; break;
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: error_message = "Native window in use"; break;
            case VK_SUBOPTIMAL_KHR: error_message = "Supotimal"; break;
            case VK_ERROR_OUT_OF_DATE_KHR: error_message = "Out of date"; break;
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: error_message = "Incompatible display"; break;
            case VK_ERROR_VALIDATION_FAILED_EXT: error_message = "Validation failed"; break;
            case VK_ERROR_INVALID_SHADER_NV: error_message = "Invalid shader"; break;
            case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: error_message = "Invalid drm format modifier plane layout"; break;
            case VK_ERROR_NOT_PERMITTED_EXT: error_message = "Not permitted"; break;
            case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: error_message = "Full screen exclusive mode lost"; break;
            case VK_THREAD_IDLE_KHR: error_message = "Thread idle"; break;
            case VK_THREAD_DONE_KHR: error_message = "Thread done"; break;
            case VK_OPERATION_DEFERRED_KHR: error_message = "Operation deferred"; break;
            case VK_OPERATION_NOT_DEFERRED_KHR: error_message = "Operation not deferred"; break;
            case VK_PIPELINE_COMPILE_REQUIRED_EXT: error_message = "Pipeline compile required"; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }

        HYP_LOG_ERROR("Graphics", "Vulkan error: '{}'!", error_message);
    }

}