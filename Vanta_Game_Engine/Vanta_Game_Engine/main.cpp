#include "GLFWWindow.h"

#include "VulkanInstanceProvider.h"
#include "VulkanSurfaceProvider.h"
#include "VulkanPhysicalDeviceProvider.h"
#include <iostream>


int main()
{
    // ============================================================
    // 1. Create GLFW Window
    // ============================================================

    GLFWWindow window(
        5000,
        5000,
        "Vanta Vulkan Engine"
    );


    if (!window.Initialization())
    {
        std::cerr
            << "Failed to initialize GLFW window.\n";

        return -1;
    }


    // ============================================================
    // 2. Create Vulkan Platform Provider
    // ============================================================

    VulkanSurfaceProvider surfaceProvider;


    // ============================================================
    // 3. Ask GLFW/Vulkan bridge for required extensions
    // ============================================================

    std::vector<const char*> requiredExtensions =
        surfaceProvider.GetRequiredInstancesExtension();


    if (requiredExtensions.empty())
    {
        std::cerr
            << "Failed to get required Vulkan "
            << "instance extensions.\n";

        return -1;
    }


    // ============================================================
    // 4. Create Vulkan Instance
    // ============================================================

    VulkanInstanceProvider instanceProvider(
        "Vanta Vulkan Engine"
    );


    if (!instanceProvider.Initialization(
        requiredExtensions))
    {
        std::cerr
            << "Failed to initialize Vulkan instance.\n";

        return -1;
    }


    // ============================================================
    // 5. Create Vulkan Surface
    // ============================================================

    VkSurfaceKHR surface =
        VK_NULL_HANDLE;


    if (!surfaceProvider.CreateSurface(
        instanceProvider.GetVulkanInstance(),
        window.getHandler(),
        surface))
    {
        std::cerr
            << "Failed to create Vulkan surface.\n";

        return -1;
    }
    VulkanPhysicalDeviceProvider physicalDeviceProvider(
        instanceProvider.GetVulkanInstance(),
        surface
    );

    if (!physicalDeviceProvider.Initialize())
    {
        return -1;
    }
   
    VkPhysicalDeviceProperties properties =
        physicalDeviceProvider.getPhysicalDevicePropertices();

    std::cout
        << "GPU: "
        << properties.deviceName
        << '\n';
    QueueFamilyIndices queueFamilies =
        physicalDeviceProvider.GetQueueFamiliesIndices();


    if (queueFamilies.graphicFamily.has_value())
    {
        std::cout
            << "Graphics Queue Family: "
            << queueFamilies.graphicFamily.value()
            << '\n';
    }


    if (queueFamilies.presentFamily.has_value())
    {
        std::cout
            << "Present Queue Family: "
            << queueFamilies.presentFamily.value()
            << '\n';
    }


    if (queueFamilies.computeFamily.has_value())
    {
        std::cout
            << "Compute Queue Family: "
            << queueFamilies.computeFamily.value()
            << '\n';
    }


    if (queueFamilies.transferFamily.has_value())
    {
        std::cout
            << "Transfer Queue Family: "
            << queueFamilies.transferFamily.value()
            << '\n';
    }
    // ============================================================
    // 6. Main Loop
    // ============================================================

    while (!window.ShouldClose())
    {
        window.PollEvent();


        // --------------------------------------------------------
        // Later:
        //
        // Vulkan rendering
        // Physical device
        // Logical device
        // Swapchain
        // Command buffers
        // etc.
        // --------------------------------------------------------
    }


    // ============================================================
    // 7. Cleanup
    // ============================================================

    /*
        IMPORTANT:

        Surface must be destroyed BEFORE
        the Vulkan instance.
    */

    surfaceProvider.DestroyedVulkanSurface(
        instanceProvider.GetVulkanInstance(),
        surface
    );


    instanceProvider.DeleteInstance();


    /*
        window destructor will call Shutdown()
        and terminate GLFW.
    */

    return 0;
}
//VANTA ENGINE
//            │
//┌───────────┴───────────┐
//│                       │
//▼                       ▼
//GLFWWindow          VulkanInstanceProvider
//│                       │
//│                       │
//GLFW only              Vulkan only
//│                       │
//└──────────┐    ┌───────┘
//▼    ▼
//VulkanSurfaceProvider
//│
//GLFW + Vulkan