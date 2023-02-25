// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-22) initial release
//

#pragma once

#include "Core.h"

namespace highlo
{
	template<typename T>
	class Singleton
	{
	public:

		HLAPI static T *Get()
		{
			static T _instance;
			return &_instance;
		}

	protected:

		HLAPI Singleton()
		{
		}

		HLAPI ~Singleton()
		{
		}

	private:

		Singleton(Singleton const&)
		{
		}

		Singleton &operator=(Singleton const&)
		{
			return *this;
		}
	};
}

