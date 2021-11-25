#include "HighLoPch.h"
#include "VulkanMaterial.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanMaterial::VulkanMaterial(const Ref<Shader> &shader, const HLString &name)
	{
	}
	
	VulkanMaterial::~VulkanMaterial()
	{
	}
	
	void VulkanMaterial::Invalidate()
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, float value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, int32 value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, uint32 value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, bool value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::vec2 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::vec3 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::vec4 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::ivec2 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::ivec3 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::ivec4 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::mat2 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::mat3 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::mat4 &value)
	{
	}
	
	void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture)
	{
	}
	
	float &VulkanMaterial::GetFloat(const HLString &name)
	{
	}
	
	int32 &VulkanMaterial::GetInt(const HLString &name)
	{
	}
	
	uint32 &VulkanMaterial::GetUInt(const HLString &name)
	{
	}

	bool &VulkanMaterial::GetBool(const HLString &name)
	{
	}
	
	glm::vec2 &VulkanMaterial::GetVector2(const HLString &name)
	{
	}
	
	glm::vec3 &VulkanMaterial::GetVector3(const HLString &name)
	{
	}

	glm::vec4 &VulkanMaterial::GetVector4(const HLString &name)
	{
	}

	glm::ivec2 &VulkanMaterial::GetIVector2(const HLString &name)
	{
	}

	glm::ivec3 &VulkanMaterial::GetIVector3(const HLString &name)
	{
	}

	glm::ivec4 &VulkanMaterial::GetIVector4(const HLString &name)
	{
	}

	glm::mat2 &VulkanMaterial::GetMatrix2(const HLString &name)
	{
	}

	glm::mat3 &VulkanMaterial::GetMatrix3(const HLString &name)
	{
	}

	glm::mat4 &VulkanMaterial::GetMatrix4(const HLString &name)
	{
	}

	Ref<Texture2D> &VulkanMaterial::GetTexture2D(const HLString &name)
	{
	}

	Ref<Texture3D> &VulkanMaterial::GetTexture3D(const HLString &name)
	{
	}

	Ref<Texture2D> VulkanMaterial::TryGetTexture2D(const HLString &name)
	{
	}

	Ref<Texture3D> VulkanMaterial::TryGetTexture3D(const HLString &name)
	{
	}

	uint32 VulkanMaterial::GetFlags() const
	{
	}

	bool VulkanMaterial::GetFlag(MaterialFlag flag) const
	{
	}

	void VulkanMaterial::SetFlag(MaterialFlag flag, bool value)
	{
	}

	void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex)
	{
	}
}

#endif // HIGHLO_API_VULKAN

