// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "BufferLayout.h"

namespace highlo
{
	BufferLayout BufferLayout::Empty = std::initializer_list<BufferElement>();

	BufferLayout BufferLayout::GetTextureLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_Color", ShaderDataType::Float4 },
			{ "a_TexCoord", ShaderDataType::Float2 },
			{ "a_TexIndex", ShaderDataType::Float },
			{ "a_TilingFactor", ShaderDataType::Float },
			{ "a_EntityID", ShaderDataType::Int }
		};
	}

	BufferLayout BufferLayout::GetLineLayout()
	{
		return
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_Color", ShaderDataType::Float4 },
			{ "a_EntityID", ShaderDataType::Int }
		};
	}

	BufferLayout BufferLayout::GetCircleLayout()
	{
		return
		{
			{ "a_WorldPosition", ShaderDataType::Float3 },
			{ "a_Thickness", ShaderDataType::Float },
			{ "a_LocalPosition", ShaderDataType::Float2 },
			{ "a_Color", ShaderDataType::Float4 },
			{ "a_EntityID", ShaderDataType::Int }
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
			{ "a_Normal", ShaderDataType::Float3 },
			{ "a_Tangent", ShaderDataType::Float3 },
			{ "a_Binormal", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 },
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

	BufferLayout BufferLayout::GetTransformBufferLayout(uint32 offset)
	{
		BufferElement mrow0;
		mrow0.Name = "a_MRow0";
		mrow0.Type = ShaderDataType::Float4;
		mrow0.Size = 4 * sizeof(float);
		mrow0.Offset = offset;
		mrow0.Normalized = false;

		BufferElement mrow1;
		mrow1.Name = "a_MRow1";
		mrow1.Type = ShaderDataType::Float4;
		mrow1.Size = 4 * sizeof(float);
		mrow1.Offset = mrow0.Size + mrow0.Offset;
		mrow1.Normalized = false;

		BufferElement mrow2;
		mrow2.Name = "a_MRow2";
		mrow2.Type = ShaderDataType::Float4;
		mrow2.Size = 4 * sizeof(float);
		mrow2.Offset = mrow0.Offset + (2 * mrow0.Size);
		mrow2.Normalized = false;

		return { mrow0, mrow1, mrow2 };
	}

	uint32 BufferLayout::GetTransformBufferSize()
	{
		return (3 * (4 * sizeof(float)));
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32 offset = 0;
		m_Stride = 0;
		for (auto &element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
}
