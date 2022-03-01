#pragma once

#include "Engine/Renderer/Material.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11Material : public Material
	{
	public:

		DX11Material(const Ref<Shader> &shader, const HLString &name = "");
		DX11Material(const Ref<Material> &other, const HLString &name = "");
		virtual ~DX11Material();

		virtual void Invalidate() override;

		// Setters
		virtual void Set(const HLString &name, float value) override;
		virtual void Set(const HLString &name, int32 value) override;
		virtual void Set(const HLString &name, uint32 value) override;
		virtual void Set(const HLString &name, bool value) override;
		virtual void Set(const HLString &name, const glm::vec2 &value) override;
		virtual void Set(const HLString &name, const glm::vec3 &value) override;
		virtual void Set(const HLString &name, const glm::vec4 &value) override;
		virtual void Set(const HLString &name, const glm::ivec2 &value) override;
		virtual void Set(const HLString &name, const glm::ivec3 &value) override;
		virtual void Set(const HLString &name, const glm::ivec4 &value) override;
		virtual void Set(const HLString &name, const glm::mat2 &value) override;
		virtual void Set(const HLString &name, const glm::mat3 &value) override;
		virtual void Set(const HLString &name, const glm::mat4 &value) override;

		virtual void Set(const HLString &name, const Ref<Texture2D> &texture) override;
		virtual void Set(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex) override;
		virtual void Set(const HLString &name, const Ref<Texture3D> &texture) override;

		// Getters
		virtual float &GetFloat(const HLString &name) override;
		virtual int32 &GetInt(const HLString &name) override;
		virtual uint32 &GetUInt(const HLString &name) override;
		virtual bool &GetBool(const HLString &name) override;
		virtual glm::vec2 &GetVector2(const HLString &name) override;
		virtual glm::vec3 &GetVector3(const HLString &name) override;
		virtual glm::vec4 &GetVector4(const HLString &name) override;
		virtual glm::ivec2 &GetIVector2(const HLString &name) override;
		virtual glm::ivec3 &GetIVector3(const HLString &name) override;
		virtual glm::ivec4 &GetIVector4(const HLString &name) override;
		virtual glm::mat2 &GetMatrix2(const HLString &name) override;
		virtual glm::mat3 &GetMatrix3(const HLString &name) override;
		virtual glm::mat4 &GetMatrix4(const HLString &name) override;

		virtual Ref<Texture2D> GetTexture2D(const HLString &name) override;
		virtual Ref<Texture3D> GetTexture3D(const HLString &name) override;

		virtual Ref<Texture2D> TryGetTexture2D(const HLString &name) override;
		virtual Ref<Texture3D> TryGetTexture3D(const HLString &name) override;

		virtual uint32 GetFlags() const override { return m_Flags; }
		virtual bool GetFlag(MaterialFlag flag) const override;
		virtual void SetFlag(MaterialFlag flag, bool value = true) override;

		virtual Ref<Shader> GetShader() const override { return m_Shader; }
		virtual const HLString &GetName() const override { return m_Name; }

		virtual void UpdateForRendering() override;

	private:

		HLString m_Name;
		Ref<Shader> m_Shader;
		uint32 m_Flags = 0;

		Allocator m_LocalData;

		std::vector<Ref<Texture>> m_Textures;
		std::map<uint32, Ref<Texture2D>> m_Texture2Ds;
	};
}

#endif // HIGHLO_API_DX11

