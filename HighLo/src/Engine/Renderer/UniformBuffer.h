#pragma once
#include "Engine/Core/DataTypes/HLString.h"
#include "UniformBufferSlotMappings.h"
#include <vector>
#include <map>

namespace highlo
{
	struct UniformVariable
	{
		UniformVariable() {}
		UniformVariable(const std::string& name, uint32_t size)
			: Name(name), Size(size)
		{
		}

		HLString Name;
		uint32 Size = 0;
	};

	enum class UniformBufferParentShader
	{
		VERTEX_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER
	};

	class UniformBuffer : public IsSharedReference
	{
	public:
		virtual ~UniformBuffer();

		uint32 BindingSlot = 0;

		inline uint32  GetSize() const { return m_DataSize; }
		inline void* GetData() const { return m_Data; }
		inline const HLString& GetName() const { return m_Name; }

		void SetVariableValue(const char* name, void* value);
		void SetBufferValue(void* value);

		void GetVariableValue(const char* name, void* out_val);

		virtual void UploadToShader() = 0;

		static Ref<UniformBuffer> Create(const HLString& name, const std::vector<UniformVariable>& layout,
			UniformBufferParentShader parent, uint32 slot = 0);

	protected:
		UniformBuffer(const HLString& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32 slot);
		uint32	m_DataSize = 0;
		void*	m_Data = nullptr;

	protected:
		const HLString m_Name;

	protected:
		UniformBufferParentShader m_ParentShader;

	protected:
		// const char* --> name
		// Pair.first  --> size
		// Pair.second --> offset
		std::map<HLString, std::pair<uint32, uint32>> m_UniformVariables;
	};
}
