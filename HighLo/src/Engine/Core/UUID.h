#pragma once

namespace highlo
{
	class UUID
	{
	public:

		HLAPI UUID();
		HLAPI UUID(uint64 uuid);
		HLAPI UUID(const UUID &other);

		HLAPI operator uint64() { return m_UUID; }
		HLAPI operator const uint64() const { return m_UUID; }

	private:

		uint64 m_UUID;
	};

	class UUID32
	{
	public:

		HLAPI UUID32();
		HLAPI UUID32(uint32 uuid);
		HLAPI UUID32(const UUID32 &other);

		HLAPI operator uint32() { return m_UUID; }
		HLAPI operator const uint32() const { return m_UUID; }

	private:

		uint32 m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<highlo::UUID>
	{
		std::size_t operator()(const highlo::UUID &uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

	template<>
	struct hash<highlo::UUID32>
	{
		std::size_t operator()(const highlo::UUID32 &uuid) const
		{
			return hash<uint32>()((uint32)uuid);
		}
	};
}