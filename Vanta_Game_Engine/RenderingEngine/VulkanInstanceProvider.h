#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
class VulkanInstanceProvider
{
public:
	VulkanInstanceProvider(std::string ApplicationName,uint32_t applicationVersion=VK_MAKE_VERSION(1,0,0));
	~VulkanInstanceProvider();

	bool Initialization();
	void DeleteInstance();


	VkInstance GetVulkanInstance() const;

	inline bool isCraeted() { return VulkanInitiazed; }

private:

	bool CreateInstance();
	bool CheckValidationLayerSupport() const;
	bool CheckValidationLayerSupport(const std::vector<const char*>& layer)const;

	void CreateAppInfo(VkApplicationInfo& appInfo);

	
	bool VulkanInitiazed = false;

	VkInstance m_vulkanInstance = VK_NULL_HANDLE;
	std::string m_applicationName = "";
	uint32_t m_applicationVersion;
	bool m_enableValidationLayers = false;

};

