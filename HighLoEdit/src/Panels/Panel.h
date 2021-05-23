#pragma once

#include <HighLo.h>
using namespace highlo;

class Panel
{
public:

	virtual void Initialize(uint32 width, uint32 height) = 0;
	virtual void Destroy() = 0;
	virtual void Update(Timestep ts) = 0;
	virtual void Render(Timestep ts) = 0;
	virtual void OnEvent(Event &e) = 0;
	virtual void OnResize(uint32 width, uint32 height) = 0;

	virtual bool IsMouseOver() const = 0;
	virtual bool IsSelected() const = 0;
};

