// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Font.h"

#include "FontData.h"
#include "Engine/Core/FileSystem.h"
#include "Engine/Core/Allocator.h"

#define HL_FONT_DEFAULT_ANGLE_THRESHOLD 3.0
#define HL_FONT_DEFAULT_MITER_LIMIT 1.0
#define HL_FONT_LCG_MULTIPLIER 6364136223846793005ull
#define HL_FONT_LCG_INCREMENT 1442695040888963407ull
#define HL_FONT_THREADS 8
#define FONT_LOG_PREFIX "Font>         "

namespace highlo
{
	namespace utils
	{
		struct FontInput
		{
			HLString FontFileName;
			HLString CharsetFileName;
			HLString FontName;
			msdf_atlas::GlyphIdentifierType GlyphIdentifier;
			double FontScale;
		};

		struct AtlasHeader
		{
			uint32 Type = 0;
			uint32 Width;
			uint32 Height;
		};

		struct FontConfig
		{
			msdf_atlas::ImageType ImageType;
			msdf_atlas::ImageFormat ImageFormat;
			msdf_atlas::YDirection YDirection;
			int32 Width, Height;
			double EmSize;
			double PxRange;
			double AngleThreshold;
			double MiterLimit;
			void (*EdgeColoring)(msdfgen::Shape&, double, uint64);
			bool ExpensiveColoring;
			uint64 ColoringSeed;
			msdf_atlas::GeneratorAttributes GeneratorAttributes;
		};

		class FontHolder
		{
		public:

			FontHolder()
				: m_FTHandle(msdfgen::initializeFreetype()), m_FontHandle(nullptr), m_FontFileName(nullptr)
			{
			}

			~FontHolder()
			{
				if (m_FTHandle)
				{
					if (m_FontHandle)
						msdfgen::destroyFont(m_FontHandle);

					msdfgen::deinitializeFreetype(m_FTHandle);
				}
			}

			bool Load(const char *fileName)
			{
				if (m_FTHandle)
				{
					// Check if font is already loaded
					if (m_FontFileName && !strcmp(m_FontFileName, fileName))
						return true;

					// Destroy old font if it exists
					if (m_FontHandle)
						msdfgen::destroyFont(m_FontHandle);

					// Load font and store the handle and the fontname
					if ((m_FontHandle = msdfgen::loadFont(m_FTHandle, fileName)))
					{
						m_FontFileName = fileName;
						return true;
					}
				}

				return false;
			}

			operator msdfgen::FontHandle*() const
			{
				return m_FontHandle;
			}

		private:

			msdfgen::FreetypeHandle *m_FTHandle;
			msdfgen::FontHandle *m_FontHandle;
			const char *m_FontFileName;
		};

		static FileSystemPath GetCacheDirectory()
		{
			FileSystemPath path = "assets/cache/fonts/";

			if (!FileSystem::Get()->FolderExists(path))
			{
				FileSystem::Get()->CreateFolder(path);
			}

			return path;
		}

		static bool TryReadFontAtlasFromCacheDir(const HLString &fontName, float fontSize, AtlasHeader &header, void *&pixels, Allocator &storageBuffer)
		{
			HLString fileName = fmt::format("{0}-{1}.cachedFont", *fontName, fontSize);
			FileSystemPath p = utils::GetCacheDirectory() / fileName;

			HL_CORE_INFO("Trying to read cache file {0}", **p);
			if (FileSystem::Get()->FileExists(p))
			{
				HL_CORE_INFO("Found cache file {0}", **p);

				int64 dataSize;
				Byte *data = FileSystem::Get()->ReadFile(p, &dataSize);

				storageBuffer = Allocator::Copy(data, (uint32)dataSize);
				
				header = *storageBuffer.As<AtlasHeader>();
				pixels = (uint8*)storageBuffer.Data + sizeof(AtlasHeader);

				return true;
			}

			HL_CORE_WARN("Did not find cache file {0}, going to re-create font atlas...", **p);
			return false;
		}

