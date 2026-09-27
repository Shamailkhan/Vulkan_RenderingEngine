#include "VulkanSurfaceProvider.h"
#include <GLFW/glfw3.h>
#include <iostream>

std::vector<const char*> VulkanSurfaceProvider::GetRequiredInstancesExtension() const
{
   
    uint32_t extensionCount = 0;

    const char** glfwExtension = glfwGetRequiredInstanceExtensions(&extensionCount);

    if (!glfwExtension)
    {
        std::cout << "GLFW failed to provide the required extension for vulkan \n";
        return {};
    }
    std::vector <const char*> extension(glfwExtension,glfwExtension+extensionCount);

    

    return extension;
}

bool VulkanSurfaceProvider::CreateSurface(VkInstance instance, GLFWwindow* window, VkSurfaceKHR& surface)
{

    if (instance == VK_NULL_HANDLE)
    {
        std::cout << " can not craete surface the vulkan instance is null \n";
        return false;
    }

    if (!window)
    {
        std::cout << " can not craete surface the glfw window  instance is null \n";
        return false;
    }

    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);

    if (result != VK_SUCCESS)
    {
     std::cout << "Failed to create the Surface window \n";

     surface = VK_NULL_HANDLE;
     return false;

    }

    std::cout << "SUCCESSFULLY CREATED A VULKAN SURFACE \n";

    return true;
}

void VulkanSurfaceProvider::DestroyedVulkanSurface(VkInstance instance, VkSurfaceKHR surface)
{
    if (instance == VK_NULL_HANDLE)
        return;
    if (surface == VK_NULL_HANDLE)
        return;

    vkDestroySurfaceKHR(instance, surface, nullptr);

}
