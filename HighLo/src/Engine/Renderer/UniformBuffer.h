// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <vector>
#include <map>

#include "Engine/Core/DataTypes/String.h"
#include "UniformBufferSlotMappings.h"

namespace highlo
{
	struct UniformVariable
	{
		UniformVariable() {}
		UniformVariable(const HLString &name, uint32 size)
			: Name(name), Size(size)
		{
		}

		HLString Name;
		uint32 Size = 0;
	};

	enum class UniformBufferParentShader
	{
		VERTEX_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER,
		COMPUTE_SHADER
	};

	class UniformBuffer : public IsSharedReference
	{
	public:
		virtual ~UniformBuffer();

		uint32 BindingSlot = 0;

		HLAPI inline uint32  GetSize() const { return m_Size; }
		HLAPI inline void *GetData() const { return m_Data; }
		HLAPI inline const HLString &GetName() const { return m_Name; }

		HLAPI void SetVariableValue(const HLString &name, void *value);
		HLAPI void SetBufferValue(void *value);

		HLAPI void GetVariableValue(const HLString &name, void *out_val);

		virtual void UploadToShader(uint32 offset = 0) = 0;

		static Ref<UniformBuffer> Create(const HLString &name, const std::vector<UniformVariable> &layout,
			UniformBufferParentShader parent, uint32 slot = 0);

	protected:

		UniformBuffer(const HLString &name, const std::vector<UniformVariable> &layout, UniformBufferParentShader parent, uint32 slot);
	
	protected:

		uint32	m_Size = 0;
		void   *m_Data = nullptr;
		const HLString m_Name;
		UniformBufferParentShader m_ParentShader;

		// const char* --> name
		// Pair.first  --> size
		// Pair.second --> offset
		std::map<HLString, std::pair<uint32, uint32>> m_UniformVariables;
	};
}
