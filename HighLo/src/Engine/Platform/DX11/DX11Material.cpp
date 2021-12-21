#include "HighLoPch.h"
#include "DX11Material.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11Material::DX11Material(const Ref<Shader> &shader, const HLString &name)
	{
	}

	DX11Material::DX11Material(const Ref<Material> &other, const HLString &name)
	{
	}
	
	DX11Material::~DX11Material()
	{
	}
	
	void DX11Material::Invalidate()
	{
	}
	
	void DX11Material::Set(const HLString &name, float value)
	{
	}
	
	void DX11Material::Set(const HLString &name, int32 value)
	{
	}
	
	void DX11Material::Set(const HLString &name, uint32 value)
	{
	}
	
	void DX11Material::Set(const HLString &name, bool value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::vec2 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::vec3 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::vec4 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::ivec2 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::ivec3 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::ivec4 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::mat2 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::mat3 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const glm::mat4 &value)
	{
	}
	
	void DX11Material::Set(const HLString &name, const Ref<Texture2D> &texture)
	{
	}

	void DX11Material::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex)
	{

	}

	void DX11Material::Set(const HLString &name, const Ref<Texture3D> &texture)
	{

	}
	
	float &DX11Material::GetFloat(const HLString &name)
	{
		float changeMe = 0.0f;
		return changeMe;
	}
	
	int32 &DX11Material::GetInt(const HLString &name)
	{
		int32 changeMe = 0;
		return changeMe;
	}
	
	uint32 &DX11Material::GetUInt(const HLString &name)
	{
		uint32 changeMe = 0;
		return changeMe;
	}
	
	bool &DX11Material::GetBool(const HLString &name)
	{
		bool changeMe = false;
		return changeMe;
	}
	
	glm::vec2 &DX11Material::GetVector2(const HLString &name)
	{
		glm::vec2 changeMe{0.0f, 0.0f};
		return changeMe;
	}
	
	glm::vec3 &DX11Material::GetVector3(const HLString &name)
	{
		glm::vec3 changeMe{ 0.0f, 0.0f, 0.0f };
		return changeMe;
	}
	
	glm::vec4 &DX11Material::GetVector4(const HLString &name)
	{
		glm::vec4 changeMe{ 0.0f, 0.0f, 0.0f, 0.0f };
		return changeMe;
	}
	
	glm::ivec2 &DX11Material::GetIVector2(const HLString &name)
	{
		glm::ivec2 changeMe{ 0, 0 };
		return changeMe;
	}
	
	glm::ivec3 &DX11Material::GetIVector3(const HLString &name)
	{
		glm::ivec3 changeMe{ 0, 0, 0 };
		return changeMe;
	}
	
	glm::ivec4 &DX11Material::GetIVector4(const HLString &name)
	{
		glm::ivec4 changeMe{ 0, 0, 0, 0 };
		return changeMe;
	}
	
	glm::mat2 &DX11Material::GetMatrix2(const HLString &name)
	{
		glm::mat2 changeMe{};
		return changeMe;
	}
	
	glm::mat3 &DX11Material::GetMatrix3(const HLString &name)
	{
		glm::mat3 changeMe{};
		return changeMe;
	}
	
	glm::mat4 &DX11Material::GetMatrix4(const HLString &name)
	{
		glm::mat4 changeMe{};
		return changeMe;
	}
	
	Ref<Texture2D> &DX11Material::GetTexture2D(const HLString &name)
	{
		return Ref<Texture2D>();
	}
	
	Ref<Texture3D> DX11Material::GetTexture3D(const HLString &name)
	{
		return Ref<Texture3D>();
	}
	
	Ref<Texture2D> DX11Material::TryGetTexture2D(const HLString &name)
	{
		return Ref<Texture2D>();
	}
	
	Ref<Texture3D> DX11Material::TryGetTexture3D(const HLString &name)
	{
		return Ref<Texture3D>();
	}
	
	bool DX11Material::GetFlag(MaterialFlag flag) const
	{
		return false;
	}
	
	void DX11Material::SetFlag(MaterialFlag flag, bool value)
	{
	}
}

#endif // HIGHLO_API_DX11

