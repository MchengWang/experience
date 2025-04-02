#include "Device.h"

namespace WMC {

    ////////////////////////////////////////////
    //////// �������� //////////////////////////
    ////////////////////////////////////////////

    VkSurfaceKHR* surface = nullptr;
    PhysicalDevice* s_PhysicalDevice = nullptr;

    /* �ú����������Ƿ�������֤�㷵���������չ�б� */
    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    /* ����Ƿ����������ͼ�㶼���� */
    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // ����б����Ƿ�������в�
        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    /**
    * �ص�����
    * messageSeverity ָ����Ϣ��������
    * messageType ��Ϣ������
    * pCallbackData �ò������ð�����Ϣ������ϸ��Ϣ�Ľṹ
    * pUserData ���������ûص��ڼ�ָ����ָ�룬���������ݴ��ݸ���
    */
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    /* ��д�ṹ�������й� Messenger ����ص�����ϸ��Ϣ */
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; // ����ָ��ϣ�����ûص����������͵�������
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; // ����ɸѡ֪ͨ�ش�����Ϣ����
        createInfo.pfnUserCallback = debugCallback; // ָ��ص�������ָ��
        createInfo.pUserData = nullptr; // Optional
    }

    /* Ѱ�Һ��ʵĶ��� */
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentSupport);

            if (presentSupport)
            {
                indices.presentFamily = i;
            }

            if (indices.isComplete())
                break;

            i++;
        }

        return indices;
    }

    // ����Ƿ����������չ������
    bool checkDeviceExtensionSupport(VkPhysicalDevice& device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    /* ���ᶨ�����ĺ������� */
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details;

        // ��ѯ Surface ��������
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, s_PhysicalDevice->GetSurface(), &details.capabilities);

        // ��ѯ֧�ֵ� Surface ��ʽ
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, s_PhysicalDevice->GetSurface(), &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, s_PhysicalDevice->GetSurface(), &formatCount, details.formats.data());
        }

        // ��ѯģʽ�Ĺ�����ʽ
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, s_PhysicalDevice->GetSurface(), &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, s_PhysicalDevice->GetSurface(), &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    /* ����������Ƿ��ʺ���Ҫִ�еĲ��� */
    bool isDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = findQueueFamilies(device);
        bool extensionSupported = checkDeviceExtensionSupport(device);

        // ��֤������֧���Ƿ��㹻
        bool swapChainAdequate = false;
        if (extensionSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete() && extensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    /**
    *������ģʽͨ��������,
    *  �����ʽ����ɫ��ȣ�
    *  ��ʾģʽ����ͼ�񡮽���������Ļ��������
    *  ������Χ����������ͼ��ķֱ��ʣ�
    */

    /* Surface ��ʽ */
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    /**
    *��ʾģʽ
    * VK_PRESENT_MODE_IMMEDIATE_KHR��Ӧ�ó����ύ��ͼ����������䵽��Ļ�ϣ����ܻᵼ��˺�ѡ�
    * VK_PRESENT_MODE_FIFO_KHR����������һ�����У�����ʾˢ��ʱ����ʾ���Ӷ��е�ǰ���ȡͼ�񣬳����ڶ��еĺ��������Ⱦ��ͼ��
    * ���������������������ȴ��� �����ִ���Ϸ�еĴ�ֱͬ�������ơ� ˢ����ʾ��ʱ�̳�Ϊ ��vertical blank��
    * VK_PRESENT_MODE_FIFO_RELAXED_KHR������Ӧ�ó����ӳٲ��Ҷ��������һ����ֱ�հ״�Ϊ��ʱ����ģʽ����ǰһ��ģʽ��ͬ��
    * ͼ�����յ���ʱ���������䣬�����ǵȴ���һ����ֱ�հס� ����ܻᵼ�����Ե�˺�ѡ�
    * VK_PRESENT_MODE_MAILBOX_KHR�����ǵڶ���ģʽ����һ�ֱ��塣 ����������ʱ��������ֹӦ�ó��򣬶��Ǽ򵥵ؽ����Ŷӵ�ӳ���滻Ϊ���µ�ӳ��
    * ��ģʽ�����ھ����ܿ����Ⱦ֡��ͬʱ�Ա���˺�ѣ��Ӷ��ȱ�׼��ֱͬ���������ӳ�������١�
    * ��ͨ������Ϊ �����ػ��塱�����ܽ�������������������һ����ζ��֡���ʱ�������
    */
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    /** ������Χ
    * ������Χ�ǽ�����ͼ��ķֱ��ʣ�������������ȫ�����������ڻ��ƵĴ��ڵķֱ��ʣ�������Ϊ��λ��
    * �˴�ʹ�øú���������ʵ��֧�ֵ��������С�����Χ��ֵ��
    */
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(s_PhysicalDevice->GetWindows(), &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    ////////////////////////////////////////////
    /////// PhysicalDevice /////////////////////
    ////////////////////////////////////////////

    PhysicalDevice::PhysicalDevice(GLFWwindow* window)
        : v_Window(window)
    {
        s_PhysicalDevice = this;
    }

	void PhysicalDevice::createInstance()
	{
        if (enableValidationLayers && !checkValidationLayerSupport())
            throw std::runtime_error("validation layers requested, but not available!");

        // ��д�й�����Ӧ�ó����һЩ��Ϣ(��ѡ)
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // ����Ľṹ����� Vulkan ��������Ҫʹ����Щȫ����չ����֤��
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // ȷ��Ҫ���õ�ȫ����֤��
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &v_Instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
	}

    void PhysicalDevice::setupDebugMessenger()
    {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(v_Instance, &createInfo, nullptr, &v_DebugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    void PhysicalDevice::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    void PhysicalDevice::pickPhysicalDevice()
    {
        // �г������Կ�����
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(v_Instance, &deviceCount, nullptr);

        if (deviceCount == 0) // û���豸֧�� Vulkan
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        // ��������֧�� Vulkan �������豸
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(v_Instance, &deviceCount, devices.data());

        // ����Ƿ����κ������豸������ӵ��ù����е�Ҫ��
        for (const auto& device : devices)
        {
            if (isDeviceSuitable(device))
            {
                v_PhysicalDevice = device;
                v_MsaaSamples = getMaxUsableSampleCount();
                break;
            }
        }

        if (v_PhysicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

    }

    void PhysicalDevice::createSurface()
    {
        if (glfwCreateWindowSurface(v_Instance, v_Window, nullptr, &v_Surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }

        surface = &v_Surface;
    }
   

    ////////////////////////////////////////////
    /////// LogicalDevice //////////////////////
    ////////////////////////////////////////////
    
    LogicalDevice::LogicalDevice(PhysicalDevice physicalDevice)
        : physicalDevice(physicalDevice)
    {
        v_PDevice = physicalDevice.GetPhysicalDevice();
    }

    void LogicalDevice::createLogicalDevice()
    {

        // �����߼��豸��Ҫʹ�ýṹָ��һϵ��ϸ��
        QueueFamilyIndices indices = findQueueFamilies(v_PDevice);

        // ��ǰ���õ���������ֻ����Ϊÿ������ϵ�д����������У�ʵ���ϲ�����Ҫ�������
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
           indices.graphicsFamily.value(),
           indices.presentFamily.value()
        };

        // ����Ϊ���з������ȼ���ʹ�� 0.0f �� 1.0f ֮��ĸ�������Ӱ���������ִ�еĵ���
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // ָ����Ҫʹ�õ��豸���ܼ�
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        // �����߼��豸
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        // ������չ ֧�ֽ�����
        // ������Ⱦ��ͼ��Ӹ��豸���ֵ� Windows
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();


        // ͨ���ʵ������ĺ�����ʵ�����߼��豸
        if (vkCreateDevice(v_PDevice, &createInfo, nullptr, &v_LDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        // ����ÿ������ϵ�еĶ��о��
        vkGetDeviceQueue(v_LDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(v_LDevice, indices.presentFamily.value(), 0, &presentQueue);
    }

    void LogicalDevice::createSwapChain() {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice.GetPhysicalDevice());

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        // ���ñȵ�ǰ����֧����Сֵ��һ��
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        // Ӧȷ����ִ���ǣ�����������ͼ����
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = physicalDevice.v_Surface; // ָ��������Ӧ�󶨵��Ǹ�ͼ���

        // ָ��������ӡ�����ϸ��Ϣ
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1; // ָ��ÿ��ͼ������Ĳ��� ����3D��ʱ����Ҫ�޸�
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // ָ���˽�ʹ�ý������е�ͼ����к��ֲ���

        QueueFamilyIndices indices = findQueueFamilies(v_PDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily)
        {
            // ͼ����Կ�������ϵ��ʹ�ã���������ʽ����Ȩת�ơ�
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            // ͼ��һ����һ������ϵ��ӵ�У���������ȷת������Ȩ��Ȼ���������һ������ϵ����ʹ������ ��ѡ����ṩ�������
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ָ���Ƿ�Ӧʹ�� Alpha ͨ���봰��ϵͳ�е��������ڻ��
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        //createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(v_LDevice, &createInfo, nullptr, &v_SwapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(v_LDevice, v_SwapChain, &imageCount, nullptr);
        v_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(v_LDevice, v_SwapChain, &imageCount, v_SwapChainImages.data());

        v_SwapChainImageFormat = surfaceFormat.format;
        v_SwapChainExtent = extent;
    }

    void LogicalDevice::createImageViews()
    {
        v_SwapChainImageViews.resize(v_SwapChainImages.size());

        for (size_t i = 0; i < v_SwapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = v_SwapChainImages[i];

            // ָ��Ӧ��ν���ͼ������
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // ����ָ�� 1D��2D��3D ����������ͼ
            createInfo.format = v_SwapChainImageFormat;

            // ������ɫͨ��
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            // ����ͼ�����;�Լ�Ӧ����ͼ����ĸ�����
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(v_LDevice, &createInfo, nullptr, &v_SwapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }

        }
    }

    void LogicalDevice::cleanupSwapChain(std::vector<VkFramebuffer> swapChianFramebuffers)
    {
        vkDestroyImageView(v_LDevice, v_DepthImageView, nullptr);
        vkDestroyImage(v_LDevice, v_DepthImage, nullptr);
        vkFreeMemory(v_LDevice, v_DepthImageMemory, nullptr);

        vkDestroyImageView(v_LDevice, v_ColorImageView, nullptr);
        vkDestroyImage(v_LDevice, v_ColorImage, nullptr);
        vkFreeMemory(v_LDevice, v_ColorImageMemory, nullptr);

        for (auto framebuffer : swapChianFramebuffers)
        {
            vkDestroyFramebuffer(v_LDevice, framebuffer, nullptr);
        }

        for (auto imageView : v_SwapChainImageViews)
        {
            vkDestroyImageView(v_LDevice, imageView, nullptr);
        }

        vkDestroySwapchainKHR(v_LDevice, v_SwapChain, nullptr);
    }

    VkSampleCountFlagBits PhysicalDevice::getMaxUsableSampleCount() {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(v_PhysicalDevice, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

}