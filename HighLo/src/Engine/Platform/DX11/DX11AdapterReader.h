// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Log.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class AdapterData
	{
	public:
		
		AdapterData(IDXGIAdapter *adapter);

		IDXGIAdapter *m_Adapter;
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
