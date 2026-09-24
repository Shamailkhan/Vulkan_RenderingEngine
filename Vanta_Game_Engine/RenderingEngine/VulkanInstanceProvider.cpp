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

bool VulkanInstanceProvider::Initialization()
{
	if (VulkanInitiazed)
		return true;

	if (!CreateInstance())
		return false; 

	VulkanInitiazed = true;
	return true;

}

void VulkanInstanceProvider::DeleteInstance()
{
}

VkInstance VulkanInstanceProvider::GetVulkanInstance() const
{
	return VkInstance();
}

bool VulkanInstanceProvider::CreateInstance()
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
	VkApplicationInfo appinfo{};
	CreateAppInfo(appinfo);


	return false;
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


	return true;;
}

void VulkanInstanceProvider::CreateAppInfo(VkApplicationInfo& appInfo)
{
}





