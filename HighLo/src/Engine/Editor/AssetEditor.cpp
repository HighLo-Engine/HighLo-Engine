// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetEditor.h"

namespace highlo
{
	void AssetEditor::OnUIRender(Timestep ts)
	{
		if (!m_IsOpen)
			return;

		bool wasOpen = m_IsOpen;
		ImGui::SetNextWindowSizeConstraints(m_MinSize, m_MaxSize);
		
		ImGui::Begin(*m_Title, &m_IsOpen, GetWindowFlags());
		Render();
		ImGui::End();

		if (wasOpen && !m_IsOpen)
			OnClose();
	}
	
	void AssetEditor::SetOpen(bool shouldOpen)
	{
		m_IsOpen = shouldOpen;
		if (!m_IsOpen)
			OnClose();
	}

	AssetEditor::AssetEditor(const HLString &title)
		: m_Title(title), m_MinSize(200, 400), m_MaxSize(2000, 2000)
	{
	}
	
	void AssetEditor::SetMinSize(uint32 width, uint32 height)
	{
		if (width <= 0) width = 200;
		if (height <= 0) height = 400;

		m_MinSize = ImVec2((float)width, (float)height);
	}
	
	void AssetEditor::SetMaxSize(uint32 width, uint32 height)
	{
		if (width <= 0) width = 2000;
		if (height <= 0) height = 2000;
		if (((float)width) <= m_MinSize.x) width = (uint32)(m_MinSize.x * 2.f);
		if (((float)height) <= m_MinSize.y) height = (uint32)(m_MinSize.y * 2.f);

		m_MaxSize = ImVec2((float)width, (float)height);
	}
}

