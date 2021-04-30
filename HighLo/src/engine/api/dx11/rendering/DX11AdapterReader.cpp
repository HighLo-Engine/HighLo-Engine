#include "HighLoPch.h"
#include "DX11AdapterReader.h"

#ifdef HIGHLO_API_DX11
#include <logging/Logger.h>

namespace highlo
{
	std::vector<AdapterData> AdapterReader::m_Adapters;

	AdapterData::AdapterData(IDXGIAdapter* adapter)
	{
		m_Adapter = adapter;
		HRESULT result = adapter->GetDesc(&m_Description);
		if (FAILED(result))
			HL_CORE_ERROR("Failed to get adapter description");
	}

	std::vector<AdapterData> AdapterReader::GetAdapters()
	{
		if (m_Adapters.size() > 0)
			return m_Adapters;

		ComPtr<IDXGIFactory> factory;
		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));

		if (FAILED(result))
			HL_CORE_ERROR("Failed to create DXGIFactory");

		IDXGIAdapter* adapter;
		UINT index = 0;
		while (SUCCEEDED(factory->EnumAdapters(index, &adapter)))
		{
			m_Adapters.push_back(AdapterData(adapter));
			index++;
		}

		return m_Adapters;
	}
}
#endif // HIGHLO_API_DX11
