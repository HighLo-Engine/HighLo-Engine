// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FontLoader.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Renderer/TrueTypeFont.h"
#include "Engine/Loaders/DocumentReader.h"

namespace highlo
{
	bool FontLoader::LoadTrueTypeFont(const FileSystemPath &filePath, TrueTypeFontLoaderResult *out_result)
	{
		if (!out_result)
		{
			HL_CORE_ERROR("Required to already have an output variable allocated, but did only get a nullptr. Aborting.");
			return false;
		}

		if (!FileSystem::Get()->FileExists(filePath))
		{
			HL_CORE_ERROR("Could not find the font file {0}. Aborting.", **filePath);
			return false;
		}

		int64 byteSize = 0;
		Byte *bytes = FileSystem::Get()->ReadFile(filePath, &byteSize);
		if (!bytes)
		{
			HL_CORE_ERROR("Could not load font file {0}. Aborting.", **filePath);
			return false;
		}

		TrueTypeFontLoaderResult result;
		result.BinarySize = (uint64)byteSize;
		result.FontBinary = bytes;
		result.FontCount = 0;
		result.Fonts = nullptr;

		// Try to load a font config file
		FileSystemPath config_path = filePath.ParentPath();
		const HLString &config_name = filePath.Filename();
		FileSystemPath config_file_name = fmt::format("{0}{1}", **(config_path / config_name), ".fontcfg").c_str();

		Ref<DocumentReader> reader = DocumentReader::Create(config_file_name, DocumentType::Json);
		if (reader->ReadContents())
		{
			std::vector<HLString> faces;
			if (!reader->ReadStringArray("faces", faces))
			{
				HL_CORE_ERROR("Could not read faces from font config {0}!", **config_file_name);
				return false;
			}

			HLString font_file_name = "";
			if (!reader->ReadString("file", &font_file_name))
			{
				HL_CORE_ERROR("Invalid font file configuration! Could not find the file config in file {0}!", **config_file_name);
				return false;
			}

			if (font_file_name != filePath.Name())
			{
				HL_CORE_ERROR("Invalid font file configuration! Mismatching font file reference found! Expected {0}, but got {1}", *filePath.Name(), *font_file_name);
				return false;
			}

			if (faces.size() < 1)
			{
				HL_CORE_ERROR("At least expected 1 font face configuration!");
				return false;
			}

			TrueTypeFontFace *real_faces = (TrueTypeFontFace*)malloc(sizeof(TrueTypeFontFace) * faces.size());
			for (uint32 i = 0; i < (uint32)faces.size(); ++i)
			{
				real_faces[i].Name = faces[i].C_Str();
			}

			result.FontCount = (uint32)faces.size();
			result.Fonts = real_faces;
		}

		*out_result = result;

		if (!out_result->FontBinary || !out_result->Fonts)
		{
			HL_CORE_ERROR("Font {0} could not be loaded! Either could not load the binary itself or could not find any font config!", **filePath);
			return false;
		}

		HL_CORE_INFO("Successfully loaded font {0}", **filePath);
		return true;
	}
}

