#pragma once

#include "ApplicationLayer.h"

namespace highlo
{
	class ApplicationLayerStack
	{
	public:

		HLAPI ApplicationLayerStack() = default;
		HLAPI ~ApplicationLayerStack();

		HLAPI void PushLayer(ApplicationLayer *layer);
		HLAPI void PopLayer(ApplicationLayer *layer);
		HLAPI void PushOverlay(ApplicationLayer *overlay);
		HLAPI void PopOverlay(ApplicationLayer *overlay);

		HLAPI std::vector<ApplicationLayer*>::iterator begin() { return m_Layers.begin(); }
		HLAPI std::vector<ApplicationLayer*>::iterator end() { return m_Layers.end(); }
		HLAPI std::vector<ApplicationLayer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		HLAPI std::vector<ApplicationLayer*>::reverse_iterator rend() { return m_Layers.rend(); }

		HLAPI std::vector<ApplicationLayer*>::const_iterator begin() const { return m_Layers.begin(); }
		HLAPI std::vector<ApplicationLayer*>::const_iterator end() const { return m_Layers.end(); }
		HLAPI std::vector<ApplicationLayer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		HLAPI std::vector<ApplicationLayer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:

		std::vector<ApplicationLayer*> m_Layers;
		uint32 m_LayerInsertIndex = 0;
	};
}

