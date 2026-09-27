#include "GLFWWindow.h"

#include "VulkanInstanceProvider.h"
#include "VulkanSurfaceProvider.h"

#include <iostream>


int main()
{
    // ============================================================
    // 1. Create GLFW Window
    // ============================================================

    GLFWWindow window(
        1280,
        720,
        "Vanta Vulkan Engine"
    );


    if (!window.Initialize())
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
        surfaceProvider.GetRequiredInstanceExtensions();


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
        window.GetHandle(),
        surface))
    {
        std::cerr
            << "Failed to create Vulkan surface.\n";

        return -1;
    }


    // ============================================================
    // 6. Main Loop
    // ============================================================

    while (!window.ShouldClose())
    {
        window.PollEvents();


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

    surfaceProvider.DestroySurface(
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