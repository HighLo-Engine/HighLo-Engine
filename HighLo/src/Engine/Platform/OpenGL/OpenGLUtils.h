// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"
#include "Engine/Graphics/TextureFormat.h"

namespace highlo
{
	namespace utils
	{
		inline GLenum OpenGLTextureFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
				case TextureFormat::SRGB:
					return GL_RGB;

				case TextureFormat::RGBA:
				case TextureFormat::RGBA8:
				case TextureFormat::RGBA16:
				case TextureFormat::RGBA16F:
				case TextureFormat::RGBA32:
				case TextureFormat::RGBA32F:
					return GL_RGBA;
				
				case TextureFormat::Depth:
					return GL_DEPTH_STENCIL;

				case TextureFormat::DEPTH32F:
					return GL_DEPTH_STENCIL;

				case TextureFormat::RED_INTEGER:
					return GL_RED_INTEGER;

				case TextureFormat::RED32F:
					return GL_RED;
			}

		HL_ASSERT(false);
		return 0;
		}

		inline GLenum OpenGLTextureInternalFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
					return GL_RGB8;

				case TextureFormat::RGBA:
				case TextureFormat::RGBA8:
					return GL_RGBA8;

				case TextureFormat::SRGB:
					return GL_SRGB8;

				case TextureFormat::RGBA16F:
					return GL_RGBA16F;

				case TextureFormat::RGBA32F:
					return GL_RGBA32F;

				case TextureFormat::DEPTH24STENCIL8:
					return GL_DEPTH24_STENCIL8;

				case TextureFormat::DEPTH32F:
					return GL_DEPTH_COMPONENT32F;

				case TextureFormat::RED_INTEGER:
					return GL_R32I;

				case TextureFormat::RED32F:
					return GL_R32F;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLFormatDataType(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
				case TextureFormat::SRGB:
				case TextureFormat::RGBA:
				case TextureFormat::DEPTH24STENCIL8:
				case TextureFormat::DEPTH32F:
					return GL_UNSIGNED_BYTE;

				case TextureFormat::RGBA16F:
				case TextureFormat::RGBA32F:
					return GL_FLOAT;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLSamplerWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
				case TextureWrap::Clamp:
					return GL_CLAMP_TO_EDGE;

				case TextureWrap::Repeat:
					return GL_REPEAT;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLSamplerFilter(TextureFilter filter, bool mipmap)
		{
			switch (filter)
			{
				case TextureFilter::Linear:	
					return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

				case TextureFilter::Nearest:
					return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
			}

			HL_ASSERT(false);
			return 0;
		}

		// Shader utils
		static GLenum ShaderStageFromString(const HLString &str)
		{
			HLString type = str.ToLowerCase();

			if (type == "vertex")
				return GL_VERTEX_SHADER_BIT;

			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER_BIT;

			if (type == "compute")
				return GL_COMPUTE_SHADER_BIT;

			if (type == "tesscontrol")
				return GL_TESS_CONTROL_SHADER_BIT;

			if (type == "tessevaluation")
				return GL_TESS_EVALUATION_SHADER_BIT;

			if (type == "geometry")
				return GL_GEOMETRY_SHADER_BIT;

			return 0;
		}

		static GLenum ShaderTypeToOpenGLStage(ShaderType type)
		{
			switch (type)
			{
				case ShaderType::Vertex:
					return GL_VERTEX_SHADER_BIT;

				case ShaderType::Fragment:
					return GL_FRAGMENT_SHADER_BIT;

				case ShaderType::Compute:
					return GL_COMPUTE_SHADER_BIT;

				case ShaderType::TessControl:
					return GL_TESS_CONTROL_SHADER_BIT;

				case ShaderType::TessEvaluation:
					return GL_TESS_EVALUATION_SHADER_BIT;

				case ShaderType::Geometry:
					return GL_GEOMETRY_SHADER_BIT;
			}

			HL_ASSERT(false);
			return 0;
		}

		static ShaderType GLStageToShaderType(const GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER_BIT:
					return ShaderType::Vertex;

				case GL_FRAGMENT_SHADER_BIT:
					return ShaderType::Fragment;

				case GL_COMPUTE_SHADER_BIT:
					return ShaderType::Compute;

				case GL_TESS_CONTROL_SHADER_BIT:
					return ShaderType::TessControl;

				case GL_TESS_EVALUATION_SHADER_BIT:
					return ShaderType::TessEvaluation;

				case GL_GEOMETRY_SHADER_BIT:
					return ShaderType::Geometry;
			}

			return ShaderType::None;
		}

		static std::unordered_map<GLenum, HLString> ConvertShaderTypeToOpenGLStage(const std::unordered_map<ShaderType, HLString> &sources)
		{
			std::unordered_map<GLenum, HLString> result;
			for (auto &[type, source] : sources)
			{
				GLenum stage = utils::ShaderTypeToOpenGLStage(type);
				result.insert({ stage, source });
			}

			return result;
		}

		static std::unordered_map<ShaderType, HLString> ConvertOpenGLStageToShaderType(const std::unordered_map<GLenum, HLString> &sources)
		{
			std::unordered_map<ShaderType, HLString> result;
			for (auto &[stage, source] : sources)
			{
				ShaderType type = utils::GLStageToShaderType(stage);
				result.insert({ type, source });
			}

			return result;
		}

		static std::string_view StageToShaderMacro(const std::string_view stage)
		{
			if (stage == "vertex") return "__VERTEX_STAGE__";
			if (stage == "fragment") return "__FRAGMENT_STAGE__";
			if (stage == "compute") return "__COMPUTE_STAGE__";
			if (stage == "geometry") return "__GEOMETRY_STAGE__";
			if (stage == "tesscontrol") return "__TESS_CONTROL_STAGE__";
			if (stage == "tesseval") return "__TESS_EVAL_STAGE__";

			HL_ASSERT(false, "Unknown shader stage.");
			return "";
		}
	}
}

#endif // HIGHLO_API_OPENGL
