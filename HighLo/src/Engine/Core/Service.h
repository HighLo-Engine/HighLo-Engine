// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

namespace highlo
{
	class Service
	{
	public:

		typedef std::vector<Service*> ServiceArray;
		HLString Name;
		int32 Order;

		HLAPI virtual ~Service() = default;

		HLAPI static ServiceArray &GetServices();
		HLAPI static void Sort();

		HLAPI virtual bool Init();
		HLAPI virtual void Update() {}
		HLAPI virtual void Exit() {}

		HLAPI static void OnInit();
		HLAPI static void OnUpdate(bool updateBackwards = false);
		HLAPI static void OnExit();

	protected:

		HLAPI Service(const HLString &name, int32 order = 0);

	private:

		bool m_IsInitialized = false;
	};
}
