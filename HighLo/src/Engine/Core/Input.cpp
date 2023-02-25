// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Input.h"

namespace highlo
{
	bool Input::HasController(int32 id)
	{
		return s_Controllers.find(id) != s_Controllers.end();
	}
	
	std::vector<int32> Input::GetConnectedControllerIDs()
	{
		std::vector<int32> result;
		result.reserve(s_Controllers.size());
		for (auto [id, controller] : s_Controllers)
			result.emplace_back(id);

		return result;
	}
	
	const Controller *Input::GetController(int32 id)
	{
		if (!Input::HasController(id))
			return nullptr;

		return &s_Controllers.at(id);
	}
	
	HLString Input::GetControllerName(int32 id)
	{
		if (!Input::HasController(id))
			return {};

		return s_Controllers.at(id).Name;
	}
	
	bool Input::IsControllerButtonPressed(int32 id, int32 button)
	{
		if (!Input::HasController(id))
			return false;

		const Controller &controller = s_Controllers.at(id);
		if (controller.ButtonStates.find(button) == controller.ButtonStates.end())
			return false;

		return controller.ButtonStates.at(button);
	}
	
	float Input::GetControllerAxis(int32 id, int32 axis)
	{
		if (!Input::HasController(id))
			return 0.0f;

		const Controller &controller = s_Controllers.at(id);
		if (controller.AxisStates.find(axis) == controller.AxisStates.end())
			return 0.0f;

		return controller.AxisStates.at(axis);
	}
	
	uint8 Input::GetControllerHat(int32 id, int32 hat)
	{
		if (!Input::HasController(id))
			return 0;

		const Controller &controller = s_Controllers.at(id);
		if (controller.HatStates.find(hat) == controller.HatStates.end())
			return 0;

		return controller.HatStates.at(hat);
	}
}

