#pragma once
#include <vulkan/vulkan.h>
#include <vector>

//it is a forward declaration of glfw type
// the header dont need to know about the full functionality of the GLFW defination 
//that is why we are forward declaring it 
// it is basically saying that there is a type name GLFWwindow you dont need to know about 

struct GLFWwindow;

class VulkanSurfaceProvider
{
public:

	VulkanSurfaceProvider() = default;
	~VulkanSurfaceProvider() = default;

	//instance extension required by the glfw/vulkan
	std::vector<const char*> GetRequiredInstancesExtension()const;

	//creat vulkan surface

	bool CreateSurface(VkInstance instance, GLFWwindow* window, VkSurfaceKHR& surface);

	//destroy the vulkan surface

	void DestroyedVulkanSurface(VkInstance instance, VkSurfaceKHR surface);


};

