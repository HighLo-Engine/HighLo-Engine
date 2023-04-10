// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-08) initial release
//

#pragma once

namespace highlo
{
	enum class UniformLayoutDataType
	{
		None = 0, 
		Float, Float2, Float3, Float4, 
		Mat3, Mat4, 
		Int, Int2, Int3, Int4, 
		UInt, UInt2, UInt3, UInt4, 
		Bool
	};

	struct UniformVariable
	{
		HLString Name;
		UniformLayoutDataType Type = UniformLayoutDataType::Float;
		uint32 TypeCount = 1;
		uint32 Size = 0;
		uint32 Offset = 0;

		HLAPI UniformVariable(const HLString &name, UniformLayoutDataType type, uint32 typeCount = 1, uint32 offset = 0, uint32 size = 0)
			: Name(name), Type(type), TypeCount(typeCount), Size(size), Offset(offset)
		{
			if (size == 0)
			{
				Size = GetComponentSize();
			}
		}

		HLAPI uint32 GetComponentCount() const
		{
			switch (Type)
			{
				case UniformLayoutDataType::Float:		return 1;
				case UniformLayoutDataType::Float2:		return 2;
				case UniformLayoutDataType::Float3:		return 3;
				case UniformLayoutDataType::Float4:		return 4;
				case UniformLayoutDataType::Mat3:		return 3 * 3;
				case UniformLayoutDataType::Mat4:		return 4 * 4;
				case UniformLayoutDataType::Int:		return 1;
				case UniformLayoutDataType::UInt:		return 1;
				case UniformLayoutDataType::Int2:		return 2;
				case UniformLayoutDataType::UInt2:		return 2;
				case UniformLayoutDataType::Int3:		return 3;
				case UniformLayoutDataType::UInt3:		return 3;
				case UniformLayoutDataType::Int4:		return 4;
				case UniformLayoutDataType::UInt4:		return 4;
				case UniformLayoutDataType::Bool:		return 1;
			}

			return 0;
		}

		HLAPI uint32 GetComponentSize() const
		{
			if (Size > 0)
				return Size;

			switch (Type)
			{
				case UniformLayoutDataType::Float:		return TypeCount * sizeof(float);
				case UniformLayoutDataType::Float2:		return TypeCount * sizeof(float) * 2;
				case UniformLayoutDataType::Float3:		return TypeCount * sizeof(float) * 3;
				case UniformLayoutDataType::Float4:		return TypeCount * sizeof(float) * 4;
				case UniformLayoutDataType::Mat3:		return TypeCount * sizeof(float) * 3 * 3;
				case UniformLayoutDataType::Mat4:		return TypeCount * sizeof(float) * 4 * 4;
				case UniformLayoutDataType::Int:		return TypeCount * sizeof(int32);
				case UniformLayoutDataType::Int2:		return TypeCount * sizeof(int32) * 2;
				case UniformLayoutDataType::Int3:		return TypeCount * sizeof(int32) * 3;
				case UniformLayoutDataType::Int4:		return TypeCount * sizeof(int32) * 4;
				case UniformLayoutDataType::UInt:		return TypeCount * sizeof(uint32);
				case UniformLayoutDataType::UInt2:		return TypeCount * sizeof(uint32) * 2;
				case UniformLayoutDataType::UInt3:		return TypeCount * sizeof(uint32) * 3;
				case UniformLayoutDataType::UInt4:		return TypeCount * sizeof(uint32) * 4;
				case UniformLayoutDataType::Bool:		return TypeCount * sizeof(bool) * 2; // TODO: check why *2 is necessary
			}

			return 0;
		}
	};

	class UniformLayout
	{
	public:

		HLAPI UniformLayout()
		{
		}

		HLAPI UniformLayout(const std::initializer_list<UniformVariable> &elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		HLAPI static std::vector<UniformVariable> GetCameraLayout();
		HLAPI static std::vector<UniformVariable> GetScreenDataLayout();
		HLAPI static std::vector<UniformVariable> GetRendererDataLayout();
		HLAPI static std::vector<UniformVariable> GetHBAODataLayout();
		HLAPI static std::vector<UniformVariable> GetShadowDataLayout();
		HLAPI static std::vector<UniformVariable> GetSceneDataLayout();
		HLAPI static std::vector<UniformVariable> GetSceneCompositeLayout();
		HLAPI static std::vector<UniformVariable> GetPointLightDataLayout();
		HLAPI static std::vector<UniformVariable> GetTransformBufferLayout();
		HLAPI static std::vector<UniformVariable> GetAnimatedBoneTransformBufferLayout();
		HLAPI static std::vector<UniformVariable> GetBoneTransformIndexLayout();
		HLAPI static std::vector<UniformVariable> GetMaterialLayout();
		HLAPI static std::vector<UniformVariable> GetSkyboxLayout();

		HLAPI std::vector<HLString> GetNames();
		HLAPI uint32 GetStride() const { return m_Stride; }

	private:

		void CalculateOffsetsAndStride();

		uint32 m_Stride = 0;
		std::vector<UniformVariable> m_Elements;
	};
}

