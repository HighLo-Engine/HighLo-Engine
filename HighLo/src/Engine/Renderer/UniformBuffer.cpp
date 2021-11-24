// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UniformBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLUniformBuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11UniformBuffer.h"
#elif HIGHLO_API_DX12
// TODO
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanUniformBuffer.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	UniformBuffer::~UniformBuffer()
	{
		if (m_Data)
			free(m_Data);
	}

	void UniformBuffer::SetVariableValue(const HLString &name, void *value)
	{
		auto entry = m_UniformVariables.find(name);
		if (entry != m_UniformVariables.end())
		{
			uint32 size = entry->second.first;
			uint32 offset = entry->second.second;
			memcpy_s((void*)((char*)m_Data + offset), size, value, size);
		}
	}
	
	void UniformBuffer::SetBufferValue(void *value)
	{
		memcpy_s(m_Data, m_Size, value, m_Size);
	}
	
	void UniformBuffer::GetVariableValue(const HLString &name, void *outVal)
	{
		auto entry = m_UniformVariables.find(name);
		if (entry != m_UniformVariables.end())
		{
			uint32 size = entry->second.first;
			uint32 offset = entry->second.second;
			memcpy_s(outVal, size, (void*)((char*)m_Data + offset), size);
		}
	}
	
	UniformBuffer::UniformBuffer(const HLString &name, const std::vector<UniformVariable> &layout, UniformBufferParentShader parent, uint32 slot)
		: m_Name(name), m_ParentShader(parent), BindingSlot(slot)
	{
		uint32 offset = 0;
		uint32 size = 0;

		for (auto &element : layout)
		{
			m_UniformVariables[element.Name] = { (uint32)element.Size, (uint32)offset };
			offset += element.Size;
			size += element.Size;
		}

		m_Size = size;
		m_Data = malloc(m_Size);
	}

	Ref<UniformBuffer> UniformBuffer::Create(const HLString &name, const std::vector<UniformVariable> &layout, UniformBufferParentShader parent, uint32 slot)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLUniformBuffer>::Create(name, layout, parent, slot);
	#elif HIGHLO_API_DX11
		return Ref<DX11UniformBuffer>::Create(name, layout, parent, slot);
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanUniformBuffer>::Create(name, layout, parent, slot);
	#endif // HIGHLO_API_OPENGL
	}
}

