#include <FF_renderer.h>

Uint32 getQueueFamilyIndex(VkPhysicalDevice physicalDevice)
{
    Uint32 QueueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueCount, nullptr);
    std::vector<VkQueueFamilyProperties> QueueFamilyProperties(QueueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueCount, QueueFamilyProperties.data());
    for (int i = 0; i < QueueCount; i++)
    {
        if (QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            return i;
        };
    };
    return 0;
};

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, SDL_Window *window)
{
    if (false)
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
}

void Renderer::Context::init(){

    instance.create();

    if (!SDL_Vulkan_LoadLibrary(nullptr))
    {
        SDL_error("Failed to Load Vulkan library: ");
    };

    if (!SDL_Vulkan_CreateSurface(window, instance.hand, nullptr, &surface))
    {
        SDL_error("Failed to create Vulkan surface: ");
    }

    device.create(this);
    swapChain.create(this);

}

void Renderer::Instance::create()
{
    const char *validationLayers[] = {
        #ifdef DEBUG
                "VK_LAYER_KHRONOS_validation"
        #endif
            };
            Uint32 instanceExtensionCount = 0;
            const char *const *instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtensionCount);

            std::cout << "Number of Vulkan instance extensions: " << instanceExtensionCount << std::endl;

            VkApplicationInfo appInfo = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = "VulkanApplication",
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                .pEngineName = "VulkanEngine",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                .apiVersion = VK_API_VERSION_1_3,
            };
            VkInstanceCreateInfo instanceInfo = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .pApplicationInfo = &appInfo,
                .enabledLayerCount = sizeof(validationLayers) / sizeof(validationLayers[0]),
                .ppEnabledLayerNames = validationLayers,
                .enabledExtensionCount = instanceExtensionCount,
                .ppEnabledExtensionNames = instanceExtensions,
            };

            VkResult result = vkCreateInstance(&instanceInfo, nullptr, &hand);
            if (result != VK_SUCCESS)
            {
                Vlk_error("Failed to create Vulkan instance!");
                Vlk_error(decryptVulkanError(result));
            };
}

void Renderer::Device::create(Context *ctx){

    auto pickPhysicalDevice = [this,&ctx]() -> VkPhysicalDevice
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(ctx ->    instance.hand, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            SDL_error("No Vulkan-supported GPUs found!");
            return VK_NULL_HANDLE;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(ctx -> instance.hand, &deviceCount, devices.data());

        for (const auto &device : devices)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                return device;
            }
        }

        return devices[0];
    };

    phyDevice = pickPhysicalDevice();

    // Queue-Infos vorbereiten
    uint32_t queueFamilyIndex = getQueueFamilyIndex(phyDevice);
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority};

    // Gerät erstellen
    const char *deviceExtensions[] = {"VK_KHR_swapchain"};
    VkDeviceCreateInfo deviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueCreateInfo,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = deviceExtensions};
    VkResult result = vkCreateDevice(phyDevice, &deviceCreateInfo, nullptr, &handle);
    if (result)
    {
        Vlk_error("Failed to create logical device!");
        Vlk_error(decryptVulkanError(result));
    }
    vkGetDeviceQueue(handle, queueFamilyIndex, 0, &queue);
}

void Renderer::SwapChain::create(Context *ctx){

    VkSurfaceCapabilitiesKHR capabilities;
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->device.phyDevice, ctx->surface, &capabilities);
    if (result)
    {
        Vlk_error("Failed to get surface capabilities!");
        Vlk_error(decryptVulkanError(result));
    }
    // Choose the surface format
    Uint32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->device.phyDevice, ctx->surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    if (formatCount != 0)
    {
        vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->device.phyDevice, ctx->surface, &formatCount, surfaceFormats.data());
    }
    VkSurfaceFormatKHR surfaceFormat = surfaceFormats[0];
    for (const auto &Format : surfaceFormats)
    {
        if (Format.format == VK_FORMAT_R8G8B8A8_SRGB && Format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            surfaceFormat = Format;
            break;
        }
    }
    // Choose the present mode
    Uint32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->device.phyDevice, ctx->surface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    if (presentModeCount != 0)
    {
        vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->device.phyDevice, ctx->surface, &presentModeCount, presentModes.data());
    }
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR; // Fallback
    for (const auto &mode : presentModes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            presentMode = mode;
            break;
        }
    }
    // Choose swap extent
    VkExtent2D extent = chooseSwapExtent(capabilities,ctx->window);

    // Clamp minImageCount
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }
    // Fill swapchain create info
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = ctx-> surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        createInfo.preTransform = capabilities.currentTransform;
    }
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    result = vkCreateSwapchainKHR(ctx->device.handle, &createInfo, nullptr, &handle);
    if (result)
    {

        Vlk_error("Failed to create swap chain!");
        Vlk_error(decryptVulkanError(result));
    }
    // Retrieve swapchain images
    vkGetSwapchainImagesKHR(ctx->device.handle, handle, &imageCount, nullptr);
    std::vector<VkImage> __images = {};
    __images.resize(imageCount);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(ctx->device.handle, handle, &imageCount, __images.data());

    for (Uint32 i = 0; i < imageCount; i++) {
        images[i].handle = __images[i];

        // Create a VkImageView for the image
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = __images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = ctx->swapChain.images[i].format; // You need to specify the format
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(ctx->device.handle, &viewInfo, nullptr, &images[i].view) != VK_SUCCESS) {
            // Handle the error
        }

        // You can't directly access extent, format, layout, memoryHandle, and memorySize from VkImage
        // You need to use vkGetImageSubresourceLayout to get the layout and size of the image
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1;
        VkImageSubresourceLayout layout;
        vkGetImageSubresourceLayout(ctx->device.handle, __images[i], &subresourceRange, &layout);
        images[i].memorySize = layout.size;

        // You can't directly access extent from VkImage
        // You need to use vkGetImageSubresourceLayout to get the extent of the image
        VkImageSubresourceRange extentRange = {};
        extentRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        extentRange.baseMipLevel = 0;
        extentRange.levelCount = 1;
        extentRange.baseArrayLayer = 0;
        extentRange.layerCount = 1;

        VkImageSubresourceLayout extentLayout;
        vkGetImageSubresourceLayout(ctx->device.handle, __images[i], &extentRange, &extentLayout);
        images[i].extent.width = extentLayout.size.width;
        images[i].extent.height = extentLayout.size.height;

        // You can't directly access format from VkImage
        // You need to specify the format when creating the VkImageView

        // You can't directly access layout from VkImage
        // You need to use vkGetImageSubresourceLayout to get the layout of the image

        // You can't directly access memoryHandle from VkImage
        // You need to allocate memory for the image and then bind it to the image
    }

    images[-1]..swapChainImageFormat = surfaceFormat.format;
    swapChainInfo.swapChainExtent = extent;
}












































