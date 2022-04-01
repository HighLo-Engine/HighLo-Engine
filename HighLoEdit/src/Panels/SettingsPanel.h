// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#pragma once

#include <HighLo.h>
using namespace highlo;

class SettingsPanel
{
public:

	SettingsPanel();
	virtual ~SettingsPanel();

	void OnUIRender(bool *pOpen = nullptr);

private:

	bool ShowYesNoDialogue();
};

