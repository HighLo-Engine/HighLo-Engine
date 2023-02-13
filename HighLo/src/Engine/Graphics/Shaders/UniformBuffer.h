// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SharedReference.h"
#include "Engine/Graphics/Shaders/UniformLayout.h"

namespace highlo
{
	class UniformBuffer : public IsSharedReference
	{
	public:

		HLAPI virtual ~UniformBuffer();

		HLAPI virtual void Bind() const = 0;
		HLAPI virtual void Unbind() const = 0;
		HLAPI uint32 GetBinding() const { return m_Binding; }

		HLAPI void SetData(const void *data, uint32 size, uint32 offset = 0, bool uploadToGPU = true);
		HLAPI void SetVariable(const HLString &name, void *value, bool uploadToGPU = true);
		HLAPI void *GetVariable(const HLString &name);

		HLAPI virtual void UploadToShader(bool now = false) = 0;

		HLAPI static Ref<UniformBuffer> Create(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());

	protected:

		UniformBuffer(uint32 binding, const std::vector<UniformVariable> &layout);

	protected:

		// String --> name
		// Pair.first  --> size
		// Pair.second --> offset
		std::map<HLString, std::pair<uint32, uint32>> m_UniformVariables;

		uint32 m_DataSize = 0;
		uint32 m_Binding = 0;

		void *m_Data = nullptr;
	};
}

