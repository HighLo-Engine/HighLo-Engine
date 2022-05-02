// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-14) initial release
//

#pragma once

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	struct OpenGLShaderUniformBuffer
	{
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		GLenum Stage = 0;
	};

	struct OpenGLShaderStorageBuffer
	{
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		GLenum Stage = 0;
	};

	struct OpenGLShaderImageSampler
	{
		uint32 BindingPoint = 0;
		uint32 DescriptorSet = 0;
		uint32 ArraySize = 0;
		HLString Name;
		GLenum Stage = 0;
	};

	struct OpenGLShaderPushConstantRange
	{
		uint32 Offset = 0;
		uint32 Size = 0;
		GLenum Stage = 0;
	};

	struct OpenGLShaderDescriptorSet
	{
		std::unordered_map<uint32, OpenGLShaderUniformBuffer*> UniformBuffers;
		std::unordered_map<uint32, OpenGLShaderStorageBuffer*> StorageBuffers;
		std::unordered_map<uint32, OpenGLShaderImageSampler> ImageSamplers;
		std::unordered_map<uint32, OpenGLShaderImageSampler> StorageImages;
		std::unordered_map<uint32, OpenGLShaderImageSampler> SeparateTextures;
		std::unordered_map<uint32, OpenGLShaderImageSampler> SeparateSamplers;

		operator bool() const
		{
			return !(StorageBuffers.empty()
					 && UniformBuffers.empty()
					 && ImageSamplers.empty()
					 && StorageImages.empty());
		}
	};
}

#endif // HIGHLO_API_OPENGL

