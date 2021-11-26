#include "HighLoPch.h"
#include "UniformBuffer.h"

#include "Engine/Platform/OpenGL/OpenGLUniformBuffer.h"

namespace highlo
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32 size, uint32 binding)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLUniformBuffer>::Create(size, binding);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		HL_ASSERT(false);
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

