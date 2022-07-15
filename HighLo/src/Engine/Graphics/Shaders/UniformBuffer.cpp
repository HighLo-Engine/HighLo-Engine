// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UniformBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLUniformBuffer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanUniformBuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11UniformBuffer.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12UniformBuffer.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLUniformBuffer>::Create(size, binding, layout);
	#elif HIGHLO_API_DX11
		return Ref<DX11UniformBuffer>::Create(size, binding, layout);
	#elif HIGHLO_API_DX12
		return Ref<DX12UniformBuffer>::Create(size, binding, layout);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanUniformBuffer>::Create(size, binding, layout);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	UniformBuffer::UniformBuffer(uint32 binding, const std::vector<UniformVariable> &layout)
	{
		uint32 offset = 0;
		uint32 size = 0;

		for (auto &element : layout)
		{
			m_UniformVariables[element.Name] = { element.Size, element.Offset };
			offset += element.Offset + element.Size;
			size += element.Size;
		}

		m_DataSize = size;
		m_Binding = binding;
		m_Data = malloc(m_DataSize);
	}

	UniformBuffer::~UniformBuffer()
	{
		if (m_Data)
			free(m_Data);
	}

	void UniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		// TODO: this could be potentially dangerous, if size is less the m_DataSize, because then we would not have the complete m_DataSize space filled with actual data
		memcpy_s(m_Data, m_DataSize, (void*)((char*)data + offset), m_DataSize);
		UploadToShader();
	}

	void UniformBuffer::SetVariable(const HLString &name, void *value)
	{
		auto &entry = m_UniformVariables.find(name);
		if (entry != m_UniformVariables.end())
		{
			uint32 size = entry->second.first;
			uint32 offset = entry->second.second;
			memcpy_s((void*)((char*)m_Data + offset), size, value, size);
		}

		UploadToShader();
	}
	
	void *UniformBuffer::GetVariable(const HLString &name)
	{
		auto &entry = m_UniformVariables.find(name);
		if (entry != m_UniformVariables.end())
		{
			uint32 size = entry->second.first;
			uint32 offset = entry->second.second;

			void *result = malloc(size);
			memcpy_s(result, size, (void*)((char*)m_Data + offset), size);
			return result;
		}

		return nullptr;
	}
}

