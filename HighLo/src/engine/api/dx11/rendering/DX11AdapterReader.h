#pragma once

#include "engine/core/Log.h"

#ifdef HIGHLO_API_DX11

#include "engine/api/dx11/DX11Resources.h"

namespace highlo
{
	class AdapterData
	{
	public:
		AdapterData(IDXGIAdapter* adapter);
		IDXGIAdapter* m_Adapter;
		DXGI_ADAPTER_DESC m_Description;
	};

	class AdapterReader
	{
	public:
		static std::vector<AdapterData> GetAdapters();
	private:
		static std::vector<AdapterData> m_Adapters;
	};
}
#endif // HIGHLO_API_DX11
