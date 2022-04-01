// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ApplicationLayerStack.h"

namespace highlo
{
	ApplicationLayerStack::~ApplicationLayerStack()
	{
		for (ApplicationLayer *layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void ApplicationLayerStack::PushLayer(ApplicationLayer *layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		++m_LayerInsertIndex;
	}

	void ApplicationLayerStack::PopLayer(ApplicationLayer *layer)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (iterator != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(iterator);
			--m_LayerInsertIndex;
		}
	}

	void ApplicationLayerStack::PushOverlay(ApplicationLayer *overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void ApplicationLayerStack::PopOverlay(ApplicationLayer *overlay)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (iterator != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(iterator);
		}
	}
}

