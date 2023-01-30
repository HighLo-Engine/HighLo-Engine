// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "TrueTypeFont.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Loaders/FontLoader.h"

namespace highlo
{
	TrueTypeFont::TrueTypeFont(const FileSystemPath &filePath, uint16 size)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();

		// TODO: Make this configurable. Hold space for 256 fonts
		uint32 font_count = 256;
		for (uint32 i = 0; i < font_count; ++i)
		{
			TrueTypeFontData data;
			data.ID = HL_INVALID_ID_U16;
			m_Fonts.push_back(data);
		}

		uint16 invalid_id = HL_INVALID_ID_U16;
		m_FontIds.Resize(font_count);
		m_FontIds.Fill(&invalid_id);

		LoadFont(size);
	}

	TrueTypeFont::~TrueTypeFont()
	{
	}
	
	bool TrueTypeFont::VerifyAtlas(FontData *font, const HLString &text)
	{
		if (font->Type == FontType::TRUE_TYPE_FONT)
		{
			uint16 id = HL_INVALID_ID_U16;
			if (!m_FontIds.Get(font->Face, &id))
			{
				HL_CORE_ERROR("[VerifyAtlas] Could not retrieve a valid id from the font hashtable. Aborting.");
				return false;
			}

			if (id == HL_INVALID_ID_U16)
			{
				HL_CORE_ERROR("Could not find a valid id. Aborting.");
				return false;
			}

			TrueTypeFontData data = m_Fonts[id];
			return VerifyFontSizeVariant(data, font, text);
		}
		
		HL_CORE_ERROR("You tried to use this function with the wrong font data. The font data must be of type TRUE_TYPE_FONT.");
		return false;
	}

	bool TrueTypeFont::LoadFont(uint32 default_size)
	{
		TrueTypeFontLoaderResult font_data = {};
		if (!FontLoader::LoadTrueTypeFont(m_AssetPath, &font_data))
		{
			// already logged the error inside LoadTrueTypeFont
			return false;
		}

		for (uint32 i = 0; i < font_data.FontCount; ++i)
		{
			TrueTypeFontFace *face = &font_data.Fonts[i];

			uint16 id = HL_INVALID_ID_U16;
			m_FontIds.Get(face->Name, &id);

			if (id != HL_INVALID_ID_U16)
			{
				HL_CORE_WARN("A font named {0} already exists and will not be loaded again.", *m_Name);
				return true;
			}

			// Retrieve a new id
			for (uint16 j = 0; j < (uint16)m_Fonts.size(); ++j)
			{
				if (m_Fonts[j].ID == HL_INVALID_ID_U16)
				{
					id = j;
					break;
				}
			}

			if (id == HL_INVALID_ID_U16)
			{
				HL_CORE_ERROR("No space left to allocate a new font!");
				return false;
			}

			TrueTypeFontData font = {};
			font.BinarySize = font_data.BinarySize;
			font.FontBinary = font_data.FontBinary;
			font.Face = face->Name;
			font.Index = i;
			font.SizeVariants = std::vector<FontData>();
			font.ID = id;

			// The offset
			font.Offset = stbtt_GetFontOffsetForIndex(font.FontBinary, i);
			int32 fontInitResult = stbtt_InitFont(&font.Info, font.FontBinary, font.Offset);
			if (fontInitResult == 0)
			{
				HL_CORE_ERROR("Failed to init True type font! for font {0} at index {1}", **m_AssetPath, i);
				return false;
			}

			FontData variant;
			if (!CreateFontVariant(font, default_size, face->Name, &variant))
			{
				HL_CORE_ERROR("Failed to create variant: {0} at index {1}", face->Name, i);
				continue;
			}

			if (!SetupFontData(&variant, default_size))
			{
				HL_CORE_ERROR("Failed to setup font data!");
				continue;
			}

			font.SizeVariants.push_back(variant);

			if (!m_FontIds.Set(face->Name, &id))
			{
				HL_CORE_ERROR("Could not insert the id {0} into the font ids hashtable!", id);
				return false;
			}

			m_Fonts[id] = font;
		}

		// Cleanup the data loaded from the FontLoader
		for (uint32 i = 0; i < font_data.FontCount; ++i)
		{
			TrueTypeFontFace *face = &font_data.Fonts[i];
			free(face);
			face = nullptr;
		}

		return true;
	}

	bool TrueTypeFont::SetupFontData(FontData *variant, uint16 font_size)
	{
		// Check for a tab and space glyph
		if (!variant->TabXAdvance)
		{
			// Search for an existing tab codepoint
			for (uint32 i = 0; i < (uint32)m_Glyphs.size(); ++i)
			{
				if (m_Glyphs[i].Codepoint == '\t')
				{
					variant->TabXAdvance = m_Glyphs[i].XAdvance;
					break;
				}
			}

			// If no codepoint was found, then use the space codepoint * 4
			// because a tab can also be represented by four spaces
			if (!variant->TabXAdvance)
			{
				for (uint32 i = 0; i < (uint32)m_Glyphs.size(); ++i)
				{
					if (m_Glyphs[i].Codepoint == ' ')
					{
						variant->TabXAdvance = (float)(m_Glyphs[i].XAdvance * 4);
						break;
					}
				}

				if (!variant->TabXAdvance)
				{
					// This should never be thrown, so warn 
					HL_CORE_WARN("Could neither find a tab or space codepoint!");

					// But we still need a valid value, so set a default
					variant->TabXAdvance = (float)(font_size * 4);
				}
			}
		}

		return true;
	}

	void TrueTypeFont::CleanupFontData(FontData *variant)
	{
		free(variant->InternalData);
		variant->InternalData = nullptr;

		// just decrement the refcount to the texture
		variant->Atlas = nullptr;
	}

	bool TrueTypeFont::CreateFontVariant(TrueTypeFontData data, uint16 size, const HLString &font_name, FontData *out_variant)
	{
		memset(out_variant, 0, sizeof(out_variant));
		out_variant->AtlasSizeX = 1024; // TODO: Make configurable
		out_variant->AtlasSizeY = 1024;
		out_variant->Size = size;
		out_variant->Type = FontType::TRUE_TYPE_FONT;
		out_variant->Face = font_name;
		out_variant->InternalDataSize = sizeof(TrueTypeVariantData);
		out_variant->InternalData = malloc(out_variant->InternalDataSize);

		TrueTypeVariantData *internal_data = (TrueTypeVariantData*)out_variant->InternalData;
		
		TrueTypeVariantData codepoint_data = {};
		codepoint_data.Scale = 0.0f;
		codepoint_data.Codepoints = std::vector<int32>(96);

		codepoint_data.Codepoints[0] = -1;
		for (int32 i = 0; i < 95; ++i)
		{
			codepoint_data.Codepoints[i + 1] = i + 32;
		}

		internal_data = &codepoint_data;

		TextureSpecification spec;
		spec.Format = TextureFormat::RGBA;
		spec.Width = (uint32)out_variant->AtlasSizeX;
		spec.Height = (uint32)out_variant->AtlasSizeY;
		spec.Properties.SamplerFilter = TextureFilter::Linear;
		spec.Properties.SamplerWrap = TextureWrap::Clamp;
		spec.Usage = TextureUsage::FontAtlas;
		out_variant->Atlas = Texture2D::CreateFromSpecification(spec);
		if (!out_variant->Atlas)
		{
			HL_CORE_ERROR("Could not create texture font atlas!");
			return false;
		}

		internal_data->Scale = stbtt_ScaleForPixelHeight(&data.Info, (float)size);
		int32 ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&data.Info, &ascent, &descent, &line_gap);
		out_variant->LineHeight = (int32)((ascent - descent + line_gap) * internal_data->Scale);

		return RebuildFontVariantAtlas(data, out_variant);
	}

	bool TrueTypeFont::RebuildFontVariantAtlas(TrueTypeFontData data, FontData *variant)
	{
		TrueTypeVariantData *internal_data = (TrueTypeVariantData*)variant->InternalData;

		uint32 pack_image_size = variant->AtlasSizeX * variant->AtlasSizeY * sizeof(uint8);
		uint8 *pixels = (uint8*)malloc(pack_image_size);
		uint32 codepoint_count = (uint32)internal_data->Codepoints.size();
		stbtt_packedchar *packed_chars = (stbtt_packedchar*)malloc(sizeof(stbtt_packedchar) * codepoint_count);

		stbtt_pack_context context;
		if (!stbtt_PackBegin(&context, pixels, variant->AtlasSizeX, variant->AtlasSizeY, 0, 1, nullptr))
		{
			HL_CORE_ERROR("stbtt_PackBegin failed.");
			return false;
		}

		// Fit all codepoints into a single range for packing
		stbtt_pack_range range;
		range.first_unicode_codepoint_in_range = 0;
		range.font_size = variant->Size;
		range.num_chars = codepoint_count;
		range.chardata_for_range = packed_chars;
		range.array_of_unicode_codepoints = internal_data->Codepoints.data();
		if (!stbtt_PackFontRanges(&context, data.FontBinary, data.Index, &range, 1))
		{
			HL_CORE_ERROR("stbtt_PackFontRanges failed.");
			return false;
		}

		// Packing ended
		stbtt_PackEnd(&context);

		// Convert from single-channel to RGBA, or pack_image_size * 4.
		uint8 *rgba_pixels = (uint8*)malloc(pack_image_size * 4);
		for (int32 j = 0; j < (int32)pack_image_size; ++j)
		{
			rgba_pixels[(j * 4) + 0] = pixels[j];
			rgba_pixels[(j * 4) + 1] = pixels[j];
			rgba_pixels[(j * 4) + 2] = pixels[j];
			rgba_pixels[(j * 4) + 3] = pixels[j];
		}


		// Store the pixel values inside the texture class
		variant->Atlas->SetData((void*)rgba_pixels, pack_image_size * 4);
		free(rgba_pixels);
		free(pixels);

		// Regenerate glyphs
		if (m_Glyphs.size() > 0)
		{
			m_Glyphs.clear();
		}

		m_Glyphs.resize(codepoint_count);
		for (uint16 i = 0; i < (uint16)codepoint_count; ++i)
		{
			stbtt_packedchar *pc = &packed_chars[i];
			FontGlyph &g = m_Glyphs[i];
			
			g.Codepoint = internal_data->Codepoints[i];
			g.PageId = 0; // TODO: Could be extended in the future
			g.XOffset = pc->xoff;
			g.YOffset = pc->yoff;
			g.X = pc->x0;
			g.Y = pc->y0;
			g.Width = pc->x1 - pc->x0;
			g.Height = pc->y1 - pc->y0;
			g.XAdvance = pc->xadvance;
		}

		// Regenerate kernings
		if (m_Kernings.size() > 0)
		{
			m_Kernings.clear();
		}

		int32 kerning_count = stbtt_GetKerningTableLength(&data.Info);
		if (kerning_count > 0)
		{
			stbtt_kerningentry *kerning_table = (stbtt_kerningentry*)malloc(sizeof(stbtt_kerningentry) * kerning_count);
			int32 entry_count = stbtt_GetKerningTable(&data.Info, kerning_table, kerning_count);
			if (entry_count != kerning_count)
			{
				free(kerning_table);
				HL_CORE_ERROR("Kerning entry count mismatch! {0} -> {1}", entry_count, kerning_count);
				return false;
			}

			m_Kernings.resize(kerning_count);
			for (uint32 i = 0; i < (uint32)kerning_count; ++i)
			{
				FontKerning &k = m_Kernings[i];
				k.Codepoint1 = kerning_table[i].glyph1;
				k.Codepoint2 = kerning_table[i].glyph2;
				k.Amount = kerning_table[i].advance;
			}

			free(kerning_table);
		}

		return true;
	}

	bool TrueTypeFont::VerifyFontSizeVariant(TrueTypeFontData data, FontData *variant, const HLString &text)
	{
		TrueTypeVariantData *internal_data = (TrueTypeVariantData*)variant->InternalData;

		uint32 char_length = text.Length();
		uint32 added_codepoint_count = 0;
		for (uint32 i = 0; i < char_length;)
		{
			int32 codepoint;
			uint8 advance;
			if (!HLStringUTF8::FromString(text, i, &codepoint, &advance))
			{
				HL_CORE_ERROR("Failed to get the current codepoint for character {0}", text.At(i));
				++i;
				continue;
			}
			else
			{
				i += advance;
				if (codepoint < 128)
				{
					// ascii codepoints will always be included by default, so we can skip them
					continue;
				}

				uint32 codepoint_count = HLStringUTF8::UTF8StringLength(internal_data->Codepoints.data());
				bool found = false;
				for (uint32 j = 95; j < codepoint_count; ++j)
				{
					if (internal_data->Codepoints[j] == codepoint)
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					internal_data->Codepoints.push_back(codepoint);
					++added_codepoint_count;
				}
			}
		}

		if (added_codepoint_count > 0)
		{
			return RebuildFontVariantAtlas(data, variant);
		}

		return true;
	}

	const FontData *TrueTypeFont::GetDefaultFontFace() const
	{
		if (m_Fonts.size() > 0)
		{
			if (m_Fonts[0].SizeVariants.size() > 0)
			{
				return &m_Fonts[0].SizeVariants[0];
			}
		}

		return nullptr;
	}

	const Ref<Texture2D> &TrueTypeFont::GetAtlas() const
	{ 
		return GetDefaultFontFace()->Atlas; 
	}
	
	int32 TrueTypeFont::GetAtlasSizeX() const 
	{
		return GetDefaultFontFace()->AtlasSizeX;
	}

	int32 TrueTypeFont::GetAtlasSizeY() const
	{
		return GetDefaultFontFace()->AtlasSizeY;
	}

	int32 TrueTypeFont::GetLineHeight() const 
	{
		return GetDefaultFontFace()->LineHeight; 
	}
	
	float TrueTypeFont::GetTabXAdvance() const 
	{
		return GetDefaultFontFace()->TabXAdvance; 
	}
}

