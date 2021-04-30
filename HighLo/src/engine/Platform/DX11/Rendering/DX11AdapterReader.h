#pragma once

#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

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