		static void CacheFontAtlas(const HLString &fontName, float fontSize, AtlasHeader header, const void *pixels)
		{
			HLString fileName = fmt::format("{0}-{1}.cachedFont", *fontName, fontSize);
			FileSystemPath p = utils::GetCacheDirectory() / fileName;

			std::ofstream stream(**p, std::ios::binary | std::ios::trunc);
			if (!stream)
			{
				stream.close();
				HL_CORE_ERROR("Failed to cache the font atlas to location {0}", **p);
				return;
			}

			stream.write((char*)&header, sizeof(AtlasHeader));
			stream.write((char*)pixels, header.Width * header.Height * sizeof(float) * 4);
			stream.close();
		}

		template<typename T, typename S, int32 N, msdf_atlas::GeneratorFunction<S, N> GEN_FN>
		static Ref<Texture2D> CreateAndCacheAtlas(const HLString &fontName, 
												  float fontSize, 
												  const std::vector<msdf_atlas::GlyphGeometry> &glyphs, 
												  const msdf_atlas::FontGeometry &fontGeometry, 
												  const FontConfig &fontConfig)
		{
			msdf_atlas::ImmediateAtlasGenerator<S, N, GEN_FN, msdf_atlas::BitmapAtlasStorage<T, N>> generator(fontConfig.Width, fontConfig.Height);
			generator.setAttributes(fontConfig.GeneratorAttributes);
			generator.setThreadCount(HL_FONT_THREADS);
			generator.generate(glyphs.data(), (int32)glyphs.size());

			msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

			AtlasHeader header;
			header.Width = bitmap.width;
			header.Height = bitmap.height;
			CacheFontAtlas(fontName, fontSize, header, bitmap.pixels);

			TextureProperties properties;
			properties.DebugName = "FontAtlas";
			properties.GenerateMips = false;
			properties.SamplerWrap = TextureWrap::Clamp;
			return Texture2D::Create(TextureFormat::RGBA32F, header.Width, header.Height, bitmap.pixels, properties);
		}

		static Ref<Texture2D> CreateCachedAtlas(AtlasHeader header, const void *pixels)
		{
			TextureProperties properties;
			properties.DebugName = "FontAtlas";
			properties.GenerateMips = false;
			properties.SamplerWrap = TextureWrap::Clamp;
			return Texture2D::Create(TextureFormat::RGBA32F, header.Width, header.Height, pixels, properties);
		}
	}

	Font::Font(const FileSystemPath &path)
		: m_FilePath(path), m_MSDFData(new MSDFData())
	{
		static const uint32 charsetRanges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
			0x2DE0, 0x2DFF, // Cyrillic Extended-A
			0xA640, 0xA69F, // Cyrillic Extended-B
			0,
		};

		// General needed variables for font loading
		int32 result = 0;
		int32 fixedWidth = -1;
		int32 fixedHeight = -1;
		double minEmSize = 0.0;
		double rangeValue = 2.0;
		utils::FontInput fontInput = {};
		utils::FontConfig fontConfig = {};
		utils::FontHolder font;
		const char *imageFormatName = nullptr;
		msdf_atlas::TightAtlasPacker::DimensionsConstraint atlasSizeConstraint = msdf_atlas::TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE;
		msdf_atlas::Charset charset;

		fontInput.GlyphIdentifier = msdf_atlas::GlyphIdentifierType::UNICODE_CODEPOINT;
		fontInput.FontScale = -1;
		fontConfig.ImageType = msdf_atlas::ImageType::MSDF;
		fontConfig.ImageFormat = msdf_atlas::ImageFormat::BINARY_FLOAT;
		fontConfig.YDirection = msdf_atlas::YDirection::BOTTOM_UP;
		fontConfig.EdgeColoring = msdfgen::edgeColoringInkTrap;
		fontConfig.GeneratorAttributes.config.overlapSupport = true;
		fontConfig.GeneratorAttributes.scanlinePass = true;
		fontConfig.AngleThreshold = HL_FONT_DEFAULT_ANGLE_THRESHOLD;
		fontConfig.MiterLimit = HL_FONT_DEFAULT_MITER_LIMIT;
		fontConfig.ImageType = msdf_atlas::ImageType::MTSDF;
		fontConfig.EmSize = 40.0;

