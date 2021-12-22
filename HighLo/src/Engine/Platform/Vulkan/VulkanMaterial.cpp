#include "HighLoPch.h"
#include "VulkanMaterial.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanMaterial::VulkanMaterial(const Ref<Shader> &shader, const HLString &name)
	{
	}
	
	VulkanMaterial::VulkanMaterial(const Ref<Material> &other, const HLString &name)
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
	
	void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex)
	{
	}

	void VulkanMaterial::Set(const HLString &name, const Ref<Texture3D> &texture)
	{
	}

	float &VulkanMaterial::GetFloat(const HLString &name)
	{
		float changeMe = 0.0f;
		return changeMe;
	}
	
	int32 &VulkanMaterial::GetInt(const HLString &name)
	{
		int32 changeMe = 0;
		return changeMe;
	}
	
	uint32 &VulkanMaterial::GetUInt(const HLString &name)
	{
		uint32 changeMe = 0;
		return changeMe;
	}

	bool &VulkanMaterial::GetBool(const HLString &name)
	{
		bool changeMe = false;
		return changeMe;
	}
	
	glm::vec2 &VulkanMaterial::GetVector2(const HLString &name)
	{
		glm::vec2 changeMe{ 0.0f, 0.0f };
		return changeMe;
	}
	
	glm::vec3 &VulkanMaterial::GetVector3(const HLString &name)
	{
		glm::vec3 changeMe{ 0.0f, 0.0f, 0.0f };
		return changeMe;
	}

	glm::vec4 &VulkanMaterial::GetVector4(const HLString &name)
	{
		glm::vec4 changeMe{ 0.0f, 0.0f, 0.0f, 0.0f };
		return changeMe;
	}

	glm::ivec2 &VulkanMaterial::GetIVector2(const HLString &name)
	{
		glm::ivec2 changeMe{ 0, 0 };
		return changeMe;
	}

	glm::ivec3 &VulkanMaterial::GetIVector3(const HLString &name)
	{
		glm::ivec3 changeMe{ 0, 0, 0 };
		return changeMe;
	}

	glm::ivec4 &VulkanMaterial::GetIVector4(const HLString &name)
	{
		glm::ivec4 changeMe{ 0, 0, 0, 0 };
		return changeMe;
	}

	glm::mat2 &VulkanMaterial::GetMatrix2(const HLString &name)
	{
		glm::mat2 changeMe{};
		return changeMe;
	}

	glm::mat3 &VulkanMaterial::GetMatrix3(const HLString &name)
	{
		glm::mat3 changeMe{};
		return changeMe;
	}

	glm::mat4 &VulkanMaterial::GetMatrix4(const HLString &name)
	{
		glm::mat4 changeMe{};
		return changeMe;
	}

	Ref<Texture2D> &VulkanMaterial::GetTexture2D(const HLString &name)
	{
		Ref<Texture2D> changeMe = Ref<Texture2D>();
		return changeMe;
	}

	Ref<Texture3D> VulkanMaterial::GetTexture3D(const HLString &name)
	{
		Ref<Texture3D> changeMe = Ref<Texture3D>();
		return changeMe;
	}

	Ref<Texture2D> VulkanMaterial::TryGetTexture2D(const HLString &name)
	{
		Ref<Texture2D> changeMe = Ref<Texture2D>();
		return changeMe;
	}

	Ref<Texture3D> VulkanMaterial::TryGetTexture3D(const HLString &name)
	{
		Ref<Texture3D> changeMe = Ref<Texture3D>();
		return changeMe;
	}

	bool VulkanMaterial::GetFlag(MaterialFlag flag) const
	{
		return false;
	}

	void VulkanMaterial::SetFlag(MaterialFlag flag, bool value)
	{
	}
}

#endif // HIGHLO_API_VULKAN

