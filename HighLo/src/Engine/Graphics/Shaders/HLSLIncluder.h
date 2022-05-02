#pragma once

#include "Shader.h"
#include "ShaderPreProcessor.h"

#if 0

#include <dxcapi.h>

namespace highlo
{
	class HLSLIncluder : public IDxcIncludeHandler
	{
	public:

		HRESULT LoadSource(LPCWSTR pFilename, IDxcBlob **ppIncludeSource) override;
		HRESULT QueryInterface(const IID &riid, void **ppvObject) override;

		ULONG AddRef() override { return 0; }
		ULONG Release() override { return 0; }

		std::unordered_set<IncludeData> &&GetIncludeData() { return std::move(m_includeData); }
		std::unordered_set<HLString> &&GetParsedSpecialMacros() { return std::move(m_ParsedSpecialMacros); }

	private:

		inline static IDxcIncludeHandler *s_DefaultIncludeHandler = nullptr;

		std::unordered_set<IncludeData> m_includeData;
		std::unordered_set<HLString> m_ParsedSpecialMacros;
		std::unordered_map<HLString, HeaderCache> m_HeaderCache;
	};
}

#endif

