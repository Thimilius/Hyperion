//------------------------ Header Guard ------------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/vulkan/vulkan_render_context.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/vulkan/vulkan_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  constexpr bool8 LOG_LAYERS = false;
  constexpr bool8 LOG_EXTENSIONS = false;

  //--------------------------------------------------------------
  void VulkanRenderContext::Initialize(Window *main_window, const RenderContextDescriptor &descriptor) {
    QueryLayersAndExtensions();
    CreateInstance();
#ifdef HYP_DEBUG
    RegisterDebugMessageCallback();
#endif
    PickPhysicalDevice();

    m_render_driver.Setup(this);

    HYP_LOG_INFO("Vulkan", "Initialized Vulkan graphics driver!");
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::Shutdown() {
#ifdef HYP_DEBUG
    auto debug_messenger_destroy_function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(LoadFunction("vkDestroyDebugUtilsMessengerEXT"));
    debug_messenger_destroy_function(m_instance, m_debug_messenger, nullptr);
#endif

    vkDestroyInstance(m_instance, nullptr);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::QueryLayersAndExtensions() {
    uint32 layer_count = 0;
    HYP_VULKAN_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, nullptr), "Failed to get instance layer count!");
    m_instance_layers.Resize(layer_count);
    HYP_VULKAN_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, m_instance_layers.GetData()), "Failed to get instance layers!");

    if (LOG_LAYERS) {
      for (auto &layer : m_instance_layers) {
        HYP_LOG_INFO("Vulkan", "Vulkan layer supported: {}", layer.layerName);
      }
    }

    uint32 extension_count = 0;
    HYP_VULKAN_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr), "Failed to get instance extension count!");
    m_instance_extensions.Resize(extension_count);
    HYP_VULKAN_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, m_instance_extensions.GetData()), "Failed to get instance extensions!");

    if (LOG_EXTENSIONS) {
      for (auto &extension : m_instance_extensions) {
        HYP_LOG_INFO("Vulkan", "Vulkan extension supported: {}", extension.extensionName);
      }
    }
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateInstance() {
    VkApplicationInfo application_info = { };
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName = "Hyperion"; // TODO: Fill in with proper name.
    application_info.pEngineName = "Hyperion";
    application_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    application_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    application_info.apiVersion = VK_API_VERSION_1_2;

    Array<const char *> layers = GetAndValidateInstanceLayer();
    Array<const char *> extensions = GetAndValidateInstanceExtensions();

    VkInstanceCreateInfo instance_create_info = { };
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;
    instance_create_info.ppEnabledLayerNames = layers.GetData();
    instance_create_info.enabledLayerCount = static_cast<uint32>(layers.GetLength());
    instance_create_info.ppEnabledExtensionNames = extensions.GetData();
    instance_create_info.enabledExtensionCount = static_cast<uint32>(extensions.GetLength());

    HYP_VULKAN_CHECK(vkCreateInstance(&instance_create_info, nullptr, &m_instance), "Failed to create vulkan instance!");
  }

#ifdef HYP_DEBUG
  //--------------------------------------------------------------
  void VulkanRenderContext::RegisterDebugMessageCallback() {
    VkDebugUtilsMessengerCreateInfoEXT messenger_create_info = { };
    messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messenger_create_info.pfnUserCallback = DebugMessageCallback;
    messenger_create_info.pUserData = nullptr;

    auto creation_function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(LoadFunction("vkCreateDebugUtilsMessengerEXT"));
    HYP_VULKAN_CHECK(creation_function(m_instance, &messenger_create_info, nullptr, &m_debug_messenger), "Failed to create debug messenger!");
  }
#endif

  //--------------------------------------------------------------
  void VulkanRenderContext::PickPhysicalDevice() {
    uint32 device_count = 0;
    HYP_VULKAN_CHECK(vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr), "Failed to get physical device count!");
    if (device_count == 0) {
      HYP_PANIC_MESSAGE("Vulkan", "Failed to find physical device with Vulkan support!");
    }

    Array<VkPhysicalDevice> physical_devices;
    physical_devices.Resize(device_count);
    HYP_VULKAN_CHECK(vkEnumeratePhysicalDevices(m_instance, &device_count, physical_devices.GetData()), "Failed to get physical devices!");

    struct QueueIndices {
      int32 i;
    };

    for (auto device : physical_devices) {
      if (IsDeviceSuitable(device)) {
        m_physical_device = device;
        break;
      }
    }
    if (m_physical_device == VK_NULL_HANDLE) {
      HYP_PANIC_MESSAGE("Vulkan", "Failed to find suitable physical device!");
    }
  }

  //--------------------------------------------------------------
  Array<const char *> VulkanRenderContext::GetAndValidateInstanceLayer() {
    Array<const char *> layers;

#ifdef HYP_DEBUG
    layers.Add("VK_LAYER_KHRONOS_validation");
#endif

    for (auto layer : layers) {
      if (!SupportsInstanceLayer(layer)) {
        HYP_LOG_ERROR("Vulkan", "Missing required instance layer: {}!", layer);
      }
    }

    return layers;
  }

  //--------------------------------------------------------------
  bool8 VulkanRenderContext::SupportsInstanceLayer(const char *layer_name) {
    for (auto &layer : m_instance_layers) {
      if (std::strcmp(layer.layerName, layer_name) == 0) {
        return true;
      }
    }
    return false;
  }

  //--------------------------------------------------------------
  Array<const char *> VulkanRenderContext::GetAndValidateInstanceExtensions() {
    Array<const char *> extensions;

#ifdef HYP_DEBUG
    extensions.Add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    for (auto extension : extensions) {
      if (!SupportsInstanceExtension(extension)) {
        HYP_LOG_ERROR("Vulkan", "Missing required instance extension: {}!", extension);
      }
    }

    return extensions;
  }

  //--------------------------------------------------------------
  bool8 VulkanRenderContext::SupportsInstanceExtension(const char *extension_name) {
    for (auto &extension : m_instance_extensions) {
      if (std::strcmp(extension.extensionName, extension_name) == 0) {
        return true;
      }
    }
    return false;
  }

  //--------------------------------------------------------------
  void *VulkanRenderContext::LoadFunction(const char *name) {
    void *function = vkGetInstanceProcAddr(m_instance, name);
    if (function == nullptr) {
      HYP_LOG_ERROR("Vulkan", "Failed to load function with name: {}", name);
      return nullptr;
    } else {
      return function;
    }
  }

  //--------------------------------------------------------------
  bool8 VulkanRenderContext::IsDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    QueueFamilyIndices indices = FindQueueFamilyIndices(device);
    return indices.IsValid();
  }

  //--------------------------------------------------------------
  VulkanRenderContext::QueueFamilyIndices VulkanRenderContext::FindQueueFamilyIndices(VkPhysicalDevice device) {
    QueueFamilyIndices indices = { };
    indices.graphics_family = -1;

    uint32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    Array<VkQueueFamilyProperties> queue_families;
    queue_families.Resize(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.GetData());

    for (int32 i = 0; i < static_cast<int32>(queue_family_count); i++) {
      VkQueueFamilyProperties &queue_family = queue_families[i];
      if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphics_family = i;
      }
    }

    return indices;
  }

  //--------------------------------------------------------------
  VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderContext::DebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data) {

    if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      HYP_LOG_ERROR("Vulkan", "Validation: {}", callback_data->pMessage);
      HYP_DEBUG_BREAK;
    } else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      HYP_LOG_WARN("Vulkan", "Validation: {}", callback_data->pMessage);
    }

    return VK_FALSE;
  }

}