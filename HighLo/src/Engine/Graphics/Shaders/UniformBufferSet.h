// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

#include "UniformBuffer.h"

namespace highlo
{
	class UniformBufferSet : public IsSharedReference
	{
	public:

		using UniformBufferCallback = std::function<void(const Ref<UniformBuffer>&)>;

		HLAPI UniformBufferSet(uint32 frames);
		HLAPI virtual ~UniformBufferSet();

		HLAPI void CreateUniform(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout);

		HLAPI Ref<UniformBuffer> GetUniform(uint32 binding, uint32 set = 0, uint32 frame = 0);
		HLAPI void SetUniform(const Ref<UniformBuffer> &uniform, uint32 set = 0, uint32 frame = 0);

		HLAPI void ForEach(const UniformBufferCallback &callback) const;

		HLAPI static Ref<UniformBufferSet> Create(uint32 frames);

	private:

		uint32 m_Frames;
		std::map<uint32, std::map<uint32, std::map<uint32, Ref<UniformBuffer>>>> m_UniformBuffers; // frame -> set -> binding
	};
}

