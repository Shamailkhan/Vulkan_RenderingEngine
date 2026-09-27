#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
class VulkanInstanceProvider
{
public:
	VulkanInstanceProvider(std::string ApplicationName,uint32_t applicationVersion=VK_MAKE_VERSION(1,0,0));
	~VulkanInstanceProvider();

	bool Initialization(const std::vector<const char*>& requiredExtensions);
	void DeleteInstance();


	VkInstance GetVulkanInstance() const;

	inline bool isCraeted() { return VulkanInitiazed; }

private:

	bool CreateInstance(const std::vector<const char*>& requiredExtensions);
	bool CheckValidationLayerSupport() const;
	bool CheckValidationLayerSupport(const std::vector<const char*>& layer)const;
	bool CheckExtensionSupport(const std::vector<const char*>& extension) const;

	

	void CreateAppInfo(VkApplicationInfo& appInfo);

	
	bool VulkanInitiazed = false;

	VkInstance m_vulkanInstance = VK_NULL_HANDLE;
	std::string m_applicationName = "";
	uint32_t m_applicationVersion= VK_MAKE_VERSION(1,0,0);
	bool m_enableValidationLayers = false;

};

