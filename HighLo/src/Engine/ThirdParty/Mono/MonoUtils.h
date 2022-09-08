// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-08) initial release
//

#pragma once

#ifdef HIGHLO_API_MONO_SCRIPTING

#include <mono/metadata/class.h>
#include <mono/metadata/object.h>

namespace highlo::utils
{
	template<typename T>
	static MonoClass *TemplateTypeToMonoClass()
	{
		return nullptr;
	}

	template<typename T>
	static MonoArray *StdToMonoArr(MonoDomain *domain, const std::vector<T> &vec)
	{
		MonoArray *result = mono_array_new(domain, utils::TemplateTypeToMonoClass<T>(), vec.size());
		for (uint32 i = 0; i < vec.size(); ++i)
		{
			mono_array_set(result, T, i, vec[i]);
		}

		return result;
	}

	static MonoString *StrToMonoStr(MonoDomain *domain, const HLString &str)
	{
		return mono_string_new(domain, *str);
	}
}

#endif // HIGHLO_API_MONO_SCRIPTING

