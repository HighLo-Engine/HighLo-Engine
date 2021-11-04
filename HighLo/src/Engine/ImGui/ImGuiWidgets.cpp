// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGuiWidgets.h"

namespace highlo::UI
{
	Ref<Texture2D> Widgets::s_SearchIcon;
	Ref<Texture2D> Widgets::s_ClearIcon;
	Ref<Texture2D> Widgets::s_GearIcon;

	void Widgets::Init()
	{
		s_SearchIcon = Texture2D::LoadFromFile("assets/Resources/icons/search.png").As<Texture2D>();
		s_ClearIcon = Texture2D::LoadFromFile("assets/Resources/icons/close.png").As<Texture2D>();
		s_GearIcon = Texture2D::LoadFromFile("assets/Resources/icons/settings.png").As<Texture2D>();
	}
	
	void Widgets::Shutdown()
	{
		s_GearIcon.Reset();
		s_ClearIcon.Reset();
		s_SearchIcon.Reset();
	}
}

