//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/engine.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    constexpr bool g_log_extensions = false;
    constexpr bool g_log_layers = false;

    //--------------------------------------------------------------
    GraphicsContextProperties VulkanGraphicsContext::GetProperties() const {
        return GraphicsContextProperties();
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::Initialize(const GraphicsContextDescriptor &descriptor) {
        Vector<const char *> required_extension_names;
        QueryExtensions();
        CheckExtensions(required_extension_names);

        Vector<const char *> required_layer_names;
        QueryLayers();
        CheckLayers(required_layer_names);

        InitializeInstance(required_extension_names, required_layer_names);
        InitializePhysicalDevice();

#ifdef HYP_DEBUG
        InitializeDebug();
#endif

        HYP_LOG_INFO("Graphics", "Initialized Vulkan graphics driver!");
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::Shutdown() {
#ifdef HYP_DEBUG
        auto destroy_debug_message_function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
        if (destroy_debug_message_function) {
            destroy_debug_message_function(m_instance, m_debug_messenger, nullptr);
        } else {
            HYP_LOG_ERROR("Graphics", "Failed to load Vulkan extension function to destroy debug message listener!");
        }
#endif

        vkDestroyInstance(m_instance, nullptr);
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) {
        HYP_ASSERT(device);
        HYP_ASSERT(device_context);
        HYP_ASSERT(swap_chain);
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::SwapBuffers() {

    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::SetVSyncMode(VSyncMode vsync_mode) {

    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::InitializeDebug() {
        auto create_debug_message_function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
        if (create_debug_message_function) {
            VkDebugUtilsMessengerCreateInfoEXT debug_message_create_info = { };
            debug_message_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debug_message_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debug_message_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debug_message_create_info.pfnUserCallback = DebugMessageCallback;

            HYP_VULKAN_CHECK(create_debug_message_function(m_instance, &debug_message_create_info, nullptr, &m_debug_messenger));
        } else {
            HYP_LOG_ERROR("Graphics", "Failed to load Vulkan extension function to create debug message listener!");
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::InitializeInstance(const Vector<const char *> &required_extension_names, const Vector<const char *> &required_layer_names) {
        VkApplicationInfo application_info = { };
        application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        application_info.pApplicationName = "Hyperion";
        application_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        application_info.pEngineName = "Hyperion";
        application_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        application_info.apiVersion = VK_VERSION_1_2;

        VkInstanceCreateInfo instance_create_info = { };
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &application_info;
        instance_create_info.enabledLayerCount = static_cast<uint32>(required_layer_names.size());
        instance_create_info.ppEnabledLayerNames = required_layer_names.data();
        instance_create_info.enabledExtensionCount = static_cast<uint32>(required_extension_names.size());
        instance_create_info.ppEnabledExtensionNames = required_extension_names.data();
        HYP_VULKAN_CHECK(vkCreateInstance(&instance_create_info, nullptr, &m_instance));
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::InitializePhysicalDevice() {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
        if (device_count == 0) {
            HYP_PANIC_MESSAGE("Graphics", "No Vulkan device available!");
        }

        Vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());
        for (const VkPhysicalDevice &device : devices) {
            VkPhysicalDeviceProperties device_properties;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceFeatures(device, &device_features);

            // NOTE: For now we just pick the very first physical device we find.
            // A more robust implementation would give each possible device a score and pick the one with the highest score.
            // A discrete device could for example get a higher score than an embedded one.

            m_physical_device = device;
            break;
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::QueryExtensions() {
        uint32 extension_count = 0;
        HYP_VULKAN_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr));
        m_extensions = Vector<VkExtensionProperties>(extension_count);
        HYP_VULKAN_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, m_extensions.data()));

        if constexpr (g_log_extensions) {
            for (const VkExtensionProperties &extension : m_extensions) {
                HYP_LOG_INFO("Graphics", "Vulkan extension: '{}' available!", extension.extensionName);
            }
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CheckExtensions(Vector<const char *> &required_extension_names) {
#ifdef HYP_DEBUG
        CheckExtension(required_extension_names, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CheckExtension(Vector<const char *> &required_extension_names, const char *extension_name) {
        required_extension_names.push_back(extension_name);

        auto it = std::find_if(m_extensions.begin(), m_extensions.end(), [extension_name](const VkExtensionProperties &extension) {
            return std::strcmp(extension_name, extension.extensionName);
        });
        if (it == m_extensions.end()) {
            HYP_PANIC_MESSAGE("Graphics", "Manditory Vulkan extension: '{}' not available!", extension_name);
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::QueryLayers() {
        uint32 layer_count = 0;
        HYP_VULKAN_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
        m_layers = Vector<VkLayerProperties>(layer_count);
        HYP_VULKAN_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, m_layers.data()));

        if constexpr (g_log_layers) {
            for (const VkLayerProperties &layer : m_layers) {
                HYP_LOG_INFO("Graphics", "Vulkan layer: '{}' available!\n{}", layer.layerName, layer.description);
            }
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CheckLayers(Vector<const char *> &required_layer_names) {
#ifdef HYP_DEBUG
        CheckLayer(required_layer_names, "VK_LAYER_KHRONOS_validation");
#endif
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CheckLayer(Vector<const char *> &required_layer_names, const char *layer_name) {
        required_layer_names.push_back(layer_name);

        auto it = std::find_if(m_layers.begin(), m_layers.end(), [layer_name](const VkLayerProperties &layer) {
            return std::strcmp(layer_name, layer.layerName);
        });
        if (it == m_layers.end()) {
            HYP_PANIC_MESSAGE("Graphics", "Manditory Vulkan layer: '{}' not available!", layer_name);
        }
    }

    //--------------------------------------------------------------
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanGraphicsContext::DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_pointer) {
        const char *type_string = nullptr;
        switch (type) 	{
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: type_string = "General"; break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: type_string = "Validation"; break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: type_string = "Performance"; break;
            default: type_string = "Unknown"; break;
        }

        const char *log_string_format = "Severity: {}, Type: {},\nMessage: {}";
        if (severity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            HYP_LOG_TRACE("Graphics", log_string_format, "Verbose", type_string, callback_data->pMessage);
        } else if (severity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            HYP_LOG_INFO("Graphics", log_string_format, "Info", type_string, callback_data->pMessage);
        } else if (severity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            HYP_LOG_WARN("Graphics", log_string_format, "Warning", type_string, callback_data->pMessage);
        } else if (severity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            HYP_LOG_ERROR("Graphics", log_string_format, "Error", type_string, callback_data->pMessage);
        }

        return VK_FALSE;
    }

}