		HLString fontFilePath = m_FilePath.String();
		fontInput.FontFileName = fontFilePath.C_Str();

		// Load fonts
		bool anyCodepointsAvailable = false;
		if (!font.Load(fontInput.FontFileName))
		{
			HL_CORE_ERROR(FONT_LOG_PREFIX "[-] Could not load font {0} [-]", *fontInput.FontFileName);
			return;
		}

		if (fontInput.FontScale <= 0.0)
			fontInput.FontScale = 1.0;

		// Load character set
		for (uint32 range = 0; range < 8; range += 2)
		{
			for (uint32 c = charsetRanges[range]; c <= charsetRanges[range + 1]; ++c)
			{
				charset.add(c);
			}
		}

		// Load glyphs
		m_MSDFData->FontGeometry = msdf_atlas::FontGeometry(&m_MSDFData->Glyphs);
		int32 glyphsLoaded = -1;
		switch (fontInput.GlyphIdentifier)
		{
			case msdf_atlas::GlyphIdentifierType::GLYPH_INDEX:
				glyphsLoaded = m_MSDFData->FontGeometry.loadGlyphset(font, fontInput.FontScale, charset);
				break;

			case msdf_atlas::GlyphIdentifierType::UNICODE_CODEPOINT:
				glyphsLoaded = m_MSDFData->FontGeometry.loadCharset(font, fontInput.FontScale, charset);
				anyCodepointsAvailable |= glyphsLoaded > 0;
				break;
		}

		if (glyphsLoaded < 0)
		{
			HL_ASSERT(false, "No glyphs could be loaded!");
		}

		HL_CORE_INFO("Loaded geometry of {0} out of {1} glyphs!", glyphsLoaded, (int32)charset.size());

		// Print missing glyphs
		if (glyphsLoaded < (int32)charset.size())
		{
			HL_CORE_WARN("Missing {0} {1}", (int32)charset.size() - glyphsLoaded, fontInput.GlyphIdentifier == msdf_atlas::GlyphIdentifierType::UNICODE_CODEPOINT ? "codepoints" : "glyphs");
		}

		if (fontInput.FontName)
			m_MSDFData->FontGeometry.setName(*fontInput.FontName);

		double pxRange = rangeValue;
		bool fixedDimensions = fixedWidth >= 0 && fixedHeight >= 0;
		bool fixedScale = fontConfig.EmSize > 0.0;
		bool floatingPointFormat = true;
		msdf_atlas::TightAtlasPacker atlasPacker;

		if (fixedDimensions)
			atlasPacker.setDimensions(fixedWidth, fixedHeight);
		else
			atlasPacker.setMinimumScale(minEmSize);

		atlasPacker.setPadding(fontConfig.ImageType == msdf_atlas::ImageType::MSDF || fontConfig.ImageType == msdf_atlas::ImageType::MTSDF ? 0 : -1);

		if (fixedScale)
			atlasPacker.setScale(fontConfig.EmSize);
		else
			atlasPacker.setScale(minEmSize);

		atlasPacker.setPixelRange(pxRange);
		atlasPacker.setMiterLimit(fontConfig.MiterLimit);

		if (int32 remaining = atlasPacker.pack(m_MSDFData->Glyphs.data(), (int32)m_MSDFData->Glyphs.size()))
		{
			if (remaining < 0)
			{
				HL_CORE_ERROR("Remaining {0}", remaining);
				HL_ASSERT(false);
			}
			else
			{
				HL_CORE_ERROR("Error: Could not fit {0} out of {1} glyphs into the atlas.", remaining, (int32)m_MSDFData->Glyphs.size());
				HL_ASSERT(false);
			}
		}

		atlasPacker.getDimensions(fontConfig.Width, fontConfig.Height);
		HL_ASSERT(fontConfig.Width > 0 && fontConfig.Height > 0);

