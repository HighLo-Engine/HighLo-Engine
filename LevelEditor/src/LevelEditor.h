#pragma once

#include <HighLo.h>
using namespace highlo;

class LevelEditor : public HLApplication
{
public:
	void OnInitialize() override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown() override;
	void OnEvent(Event &e) override;
};