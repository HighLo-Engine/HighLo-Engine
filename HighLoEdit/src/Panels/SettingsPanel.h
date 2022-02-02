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

