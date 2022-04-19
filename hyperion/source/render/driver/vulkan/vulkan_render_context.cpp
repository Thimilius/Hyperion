//------------------------ Header Guard ------------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/vulkan/vulkan_render_context.hpp"

//---------------------- Library Includes ----------------------
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/render/driver/vulkan/vulkan_shader_compiler.hpp"
#include "hyperion/render/driver/vulkan/vulkan_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  constexpr bool8 LOG_LAYERS = false;
  constexpr bool8 LOG_EXTENSIONS = false;

  struct VulkanVertex {
    Vector2 position;
    Vector3 color;

    static VkVertexInputBindingDescription GetBindingDescription() {
      VkVertexInputBindingDescription binding_description = { };
      binding_description.binding = 0;
      binding_description.stride = sizeof(VulkanVertex);
      binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
      return binding_description;
    }

    static Array<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
      Array<VkVertexInputAttributeDescription> attribute_descriptions = { };
      attribute_descriptions.Resize(2);

      attribute_descriptions[0].binding = 0;
      attribute_descriptions[0].location = 0;
      attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
      attribute_descriptions[0].offset = offsetof(VulkanVertex, position);

      attribute_descriptions[1].binding = 0;
      attribute_descriptions[1].location = 1;
      attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attribute_descriptions[1].offset = offsetof(VulkanVertex, color);

      return attribute_descriptions;
    }
  };

  const Array<VulkanVertex> g_vertices = {
    { {-0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f } },
    { { 0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f } },
    { { 0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f } },
    { {-0.5f,  0.5f}, { 1.0f, 1.0f, 1.0f } },
  };

  const Array<uint16> g_indices = {
    0, 1, 2,
    0, 2, 3,
  };

  //--------------------------------------------------------------
  void VulkanRenderContext::Initialize(Window *main_window, const RenderContextDescriptor &descriptor) {
    QueryLayersAndExtensions();
    CreateInstance();
#ifdef HYP_DEBUG
    RegisterDebugMessageCallback();
#endif
    if (CreateSurface(main_window, m_instance, &m_surface) != VK_SUCCESS) {
      HYP_PANIC_MESSAGE("Vulkan", "Failed to create surface!");
    }
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFramebuffers();
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSyncObjects();
    CreateAllocator();
    CreateVertexBuffer();
    CreateIndexBuffer();

    m_render_driver.Setup(this);

    HYP_LOG_INFO("Vulkan", "Initialized Vulkan graphics driver!");
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::Shutdown() {
    CleanupSwapchain();
    Cleanup();
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::SwapBuffers(Window *window) { }

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

#ifdef HYP_DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = { };
    FillDebugMessengerCreateInfoStruct(&debug_messenger_create_info);
    instance_create_info.pNext = &debug_messenger_create_info;
#endif

    HYP_VULKAN_CHECK(vkCreateInstance(&instance_create_info, nullptr, &m_instance), "Failed to create vulkan instance!");
  }

#ifdef HYP_DEBUG
  //--------------------------------------------------------------
  void VulkanRenderContext::RegisterDebugMessageCallback() {
    VkDebugUtilsMessengerCreateInfoEXT messenger_create_info = { };
    FillDebugMessengerCreateInfoStruct(&messenger_create_info);

    auto creation_function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(LoadFunction("vkCreateDebugUtilsMessengerEXT"));
    HYP_VULKAN_CHECK(creation_function(m_instance, &messenger_create_info, nullptr, &m_debug_messenger), "Failed to create debug messenger!");
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::FillDebugMessengerCreateInfoStruct(VkDebugUtilsMessengerCreateInfoEXT *messenger_create_info) {
    messenger_create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messenger_create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messenger_create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messenger_create_info->pfnUserCallback = DebugMessageCallback;
    messenger_create_info->pUserData = nullptr;
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
    Array<const char *> required_extensions = GetRequiredInstanceExtensions();

    for (auto extension : required_extensions) {
      if (!SupportsInstanceExtension(extension)) {
        HYP_LOG_ERROR("Vulkan", "Missing required instance extension: {}!", extension);
      }
    }

    return required_extensions;
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
    VulkanQueueFamilyIndices indices = FindQueueFamilyIndices(device);
    bool8 extensions_supported = CheckDeviceExtensionSupport(device);

    bool8 swap_chain_adequate = false;
    if (extensions_supported) {
      VulkanSwapchainSupportDetails swap_chain_support_details = QuerySwapchainSupportDetails(device);
      swap_chain_adequate = !swap_chain_support_details.formats.IsEmpty() && !swap_chain_support_details.present_modes.IsEmpty();
    }

    return indices.IsValid() && extensions_supported && swap_chain_adequate;
  }

  //--------------------------------------------------------------
  Array<const char *> VulkanRenderContext::GetRequiredInstanceExtensions() const {
    Array<const char *> instance_extensions;

    instance_extensions.Add(VK_KHR_SURFACE_EXTENSION_NAME);

#ifdef HYP_DEBUG
    instance_extensions.Add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    
    return instance_extensions;
  }

  //--------------------------------------------------------------
  Array<const char *> VulkanRenderContext::GetRequiredDeviceExtensions() const {
    Array<const char *> device_extensions;
    device_extensions.Add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    return device_extensions;
  }

  //--------------------------------------------------------------
  VulkanQueueFamilyIndices VulkanRenderContext::FindQueueFamilyIndices(VkPhysicalDevice device) {
    VulkanQueueFamilyIndices indices = { };
    indices.graphics_family = -1;
    indices.presentation_family = -1;

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

      VkBool32 presentation_supported = false;
      HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentation_supported), "Failed to get device surface support!");
      if (presentation_supported) {
        indices.presentation_family = i;
      }
    }

    return indices;
  }

  //--------------------------------------------------------------
  bool8 VulkanRenderContext::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32 device_extension_count = 0;
    HYP_VULKAN_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &device_extension_count, nullptr), "Failed to get device extension count!");

    Array<VkExtensionProperties> available_extensions;
    available_extensions.Resize(device_extension_count);
    HYP_VULKAN_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &device_extension_count, available_extensions.GetData()),
                     "Failed to get device extensions!");

    Array<const char *> required_extensions = GetRequiredDeviceExtensions();

    bool8 supports_extensions = true;
    for (const char *required_extension : required_extensions) {
      bool8 found_extension = false;
      for (VkExtensionProperties &available_extension : available_extensions) {
        if (std::strcmp(required_extension, available_extension.extensionName) == 0) {
          found_extension = true;
          break;
        }
      }
      if (!found_extension) {
        supports_extensions = false;
        break;
      }
    }
    return supports_extensions;
  }

  //--------------------------------------------------------------
  VulkanSwapchainSupportDetails VulkanRenderContext::QuerySwapchainSupportDetails(VkPhysicalDevice device) {
    VulkanSwapchainSupportDetails details;

    HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities), "Failed to get surface capabilities!");

    uint32 device_surface_format_count = 0;
    HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &device_surface_format_count, nullptr), "Failed to get device format count!");
    if (device_surface_format_count != 0) {
      details.formats.Resize(device_surface_format_count);
      HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &device_surface_format_count, details.formats.GetData()),
                       "Failed to get device formats!");
    }

    uint32 device_surface_present_mode_count = 0;
    HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &device_surface_present_mode_count, nullptr),
                     "Failed to get device present mode count!");
    if (device_surface_present_mode_count != 0) {
      details.present_modes.Resize(device_surface_present_mode_count);
      HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &device_surface_present_mode_count, details.present_modes.GetData()),
                       "Failed to get device present modes!");
    }

    return details;
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateLogicalDevice() {
    VulkanQueueFamilyIndices indices = FindQueueFamilyIndices(m_physical_device);
    Set<int32> unique_queue_indices = { indices.graphics_family, indices.presentation_family };
    Array<VkDeviceQueueCreateInfo> queue_create_infos;
    queue_create_infos.Reserve(unique_queue_indices.GetLength());

    float32 queue_priority = 1.0f;
    for (int32 queue_family : unique_queue_indices) {
      VkDeviceQueueCreateInfo queue_create_info = { };
      queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_create_info.queueFamilyIndex = static_cast<uint32>(queue_family);
      queue_create_info.queueCount = 1;
      queue_create_info.pQueuePriorities = &queue_priority;
      queue_create_infos.Add(queue_create_info);
    }

    VkPhysicalDeviceFeatures physical_device_features = { };

    Array<const char *> required_device_extensions = GetRequiredDeviceExtensions();

    VkDeviceCreateInfo device_create_info = { };
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = queue_create_infos.GetData();
    device_create_info.queueCreateInfoCount = static_cast<uint32>(queue_create_infos.GetLength());
    device_create_info.pEnabledFeatures = &physical_device_features;
    device_create_info.enabledExtensionCount = static_cast<uint32>(required_device_extensions.GetLength());
    device_create_info.ppEnabledExtensionNames = required_device_extensions.GetData();

    HYP_VULKAN_CHECK(vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device), "Failed to create logical device!");

    vkGetDeviceQueue(m_device, indices.graphics_family, 0, &m_graphics_queue);
    vkGetDeviceQueue(m_device, indices.presentation_family, 0, &m_presentation_queue);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateSwapChain() {
    VulkanSwapchainSupportDetails swapchain_support_details = QuerySwapchainSupportDetails(m_physical_device);

    VkSurfaceFormatKHR surface_format = ChooseSwapChainFormat(swapchain_support_details.formats);
    VkPresentModeKHR present_mode = ChoosePresentMode(swapchain_support_details.present_modes);
    VkExtent2D extent = ChooseSwapExtent(swapchain_support_details.capabilities);

    uint32 image_count = swapchain_support_details.capabilities.minImageCount + 1;
    if (swapchain_support_details.capabilities.maxImageCount > 0 && image_count > swapchain_support_details.capabilities.maxImageCount) {
      image_count = swapchain_support_details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = { };
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = m_surface;
    create_info.minImageCount = image_count;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageFormat = surface_format.format;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VulkanQueueFamilyIndices indices = FindQueueFamilyIndices(m_physical_device);
    uint32 queue_family_indices[] = { static_cast<uint32>(indices.graphics_family), static_cast<uint32>(indices.presentation_family) };
    if (indices.graphics_family != indices.presentation_family) {
      create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      create_info.queueFamilyIndexCount = 2;
      create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
      create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      create_info.queueFamilyIndexCount = 0;
      create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = swapchain_support_details.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    HYP_VULKAN_CHECK(vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain), "Failed to create swapchain!");

    m_swapchain_image_format = surface_format.format;
    m_swapchain_extent = extent;

    uint32 swapchain_image_count = 0;
    HYP_VULKAN_CHECK(vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchain_image_count, nullptr), "Failed to get swapchain image count!");
    m_swapchain_images.Resize(swapchain_image_count);
    HYP_VULKAN_CHECK(vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchain_image_count, m_swapchain_images.GetData()), "Failed to get swapchain images!");
  }

  //--------------------------------------------------------------
  VkSurfaceFormatKHR VulkanRenderContext::ChooseSwapChainFormat(const Array<VkSurfaceFormatKHR> formats) {
    for (const VkSurfaceFormatKHR &format : formats) {
      if (format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return format;
      }
    }
    return formats.GetFirst();
  }

  //--------------------------------------------------------------
  VkPresentModeKHR VulkanRenderContext::ChoosePresentMode(const Array<VkPresentModeKHR> present_modes) {
    for (const VkPresentModeKHR present_mode : present_modes) {
      if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return present_mode;
      }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
  }

  //--------------------------------------------------------------
  VkExtent2D VulkanRenderContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX && capabilities.currentExtent.width != 0) {
      return capabilities.currentExtent;
    } else {
      VkExtent2D extent = {
        Display::GetWidth(),
        Display::GetHeight()
      };

      if (capabilities.currentExtent.width != 0) {
        extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
      }

      return extent;
    }
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateImageViews() {
    m_swapchain_image_views.Resize(m_swapchain_images.GetLength());
    for (uint32 i = 0; i < m_swapchain_images.GetLength(); i++) {
      VkImageViewCreateInfo create_info = { };
      create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      create_info.image = m_swapchain_images[i];
      create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
      create_info.format = m_swapchain_image_format;
      create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      create_info.subresourceRange.baseMipLevel = 0;
      create_info.subresourceRange.levelCount = 1;
      create_info.subresourceRange.baseArrayLayer = 0;
      create_info.subresourceRange.layerCount = 1;

      HYP_VULKAN_CHECK(vkCreateImageView(m_device, &create_info, nullptr, &m_swapchain_image_views[i]), "Failed to create swapchain image view!");
    }
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateRenderPass() {
    VkAttachmentDescription color_attachment = { };
    color_attachment.format = m_swapchain_image_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_reference = { };
    color_attachment_reference.attachment = 0;
    color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = { };
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_reference;

    VkSubpassDependency subpass_dependency = { };
    subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpass_dependency.dstSubpass = 0;
    subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.srcAccessMask = 0;
    subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo create_info = { };
    create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount = 1;
    create_info.pAttachments = &color_attachment;
    create_info.subpassCount = 1;
    create_info.pSubpasses = &subpass;
    create_info.dependencyCount = 1;
    create_info.pDependencies = &subpass_dependency;

    HYP_VULKAN_CHECK(vkCreateRenderPass(m_device, &create_info, nullptr, &m_render_pass), "Failed to create render pass!");
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateGraphicsPipeline() {
    VkShaderModule vertex_shader_module = VulkanShaderCompiler::CompileModule(m_device, "data/shaders/vulkan/shader.vert", VK_SHADER_STAGE_VERTEX_BIT);
    VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info = { };
    vertex_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertex_shader_stage_create_info.module = vertex_shader_module;
    vertex_shader_stage_create_info.pName = "main";

    VkShaderModule fragment_shader_module = VulkanShaderCompiler::CompileModule(m_device, "data/shaders/vulkan/shader.frag", VK_SHADER_STAGE_FRAGMENT_BIT);
    VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info = { };
    fragment_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragment_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragment_shader_stage_create_info.module = fragment_shader_module;
    fragment_shader_stage_create_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_shader_stage_create_info, fragment_shader_stage_create_info };

    VkVertexInputBindingDescription binding_description = VulkanVertex::GetBindingDescription();
    Array<VkVertexInputAttributeDescription> attribute_descriptions = VulkanVertex::GetAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertex_input_create_info = { };
    vertex_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_create_info.vertexBindingDescriptionCount = 1;
    vertex_input_create_info.pVertexBindingDescriptions = &binding_description;
    vertex_input_create_info.vertexAttributeDescriptionCount = static_cast<uint32>(attribute_descriptions.GetLength());
    vertex_input_create_info.pVertexAttributeDescriptions = attribute_descriptions.GetData();

    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info = { };
    input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = { };
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float32>(m_swapchain_extent.width);
    viewport.height = static_cast<float32>(m_swapchain_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = { };
    scissor.offset = { 0, 0 };
    scissor.extent = m_swapchain_extent;

    VkPipelineViewportStateCreateInfo viewport_state_create_info = { };
    viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.viewportCount = 1;
    viewport_state_create_info.pViewports = &viewport;
    viewport_state_create_info.scissorCount = 1;
    viewport_state_create_info.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = { };
    rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state_create_info.depthClampEnable = VK_FALSE;
    rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
    rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization_state_create_info.lineWidth = 1.0f;
    rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterization_state_create_info.depthBiasEnable = VK_FALSE;
    rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
    rasterization_state_create_info.depthBiasClamp = 0.0f;
    rasterization_state_create_info.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisample_create_info = { };
    multisample_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_create_info.sampleShadingEnable = VK_FALSE;
    multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample_create_info.minSampleShading = 1.0f;
    multisample_create_info.pSampleMask = nullptr;
    multisample_create_info.alphaToCoverageEnable = VK_FALSE;
    multisample_create_info.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState color_blend_attachment_state = { };
    color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment_state.blendEnable = VK_FALSE;
    color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = { };
    color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state_create_info.logicOpEnable = VK_FALSE;
    color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
    color_blend_state_create_info.attachmentCount = 1;
    color_blend_state_create_info.pAttachments = &color_blend_attachment_state;
    color_blend_state_create_info.blendConstants[0] = 0.0f;
    color_blend_state_create_info.blendConstants[1] = 0.0f;
    color_blend_state_create_info.blendConstants[2] = 0.0f;
    color_blend_state_create_info.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo layout_create_info = { };
    layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_create_info.setLayoutCount = 0;
    layout_create_info.pSetLayouts = nullptr;
    layout_create_info.pushConstantRangeCount = 0;
    layout_create_info.pPushConstantRanges = nullptr;

    HYP_VULKAN_CHECK(vkCreatePipelineLayout(m_device, &layout_create_info, nullptr, &m_pipeline_layout), "Failed to create pipeline layout!");

    VkDynamicState dynamic_states[] = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = { };
    dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount = 2;
    dynamic_state_create_info.pDynamicStates = dynamic_states;

    VkGraphicsPipelineCreateInfo create_info = { };
    create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    create_info.stageCount = 2;
    create_info.pStages = shader_stages;
    create_info.pVertexInputState = &vertex_input_create_info;
    create_info.pInputAssemblyState = &input_assembly_create_info;
    create_info.pViewportState = &viewport_state_create_info;
    create_info.pRasterizationState = &rasterization_state_create_info;
    create_info.pMultisampleState = &multisample_create_info;
    create_info.pDepthStencilState = nullptr;
    create_info.pColorBlendState = &color_blend_state_create_info;
    create_info.pDynamicState = &dynamic_state_create_info;
    create_info.layout = m_pipeline_layout;
    create_info.renderPass = m_render_pass;
    create_info.subpass = 0;
    create_info.basePipelineHandle = VK_NULL_HANDLE;
    create_info.basePipelineIndex = -1;

    HYP_VULKAN_CHECK(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &create_info, nullptr, &m_graphics_pipeline),
                     "Failed to create graphics pipeline!");

    vkDestroyShaderModule(m_device, fragment_shader_module, nullptr);
    vkDestroyShaderModule(m_device, vertex_shader_module, nullptr);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateFramebuffers() {
    m_swapchain_framebuffers.Resize(m_swapchain_image_views.GetLength());

    for (uint32 i = 0; i < m_swapchain_image_views.GetLength(); i++) {
      VkImageView attachments[] = {
        m_swapchain_image_views[i]
      };

      VkFramebufferCreateInfo create_info = { };
      create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      create_info.renderPass = m_render_pass;
      create_info.attachmentCount = 1;
      create_info.pAttachments = attachments;
      create_info.width = m_swapchain_extent.width;
      create_info.height = m_swapchain_extent.height;
      create_info.layers = 1;

      HYP_VULKAN_CHECK(vkCreateFramebuffer(m_device, &create_info, nullptr, &m_swapchain_framebuffers[i]), "Failed to create framebuffer!");
    }
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateCommandPool() {
    VulkanQueueFamilyIndices indices = FindQueueFamilyIndices(m_physical_device);

    VkCommandPoolCreateInfo create_info = { };
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    create_info.queueFamilyIndex = indices.graphics_family;

    HYP_VULKAN_CHECK(vkCreateCommandPool(m_device, &create_info, nullptr, &m_command_pool), "Failed to create command pool!");
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateCommandBuffers() {
    m_command_buffers.Resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocate_info = { };
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool = m_command_pool;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = static_cast<uint32>(m_command_buffers.GetLength());

    HYP_VULKAN_CHECK(vkAllocateCommandBuffers(m_device, &allocate_info, m_command_buffers.GetData()), "Failed to allocate command buffers!");
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateSyncObjects() {
    m_image_available_semaphores.Resize(MAX_FRAMES_IN_FLIGHT);
    m_render_finished_semaphores.Resize(MAX_FRAMES_IN_FLIGHT);
    m_in_flight_fences.Resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphore_create_info = { };
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_create_info = { };
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      HYP_VULKAN_CHECK(vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_image_available_semaphores[i]), "Failed to create semaphore!");
      HYP_VULKAN_CHECK(vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_render_finished_semaphores[i]), "Failed to create semaphore!");
      HYP_VULKAN_CHECK(vkCreateFence(m_device, &fence_create_info, nullptr, &m_in_flight_fences[i]), "Failed to create fence!");
    }
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateAllocator() {
    VmaAllocatorCreateInfo create_info = { };
    create_info.flags = 0;
    create_info.physicalDevice = m_physical_device;
    create_info.device = m_device;
    create_info.pHeapSizeLimit = 0;
    create_info.instance = m_instance;

    vmaCreateAllocator(&create_info, &m_allocator);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateVertexBuffer() {
    VkDeviceSize buffer_size = g_vertices.GetLength() * sizeof(g_vertices[0]);

    VkBuffer staging_buffer;
    VmaAllocation staging_buffer_memory;
    CreateBuffer(
      buffer_size,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VMA_MEMORY_USAGE_CPU_ONLY,
      staging_buffer,
      staging_buffer_memory
    );
    void *memory;
    HYP_VULKAN_CHECK(vmaMapMemory(m_allocator, staging_buffer_memory, &memory), "Failed to map vertex buffer memory!");
    std::memcpy(memory, g_vertices.GetData(), buffer_size);
    vmaUnmapMemory(m_allocator, staging_buffer_memory);

    CreateBuffer(
      buffer_size,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VMA_MEMORY_USAGE_GPU_ONLY,
      m_vertex_buffer,
      m_vertex_buffer_memory
    );
    CopyBuffer(staging_buffer, m_vertex_buffer, buffer_size);

    vmaDestroyBuffer(m_allocator, staging_buffer, staging_buffer_memory);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateIndexBuffer() {
    VkDeviceSize buffer_size = g_indices.GetLength() * sizeof(g_indices[0]);

    VkBuffer staging_buffer;
    VmaAllocation staging_buffer_memory;
    CreateBuffer(
      buffer_size,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VMA_MEMORY_USAGE_CPU_ONLY,
      staging_buffer,
      staging_buffer_memory
    );
    void *memory;
    HYP_VULKAN_CHECK(vmaMapMemory(m_allocator, staging_buffer_memory, &memory), "Failed to map index buffer memory!");
    std::memcpy(memory, g_indices.GetData(), buffer_size);
    vmaUnmapMemory(m_allocator, staging_buffer_memory);

    CreateBuffer(
      buffer_size,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VMA_MEMORY_USAGE_GPU_ONLY,
      m_index_buffer,
      m_index_buffer_memory
    );
    CopyBuffer(staging_buffer, m_index_buffer, buffer_size);

    vmaDestroyBuffer(m_allocator, staging_buffer, staging_buffer_memory);
  }

  //--------------------------------------------------------------
  uint32 VulkanRenderContext::FindMemoryType(uint32 type_filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(m_physical_device, &memory_properties);

    for (uint32 i = 0; i < memory_properties.memoryTypeCount; i++) {
      if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
        return i;
      }
    }

    HYP_LOG_ERROR("Vulkan", "Failed to find suitable memory type!");
    return 0;
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage, VkBuffer &buffer, VmaAllocation &buffer_memory) {
    VkBufferCreateInfo create_info = { };
    create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    create_info.size = size;
    create_info.usage = usage;
    create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocation_create_info = { };
    allocation_create_info.usage = memory_usage;

    HYP_VULKAN_CHECK(vmaCreateBuffer(m_allocator, &create_info, &allocation_create_info, &buffer, &buffer_memory, nullptr), "Failed to create buffer!");
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CopyBuffer(VkBuffer source_buffer, VkBuffer destination_buffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocate_info = { };
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandPool = m_command_pool;
    allocate_info.commandBufferCount = 1;
    VkCommandBuffer command_buffer;
    HYP_VULKAN_CHECK(vkAllocateCommandBuffers(m_device, &allocate_info, &command_buffer), "Failed to allocate command buffer!");

    VkCommandBufferBeginInfo begin_info = { };
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    HYP_VULKAN_CHECK(vkBeginCommandBuffer(command_buffer, &begin_info), "Failed to begin command buffer!");
    VkBufferCopy copy_region = { };
    copy_region.srcOffset = 0;
    copy_region.dstOffset = 0;
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer, source_buffer, destination_buffer, 1, &copy_region);
    HYP_VULKAN_CHECK(vkEndCommandBuffer(command_buffer), "Failed to end command buffer!");

    VkSubmitInfo submit_info = { };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    HYP_VULKAN_CHECK(vkQueueSubmit(m_graphics_queue, 1, &submit_info, VK_NULL_HANDLE), "Failed to submit command buffer!");
    HYP_VULKAN_CHECK(vkQueueWaitIdle(m_graphics_queue), "Failed to wait for device!");

    vkFreeCommandBuffers(m_device, m_command_pool, 1, &command_buffer);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::RecreateSwapchain() {
    vkDeviceWaitIdle(m_device);

    CleanupSwapchain();

    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateFramebuffers();

    vkDeviceWaitIdle(m_device);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::CleanupSwapchain() {
    for (auto framebuffer : m_swapchain_framebuffers) {
      vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }

    for (auto swapchain_image_view : m_swapchain_image_views) {
      vkDestroyImageView(m_device, swapchain_image_view, nullptr);
    }
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

    vkDestroyRenderPass(m_device, m_render_pass, nullptr);
  }

  //--------------------------------------------------------------
  void VulkanRenderContext::Cleanup() {
    vmaDestroyBuffer(m_allocator, m_vertex_buffer, m_vertex_buffer_memory);
    vmaDestroyBuffer(m_allocator, m_index_buffer, m_index_buffer_memory);
    vmaDestroyAllocator(m_allocator);

    vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);

    for (uint32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(m_device, m_image_available_semaphores[i], nullptr);
      vkDestroySemaphore(m_device, m_render_finished_semaphores[i], nullptr);
      vkDestroyFence(m_device, m_in_flight_fences[i], nullptr);
    }

    vkDestroyCommandPool(m_device, m_command_pool, nullptr);

    vkDestroyDevice(m_device, nullptr);

#ifdef HYP_DEBUG
    auto debug_messenger_destroy_function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(LoadFunction("vkDestroyDebugUtilsMessengerEXT"));
    debug_messenger_destroy_function(m_instance, m_debug_messenger, nullptr);
#endif

    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);
  }

  //--------------------------------------------------------------
  VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderContext::DebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data) {

    if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      // HACK: This message is triggered when minimizing the window and the size becomes (0, 0). We are just ignoring this case for now...
      if (String(callback_data->pMessage).contains("VUID-VkSwapchainCreateInfoKHR-imageExtent-01274")) {
        return VK_FALSE;
      }

      HYP_LOG_ERROR("Vulkan", "Validation: {}", callback_data->pMessage);
      HYP_DEBUG_BREAK;
    } else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      HYP_LOG_WARN("Vulkan", "Validation: {}", callback_data->pMessage);
    }

    return VK_FALSE;
  }

}
