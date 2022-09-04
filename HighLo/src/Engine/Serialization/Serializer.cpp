// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Serializer.h"

namespace highlo
{
	static std::vector<Ref<Serializable>> s_Serializables;

	void Serializer::Serialize(const FileSystemPath &filePath)
	{
	}

	void Serializer::Deserialize(const FileSystemPath &filePath)
	{
	}
	
	void Serializer::RegisterSerializable(const Ref<Serializable> &obj)
	{
		s_Serializables.push_back(obj);
	}

	void Serializer::UnregisterSerializable(const Ref<Serializable> &obj)
	{
		auto it = std::find(s_Serializables.begin(), s_Serializables.end(), obj);
		if (it != s_Serializables.end())
		{
			s_Serializables.erase(it);
		}
	}
}

