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



	return false;
}

bool VulkanInstanceProvider::CheckValidationLayerSupport() const
{
	return false;
}

bool VulkanInstanceProvider::CheckValidationLayerSupport(const std::vector<const char*>& layer) const
{
	return false;
}

void VulkanInstanceProvider::CreateAppInfo(VkApplicationInfo& appInfo)
{
}





