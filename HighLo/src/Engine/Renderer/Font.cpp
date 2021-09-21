// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Font.h"

#include "FontData.h"

#define HL_FONT_DEFAULT_ANGLE_THRESHOLD 3.0
#define HL_FONT_DEFAULT_MITER_LIMIT 1.0
#define HL_FONT_LCG_MULTIPLIER 6364136223846793005ull
#define HL_FONT_LCG_INCREMENT 1442695040888963407ull
#define HL_FONT_THREADS 8


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
				if (m_FTHandle && m_FontHandle)
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

		template<typename T, typename S, int32 N, msdf_atlas::GeneratorFunction<S, N> GEN_FUNC>
		static Ref<Texture2D> MakeAtlas(const std::vector<msdf_atlas::GlyphGeometry> &glyphs, const msdf_atlas::FontGeometry &fontGeo, const FontConfig &config)
		{
			msdf_atlas::ImmediateAtlasGenerator<S, N, GEN_FUNC, msdf_atlas::BitmapAtlasStorage<T, N>> generator(config.Width, config.Height);
			generator.setAttributes(config.GeneratorAttributes);
			generator.setThreadCount(HL_FONT_THREADS);
			generator.generate(glyphs.data(), (int32)glyphs.size());

			msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();
			TextureProperties props;
			props.GenerateMips = false;
			props.SamplerWrap = TextureWrap::Clamp;
			Ref<Texture2D> texture = Texture2D::Create(TextureFormat::RGBA32F, bitmap.width, bitmap.height, bitmap.pixels, props);
			return texture;
		}
	}

	static Ref<Font> s_DefaultFont;

	Font::Font(const FileSystemPath &path)
		: m_FilePath(path), m_MSDFData(new MSDFData())
	{
		static const uint32_t charsetImGuiRanges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
			0x2DE0, 0x2DFF, // Cyrillic Extended-A
			0xA640, 0xA69F, // Cyrillic Extended-B
			0,
		};

		int32 result = 0;
		int32 fixedWidth = -1, fixedHeight = -1;
		int32 glyphsLoaded = -1;
		double minEmSize = 0.0;
		double pxRange = 2.0;
		bool fixedDimensions = fixedWidth >= 0 && fixedHeight >= 0;
		bool anyCodepointsAvailable = false;
		bool floatingPointFormat = true;
		
		msdf_atlas::TightAtlasPacker::DimensionsConstraint atlasSizeConstraint = msdf_atlas::TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE;
		msdf_atlas::Charset charset;
		msdf_atlas::TightAtlasPacker atlasPacker;

		utils::FontInput fontInput = {};
		fontInput.GlyphIdentifier = msdf_atlas::GlyphIdentifierType::UNICODE_CODEPOINT;
		fontInput.FontScale = -1;
		fontInput.FontFileName = m_FilePath.String();

		utils::FontConfig config = {};
		//config.ImageType = msdf_atlas::ImageType::MSDF;
		config.ImageType = msdf_atlas::ImageType::MTSDF;
		config.ImageFormat = msdf_atlas::ImageFormat::BINARY_FLOAT;
		config.YDirection = msdf_atlas::YDirection::BOTTOM_UP;
		config.EdgeColoring = msdfgen::edgeColoringInkTrap;
		config.GeneratorAttributes.config.overlapSupport = true;
		config.GeneratorAttributes.scanlinePass = true;
		config.AngleThreshold = HL_FONT_DEFAULT_ANGLE_THRESHOLD;
		config.MiterLimit = HL_FONT_DEFAULT_MITER_LIMIT;
		config.EmSize = 40.0;

		utils::FontHolder font;
		if (!font.Load(*fontInput.FontFileName))
			HL_ASSERT(false);

		if (fontInput.FontScale <= 0.0)
			fontInput.FontScale = 1.0;

		// Load character set
		for (uint32 range = 0; range < 8; range += 2)
		{
			for (uint32 c = charsetImGuiRanges[range]; c <= charsetImGuiRanges[range + 1]; ++c)
				charset.add(c);
		}

		// Load glyphs
		m_MSDFData->FontGeometry = msdf_atlas::FontGeometry(&m_MSDFData->Glyphs);
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

		HL_ASSERT(glyphsLoaded >= 0);
		HL_CORE_TRACE("Loaded Font Geometry of {0} out of {1} glyphs.", glyphsLoaded, (int32)charset.size());

		// Check if any glyphs are missing
		if (glyphsLoaded < (int32)charset.size())
			HL_CORE_WARN("Missing {0} of {1} {2}", (int32)charset.size() - glyphsLoaded, (int32)charset.size(), fontInput.GlyphIdentifier == msdf_atlas::GlyphIdentifierType::UNICODE_CODEPOINT ? "codepoints" : "glyphs");
	
		if (fontInput.FontName)
			m_MSDFData->FontGeometry.setName(*fontInput.FontName);

		// Determine final atlas dimensions, scale and range, pack glyphs
		bool fixedScale = config.EmSize > 0;

		if (fixedDimensions)
			atlasPacker.setDimensions(fixedWidth, fixedHeight);
		else
			atlasPacker.setDimensionsConstraint(atlasSizeConstraint);

		// TODO: In this case (if padding is -1), the border pixels of each glyph are black, but still computed. For floating-point output, this may play a role.
		atlasPacker.setPadding(config.ImageType == msdf_atlas::ImageType::MSDF || config.ImageType == msdf_atlas::ImageType::MTSDF ? 0 : -1);

		if (fixedScale)
			atlasPacker.setScale(config.EmSize);
		else
			atlasPacker.setMinimumScale(minEmSize);

		atlasPacker.setPixelRange(pxRange);
		atlasPacker.setMiterLimit(config.MiterLimit);

		if (int32 remaining = atlasPacker.pack(m_MSDFData->Glyphs.data(), (int32)m_MSDFData->Glyphs.size()))
		{
			if (remaining < 0)
			{
				HL_ASSERT(false);
			}
			else if (remaining > 0)
			{
				HL_CORE_ERROR("Error: Could not fit {0} out of {1} glyphs into the texture atlas.", remaining, (int32)m_MSDFData->Glyphs.size());
				HL_ASSERT(false);
			}
		}

		atlasPacker.getDimensions(config.Width, config.Height);
		HL_ASSERT(config.Width > 0 && config.Height > 0);
		config.EmSize = atlasPacker.getScale();
		config.PxRange = atlasPacker.getPixelRange();

		if (!fixedScale)
			HL_CORE_TRACE("Glyph size: {0} pixels/EM", config.EmSize);

		if (!fixedDimensions)
			HL_CORE_TRACE("Atlas Dimensions: {0}x{1}", config.Width, config.Height);

		// Edge Coloring

		if (config.ImageType == msdf_atlas::ImageType::MSDF || config.ImageType == msdf_atlas::ImageType::MTSDF)
		{
			if (config.ExpensiveColoring)
			{
				msdf_atlas::Workload([&glyphs = m_MSDFData->Glyphs, &config](int32 index, int32 threadNo) -> bool
				{
					uint64 glyphSeed = (HL_FONT_LCG_MULTIPLIER * (config.ColoringSeed ^ index) + HL_FONT_LCG_INCREMENT) * !!config.ColoringSeed;
					glyphs[index].edgeColoring(config.EdgeColoring, config.AngleThreshold, glyphSeed);
					return true;
				}, (int32)m_MSDFData->Glyphs.size()).finish(HL_FONT_THREADS);
			}
			else
			{
				uint64 glyphSeed = config.ColoringSeed;
				for (msdf_atlas::GlyphGeometry &glyph : m_MSDFData->Glyphs)
				{
					glyphSeed *= HL_FONT_LCG_MULTIPLIER;
					glyph.edgeColoring(config.EdgeColoring, config.AngleThreshold, glyphSeed);
				}
			}
		}

		// Load Texture Atlas

		Ref<Texture2D> texture;
		switch (config.ImageType)
		{
			case msdf_atlas::ImageType::MSDF:
			{
				if (floatingPointFormat)
					texture = utils::MakeAtlas<float, float, 3, msdf_atlas::msdfGenerator>(m_MSDFData->Glyphs, m_MSDFData->FontGeometry, config);
				else
					texture = utils::MakeAtlas<msdf_atlas::byte, float, 3, msdf_atlas::msdfGenerator>(m_MSDFData->Glyphs, m_MSDFData->FontGeometry, config);

				break;
			}

			case msdf_atlas::ImageType::MTSDF:
			{
				if (floatingPointFormat)
					texture = utils::MakeAtlas<float, float, 4, msdf_atlas::mtsdfGenerator>(m_MSDFData->Glyphs, m_MSDFData->FontGeometry, config);
				else
					texture = utils::MakeAtlas<msdf_atlas::byte, float, 4, msdf_atlas::mtsdfGenerator>(m_MSDFData->Glyphs, m_MSDFData->FontGeometry, config);

				break;
			}
		}

		m_TextureAtlas = texture;
	}

	Font::~Font()
	{
		delete m_MSDFData;
	}

	void Font::Init()
	{
		s_DefaultFont = Font::Create(FileSystemPath("assets/fonts/BarlowSemiCondensedFontFamily/BarlowSemiCondensed-Black.ttf"));
	}

	Ref<Font> Font::GetDefaultFont()
	{
		return s_DefaultFont;
	}

	Ref<Font> Font::Create(const FileSystemPath &path)
	{
		return Ref<Font>::Create(path);
	}
}

