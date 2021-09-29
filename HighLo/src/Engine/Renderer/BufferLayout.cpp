// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "BufferLayout.h"

namespace highlo
{
	BufferLayout BufferLayout::Empty = std::initializer_list<BufferElement>();

	BufferLayout BufferLayout::GetTextureLayout()
	{
		return
		{
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_Color", ShaderDataType::Float4 },
			{ "in_TexCoord", ShaderDataType::Float2 },
			{ "in_TexIndex", ShaderDataType::Float },
			{ "in_TilingFactor", ShaderDataType::Float },
			{ "in_EntityID", ShaderDataType::Int }
		};
	}

	BufferLayout BufferLayout::GetLineLayout()
	{
		return
		{
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_Color", ShaderDataType::Float4 },
			{ "in_EntityID", ShaderDataType::Int }
		};
	}

	BufferLayout BufferLayout::GetCircleLayout()
	{
		return
		{
			{ "in_WorldPosition", ShaderDataType::Float3 },
			{ "in_Thickness", ShaderDataType::Float },
			{ "in_LocalPosition", ShaderDataType::Float2 },
			{ "in_Color", ShaderDataType::Float4 },
			{ "in_EntityID", ShaderDataType::Int }
		};
	}

	BufferLayout BufferLayout::GetTextLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 },
			{ "a_TexIndex", ShaderDataType::Float },
			{ "a_Color", ShaderDataType::Float4 },
		};
	}

	BufferLayout BufferLayout::GetStaticShaderLayout()
	{
		return
		{
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 }
		};
	}

	BufferLayout BufferLayout::GetAnimatedShaderLayout()
	{
		return
		{
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 },
			{ "TANGENT"	, ShaderDataType::Float3 },
			{ "BINORMAL", ShaderDataType::Float3 },
			{ "BONE_IDS", ShaderDataType::Int4 },
			{ "BONE_WEIGHTS", ShaderDataType::Float4 }
		};
	}

	BufferLayout BufferLayout::GetSkyboxLayout()
	{
		return
		{
			{ "POSITION", ShaderDataType::Float3 }
		};
	}

	BufferLayout BufferLayout::GetGridLayout()
	{
		return
		{
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_TexCoord", ShaderDataType::Float2 }
		};
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32 offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
}
