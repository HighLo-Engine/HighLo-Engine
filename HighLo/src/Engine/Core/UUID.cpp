// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UUID.h"

#include <random>

namespace highlo
{
	static std::random_device s_UUID_RandomDevice;

	static std::mt19937_64 s_UUID_eng(s_UUID_RandomDevice());
	static std::uniform_int_distribution<uint64> s_UUID_UniformDistribution;

	static std::mt19937 s_UUID_eng32(s_UUID_RandomDevice());
	static std::uniform_int_distribution<uint32> s_UUID_UniformDistribution32;

	UUID::UUID()
		: m_UUID(s_UUID_UniformDistribution(s_UUID_eng))
	{
	}

	UUID::UUID(uint64 uuid)
		: m_UUID(uuid)
	{
	}
	
	UUID::UUID(const UUID &other)
		: m_UUID(other.m_UUID)
	{
	}

	UUID32::UUID32()
		: m_UUID(s_UUID_UniformDistribution32(s_UUID_eng32))
	{
	}
	
	UUID32::UUID32(uint32 uuid)
		: m_UUID(uuid)
	{
	}
	
	UUID32::UUID32(const UUID32 &other)
		: m_UUID(other.m_UUID)
	{
	}
}

