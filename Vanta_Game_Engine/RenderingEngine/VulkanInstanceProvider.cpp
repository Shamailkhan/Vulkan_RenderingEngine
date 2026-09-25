#include "VulkanInstanceProvider.h"
#include<iostream>


#ifdef NDEBUG
static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

namespace
{
	const std::vector<const char*> VALIDATION_LAYERS =
	{
		"VK_LAYER_KHRONOS_validation"
	};
}


VulkanInstanceProvider::VulkanInstanceProvider(std::string ApplicationName,uint32_t applicationVersion)
	:m_applicationName(ApplicationName),
	m_applicationVersion(applicationVersion),
	m_enableValidationLayers
	(ENABLE_VALIDATION_LAYERS)

{
}

VulkanInstanceProvider::~VulkanInstanceProvider()
{
	DeleteInstance();

}

bool VulkanInstanceProvider::Initialization(const std::vector<const char*>& requiredExtensions)
{
	if (VulkanInitiazed)
		return true;

	if (!CreateInstance(requiredExtensions))
	{
		std::cout << "Failed to create Vulkan instance.\n";
		return false; 
	}

	VulkanInitiazed = true;
	std::cout << "Vulkan instance initialized successfully.\n";
	return true;

}

void VulkanInstanceProvider::DeleteInstance()
{

	if (m_vulkanInstance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(m_vulkanInstance,nullptr);
		m_vulkanInstance = VK_NULL_HANDLE;
	}
	m_vulkanInstance = false;


}

VkInstance VulkanInstanceProvider::GetVulkanInstance() const
{
	return VkInstance();
}

bool VulkanInstanceProvider::CreateInstance(const std::vector<const char*>& requiredExtensions)
{

	// --------------------------------------------------------
	// Check validation layers
	// --------------------------------------------------------

	if (m_enableValidationLayers)
	{
		if (!CheckValidationLayerSupport())
		{
			std::cout<<"Requested Vulkan validation layers are not available.\n";
			return false;
		}
			
	}
	// Application information
	VkApplicationInfo appinfo{};
	CreateAppInfo(appinfo);

	// Instance creation information

	VkInstanceCreateInfo createInfo{};

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appinfo;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();

	// Validation Layers
	if (m_enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());

		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}

	else {
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vulkanInstance);

	if (result != VK_SUCCESS)
	{
		std::cout << "vkCreateInstance failed. VkResult = " << result << "\n";
		m_vulkanInstance = VK_NULL_HANDLE;
		return false;

	}

	std::cout << "Vulkan instance craeted \n";

	return true;
}

bool VulkanInstanceProvider::CheckValidationLayerSupport() const
{
	return CheckValidationLayerSupport(VALIDATION_LAYERS);
}

bool VulkanInstanceProvider::CheckValidationLayerSupport(const std::vector<const char*>& layer) const
{
	//the function check if the required validation layer properties exisit 
	//Vulkan Validataion layer is a debugging system that check how your application uses vulkan 
	uint32_t LayerCount = 0;
	VkResult result = vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
	if (result != VK_SUCCESS)
		return false;
	std::vector <VkLayerProperties> avalibleLayers(LayerCount);

	result = vkEnumerateInstanceLayerProperties(&LayerCount,avalibleLayers.data());

	if (result != VK_SUCCESS)
		return false;

	
	for (const char* requestedLayer : layer)
	{
		bool found = false;
		for (const auto& avalible_leyer : avalibleLayers)
		{

			if (std::strcmp(requestedLayer, avalible_leyer.layerName) == 0)
			{
				found = true;
				break;
			}

		}
		if (!found)
		{
			std::cout << " Missing validation layer : " << requestedLayer<<"\n";
			return false;
		}
	}


	return true;
}

bool VulkanInstanceProvider::CheckExtensionSupport(const std::vector<const char*>& extension) const
{
	// Get Number Of Available Extensions
	uint32_t extensionCount = 0;

	VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	if (result != VK_SUCCESS)
		return false;

	
	// Get Extension Properties
	std::vector<VkExtensionProperties> avalibleExtensions(extensionCount);

	result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, avalibleExtensions.data());
	if (result != VK_SUCCESS)
		return false;
	// Check Requested Extensions
	for (const char* requestedExtension : extension)
	{
		bool found = false;
		for (const auto& availableExtension : avalibleExtensions)
		{
			if (std::strcmp(
				requestedExtension,
				availableExtension.extensionName
			) == 0)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			std::cout << "Missing Vulkan instance extension: "
				<< requestedExtension
				<< '\n';
			return false;
		}

	}



	return true;
}





void VulkanInstanceProvider::CreateAppInfo(VkApplicationInfo& appInfo)
{
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.applicationVersion = m_applicationVersion;
	appInfo.pApplicationName = m_applicationName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "VULKAN RENDERING ENGINE";

	appInfo.apiVersion = VK_API_VERSION_1_3;




}





