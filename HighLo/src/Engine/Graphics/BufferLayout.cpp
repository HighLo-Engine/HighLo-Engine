// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

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
			{ "a_Color", ShaderDataType::Float4 },
			{ "a_TexIndex", ShaderDataType::Float },
		};
	}

	BufferLayout BufferLayout::GetStaticShaderLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 },
			{ "a_Normal", ShaderDataType::Float3 },
			{ "a_Tangent", ShaderDataType::Float3 },
			{ "a_Binormal", ShaderDataType::Float3 },
		};
	}

	BufferLayout BufferLayout::GetAnimatedShaderLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 },
			{ "a_Normal", ShaderDataType::Float3 },
			{ "a_Tangent", ShaderDataType::Float3 },
			{ "a_Binormal", ShaderDataType::Float3 },
			{ "a_BoneIndices", ShaderDataType::Int4 },
			{ "a_BoneWeights", ShaderDataType::Float4 },
		};
	}

	BufferLayout BufferLayout::GetSkyboxLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 },
		};
	}

	BufferLayout BufferLayout::GetGridLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 },
		};
	}

	BufferLayout BufferLayout::GetCompositeLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 },
		};
	}

	BufferLayout BufferLayout::GetTransformBufferLayout()
	{
		return
		{
			{ "a_MRow0", ShaderDataType::Float4 },
			{ "a_MRow1", ShaderDataType::Float4 },
			{ "a_MRow2", ShaderDataType::Float4 }
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
