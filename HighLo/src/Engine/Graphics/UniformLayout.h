#pragma once

namespace highlo
{
	enum class UniformLayoutDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	struct UniformLayoutElement
	{
		HLString Name;
		UniformLayoutDataType Type = UniformLayoutDataType::Float;
		uint32 Size = 0;
		uint32 Offset = 0;
	};

	class UniformLayout
	{
	public:

		HLAPI UniformLayout()
		{
		}

		HLAPI UniformLayout(const std::initializer_list<UniformLayoutElement> &elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		HLAPI uint32 GetStride() const { return m_Stride; }

	private:

		void CalculateOffsetsAndStride();

		uint32 m_Stride = 0;
		std::vector<UniformLayoutElement> m_Elements;
	};
}

