#include "HighLoPch.h"
#include "WindowsFileDialogue.h"

#ifdef HL_PLATFORM_WINDOWS

namespace highlo
{
	HLString WindowsFileDialogue::OpenFileDialogueInternal(FILEOPENDIALOGOPTIONS options, bool open)
	{
		HLString result = "";
		HRESULT hr = E_FAIL;

		if (open)
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&m_FileDialogue));
		else
			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&m_FileDialogue));

		if (!SUCCEEDED(hr))
			return result;

		if (m_Filter.HasFilters())
		{
			std::vector<COMDLG_FILTERSPEC> filters;
			for (auto &pair : m_Filter.GetFilters())
			{
				filters.push_back({ pair.first.W_Str(), pair.second.W_Str() });
			}

			m_FileDialogue->SetFileTypes((UINT) filters.size(), &filters[0]);
		}

		m_FileDialogue->SetOptions(options);
		hr = m_FileDialogue->Show(NULL);

		if (!SUCCEEDED(hr))
		{
			m_FileDialogue->Release();
			return result;
		}

		IShellItem *item;
		hr = m_FileDialogue->GetResult(&item);

		if (!SUCCEEDED(hr))
		{
			m_FileDialogue->Release();
			return result;
		}

		PWSTR filePath;
		hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

		if (SUCCEEDED(hr))
		{
			size_t chars_converted = 0;
			char path[MAX_PATH];
			wcstombs_s(&chars_converted, path, filePath, MAX_PATH);

			result = path;
			CoTaskMemFree(filePath);
		}

		item->Release();
		m_FileDialogue->Release();
		return result;
	}

	HLString WindowsFileDialogue::SelectDirectory()
	{
		return OpenFileDialogueInternal(FOS_PICKFOLDERS | FOS_PATHMUSTEXIST, true);
	}

	HLString WindowsFileDialogue::SelectFile()
	{
		return OpenFileDialogueInternal(FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST, true);
	}
	
	HLString WindowsFileDialogue::SaveFile()
	{
		return OpenFileDialogueInternal(FOS_PATHMUSTEXIST, false);
	}
}

#endif // HL_PLATFORM_WINDOWS