		fontConfig.EmSize = atlasPacker.getScale();
		fontConfig.PxRange = atlasPacker.getPixelRange();

		if (!fixedScale)
		{
			HL_CORE_TRACE("Glyph size: {0} pixels/EM", fontConfig.EmSize);
		}

		if (!fixedDimensions)
		{
			HL_CORE_TRACE("Atlas dimensions: {0}x{1}", fontConfig.Width, fontConfig.Height);
		}

		// Edge coloring
		if (fontConfig.ImageType == msdf_atlas::ImageType::MSDF || fontConfig.ImageType == msdf_atlas::ImageType::MTSDF)
		{
			if (fontConfig.ExpensiveColoring)
			{
				msdf_atlas::Workload([&glyphs = m_MSDFData->Glyphs, &fontConfig](int32 i, int32 threadNo) -> bool
				{
					uint64 glyphSeed = (HL_FONT_LCG_MULTIPLIER * (fontConfig.ColoringSeed ^ i) + HL_FONT_LCG_INCREMENT) * !!fontConfig.ColoringSeed;
					glyphs[i].edgeColoring(fontConfig.EdgeColoring, fontConfig.AngleThreshold, glyphSeed);
					return true;
				}, (int32)m_MSDFData->Glyphs.size()).finish(HL_FONT_THREADS);
			}
			else
			{
				uint64 glyphSeed = fontConfig.ColoringSeed;
				for (msdf_atlas::GlyphGeometry &glyph : m_MSDFData->Glyphs)
				{
					glyphSeed *= HL_FONT_LCG_MULTIPLIER;
					glyph.edgeColoring(fontConfig.EdgeColoring, fontConfig.AngleThreshold, glyphSeed);
				}
			}
		}

		// Check the font cache
		HLString fontName = m_FilePath.Filename();
		Allocator storageBuffer;
		utils::AtlasHeader header;
		void *pixels;

		if (utils::TryReadFontAtlasFromCacheDir(fontName, (float)fontConfig.EmSize, header, pixels, storageBuffer))
		{
			m_TextureAtlas = utils::CreateCachedAtlas(header, pixels);
			storageBuffer.Release();
		}
		else
		{
			switch (fontConfig.ImageType)
			{
				case msdf_atlas::ImageType::MSDF:
				{
					if (floatingPointFormat)
					{
						m_TextureAtlas = utils::CreateAndCacheAtlas<float, float, 3, msdf_atlas::msdfGenerator>(fontName, 
																												(float)fontConfig.EmSize, 
																												m_MSDFData->Glyphs, m_MSDFData->FontGeometry, 
																												fontConfig);
					}
					else
					{
						m_TextureAtlas = utils::CreateAndCacheAtlas<Byte, float, 3, msdf_atlas::msdfGenerator>(fontName,
																											   (float)fontConfig.EmSize,
																											   m_MSDFData->Glyphs, m_MSDFData->FontGeometry,
																											   fontConfig);
					}

					break;
				}

				case msdf_atlas::ImageType::MTSDF:
				{
					if (floatingPointFormat)
					{
						m_TextureAtlas = utils::CreateAndCacheAtlas<float, float, 4, msdf_atlas::mtsdfGenerator>(fontName,
																												 (float)fontConfig.EmSize,
																												 m_MSDFData->Glyphs, m_MSDFData->FontGeometry,
																												 fontConfig);
					}
					else
					{
						m_TextureAtlas = utils::CreateAndCacheAtlas<Byte, float, 4, msdf_atlas::mtsdfGenerator>(fontName,
																												(float)fontConfig.EmSize,
																												m_MSDFData->Glyphs, m_MSDFData->FontGeometry,
																												fontConfig);
					}

					break;
				}
			}
		}
	}

	Font::~Font()
	{
		delete m_MSDFData;
	}

	Ref<Font> Font::Create(const FileSystemPath &path)
	{
		return Ref<Font>::Create(path);
	}
}

