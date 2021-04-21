//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

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