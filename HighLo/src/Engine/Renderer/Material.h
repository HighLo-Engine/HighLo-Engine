// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-11-24) refactored hole class to work with new Shader System (SPIR-V)
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Shaders/Shader.h"
#include "Texture2D.h"
#include "Texture3D.h"

namespace highlo
{
	enum class MaterialFlag
	{
		None		= HL_BIT(0),
		DepthTest	= HL_BIT(1),
		Blend		= HL_BIT(2),
		TwoSided	= HL_BIT(3)
	};

	class Material : public IsSharedReference
	{
	public:

		HLAPI virtual ~Material() = default;

		HLAPI virtual void Invalidate() = 0;

		// Setters
		HLAPI virtual void Set(const HLString &name, float value) = 0;
		HLAPI virtual void Set(const HLString &name, int32 value) = 0;
		HLAPI virtual void Set(const HLString &name, uint32 value) = 0;
		HLAPI virtual void Set(const HLString &name, bool value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::vec2 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::vec3 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::vec4 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::ivec2 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::ivec3 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::ivec4 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::mat2 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::mat3 &value) = 0;
		HLAPI virtual void Set(const HLString &name, const glm::mat4 &value) = 0;

		HLAPI virtual void Set(const HLString &name, const Ref<Texture2D> &texture) = 0;
		HLAPI virtual void Set(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex) = 0;
		HLAPI virtual void Set(const HLString &name, const Ref<Texture3D> &texture) = 0;
		
		// Getters
		HLAPI virtual float &GetFloat(const HLString &name) = 0;
		HLAPI virtual int32 &GetInt(const HLString &name) = 0;
		HLAPI virtual uint32 &GetUInt(const HLString &name) = 0;
		HLAPI virtual bool &GetBool(const HLString &name) = 0;
		HLAPI virtual glm::vec2 &GetVector2(const HLString &name) = 0;
		HLAPI virtual glm::vec3 &GetVector3(const HLString &name) = 0;
		HLAPI virtual glm::vec4 &GetVector4(const HLString &name) = 0;
		HLAPI virtual glm::ivec2 &GetIVector2(const HLString &name) = 0;
		HLAPI virtual glm::ivec3 &GetIVector3(const HLString &name) = 0;
		HLAPI virtual glm::ivec4 &GetIVector4(const HLString &name) = 0;
		HLAPI virtual glm::mat2 &GetMatrix2(const HLString &name) = 0;
		HLAPI virtual glm::mat3 &GetMatrix3(const HLString &name) = 0;
		HLAPI virtual glm::mat4 &GetMatrix4(const HLString &name) = 0;

		HLAPI virtual Ref<Texture2D> GetTexture2D(const HLString &name) = 0;
		HLAPI virtual Ref<Texture3D> GetTexture3D(const HLString &name) = 0;

		HLAPI virtual Ref<Texture2D> TryGetTexture2D(const HLString &name) = 0;
		HLAPI virtual Ref<Texture3D> TryGetTexture3D(const HLString &name) = 0;

		HLAPI virtual uint32 GetFlags() const = 0;
		HLAPI virtual bool GetFlag(MaterialFlag flag) const = 0;
		HLAPI virtual void SetFlag(MaterialFlag flag, bool value = true) = 0;

		HLAPI virtual Ref<Shader> GetShader() const = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI virtual void UpdateForRendering() = 0;

		HLAPI static Ref<Material> Create(const Ref<Shader> &shader, const HLString &name = "");
		HLAPI static Ref<Material> Copy(const Ref<Material> &other, const HLString &name = "");
	};
}

