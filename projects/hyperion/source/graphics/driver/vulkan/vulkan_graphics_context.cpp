//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/engine.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_swap_chain.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    constexpr bool g_log_extensions = false;
    constexpr bool g_log_layers = false;
    constexpr VkDebugUtilsMessageSeverityFlagBitsEXT g_debug_message_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

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
        InitializeSurface();
        InitializePhysicalDevice();
        InitializeQueueFamilyIndices();

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

        VkDevice logical_device;
        {
            Vector<const char *> required_device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            {
                uint32 device_extensions_count;
                HYP_VULKAN_CHECK(vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &device_extensions_count, nullptr));
                Vector<VkExtensionProperties> device_extensions(device_extensions_count);
                HYP_VULKAN_CHECK(vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &device_extensions_count, device_extensions.data()));

                for (const char *device_extension_name : required_device_extensions) {
                    auto it = std::find_if(device_extensions.begin(), device_extensions.end(), [device_extension_name](const VkExtensionProperties& extension_property) {
                        return std::strcmp(extension_property.extensionName, device_extension_name) == 0;
                    });
                    if (it == device_extensions.end()) {
                        HYP_PANIC_MESSAGE("Graphics", "Manditory Vulkan device extension: '{}' not available!", device_extension_name);
                    }
                }
            }

            Vector<VkDeviceQueueCreateInfo> device_queue_create_infos;
            Set<uint32> unique_queue_family_indices = { m_queue_family_indices.graphics_family_index, m_queue_family_indices.present_family_index };

            float32 queue_priority = 1.0f;
            for (uint32 queue_family_index : unique_queue_family_indices) {
                VkDeviceQueueCreateInfo device_queue_create_info = { };
                device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                device_queue_create_info.queueFamilyIndex = queue_family_index;
                device_queue_create_info.queueCount = 1;
                device_queue_create_info.pQueuePriorities = &queue_priority;
                device_queue_create_infos.push_back(device_queue_create_info);
            }

            VkPhysicalDeviceFeatures physical_device_features = { };

            VkDeviceCreateInfo device_create_info = { };
            device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            device_create_info.queueCreateInfoCount = static_cast<uint32>(device_queue_create_infos.size());
            device_create_info.pQueueCreateInfos = device_queue_create_infos.data();
            device_create_info.pEnabledFeatures = &physical_device_features;
            device_create_info.enabledExtensionCount = static_cast<uint32>(required_device_extensions.size());
            device_create_info.ppEnabledExtensionNames = required_device_extensions.data();

            HYP_VULKAN_CHECK(vkCreateDevice(m_physical_device, &device_create_info, nullptr, &logical_device));
            VkQueue graphics_queue;
            vkGetDeviceQueue(logical_device, m_queue_family_indices.graphics_family_index, 0, &graphics_queue);

            *device = new VulkanGraphicsDevice(logical_device, graphics_queue);
        }

        {
            VkQueue present_queue;
            vkGetDeviceQueue(logical_device, m_queue_family_indices.present_family_index, 0, &present_queue);

            *swap_chain = new VulkanGraphicsSwapChain(this, m_surface, present_queue);
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::SwapBuffers() {

    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::SetVSyncMode(VSyncMode vsync_mode) {

    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CheckExtensions(Vector<const char *> &required_extension_names) {
#ifdef HYP_DEBUG
        CheckExtension(required_extension_names, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
        CheckExtension(required_extension_names, VK_KHR_SURFACE_EXTENSION_NAME);
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
    void VulkanGraphicsContext::InitializeSurface() {
        HYP_VULKAN_CHECK(CreateSurface(&m_surface));
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::InitializePhysicalDevice() {
        uint32 device_count = 0;
        HYP_VULKAN_CHECK(vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr));
        if (device_count == 0) {
            HYP_PANIC_MESSAGE("Graphics", "No Vulkan device available!");
        }

        Vector<VkPhysicalDevice> devices(device_count);
        HYP_VULKAN_CHECK(vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data()));
        for (const VkPhysicalDevice &device : devices) {
            VkPhysicalDeviceProperties device_properties;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceFeatures(device, &device_features);

            // NOTE: For now we just pick the very first physical device we find.
            // A more robust implementation would give each possible device a score and pick the one with the highest.
            // A discrete device could for example get a higher score than an embedded one.

            m_physical_device = device;
            break;
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::InitializeQueueFamilyIndices() {
        uint32 queue_family_properties_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_properties_count, nullptr);
        Vector<VkQueueFamilyProperties> queue_family_properties(queue_family_properties_count);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_properties_count, queue_family_properties.data());

        uint32 index = 0;
        for (const VkQueueFamilyProperties &queue_family_property : queue_family_properties) {
            if (queue_family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_queue_family_indices.graphics_family_index = index;
                m_queue_family_indices.has_graphics_family_index = true;
            }

            VkBool32 surface_present_support = false;
            HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, index, m_surface, &surface_present_support));
            if (surface_present_support) {
                m_queue_family_indices.present_family_index = index;
                m_queue_family_indices.has_present_family_index = true;
            }

            if (m_queue_family_indices.has_graphics_family_index && m_queue_family_indices.has_present_family_index) {
                break;
            }

            index++;
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
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanGraphicsContext::DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_pointer) {
        const char *type_string = nullptr;
        switch (type) 	{
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: type_string = "General"; break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: type_string = "Validation"; break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: type_string = "Performance"; break;
            default: type_string = "Unknown"; break;
        }

        if (severity >= g_debug_message_severity) {
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
        }

        return VK_FALSE;
    }

}