// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Renderer2D.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "FontData.h"

#include "Engine/Utils/StringUtils.h"

namespace highlo
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// TODO: make this editor-only
		int32 EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// TODO: make this editor-only
		int32 EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		float Thickness;
		glm::vec2 LocalPosition;
		glm::vec4 Color;

		// TODO: make this editor-only
		int32 EntityID;
	};

	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
	};

	struct Renderer2DData
	{
		static const uint32 MaxQuads = 20000;
		static const uint32 MaxVertices = MaxQuads * 4;
		static const uint32 MaxIndices = MaxQuads * 6;
		static const uint32 MaxTextureSlots = 32; // TODO: THIS is platform dependent, so get it from the RenderingAPI

		static const uint32 MaxLines = 10000;
		static const uint32 MaxLineVertices = MaxLines * 2;
		static const uint32 MaxLineIndices = MaxLines * 6;

		Ref<Texture2D> WhiteTexture;

		// Quads
		Ref<Shader> TextureShader;
		Ref<VertexArray> QuadVertexArray;
		uint32 QuadIndexCount = 0;
		QuadVertex *QuadVertexBufferBase = nullptr;
		QuadVertex *QuadVertexBufferPtr = nullptr;

		// Lines
		Ref<Shader> LineShader;
		Ref<VertexArray> LineVertexArray;
		LineVertex *LineVertexBufferBase = nullptr;
		LineVertex *LineVertexBufferPtr = nullptr;
		uint32 LineIndexCount = 0;

		// Circles
		Ref<Shader> CircleShader;
		Ref<VertexArray> CircleVertexArray;
		CircleVertex *CircleVertexBufferBase = nullptr;
		CircleVertex *CircleVertexBufferPtr = nullptr;
		uint32 CircleIndexCount = 0;

		// Text
		Ref<VertexArray> TextVertexArray;
		Ref<Material> TextMaterial;
		std::array<Ref<Texture2D>, MaxTextureSlots> FontTextureSlots;
		uint32 FontTextureSlotIndex = 0;

		uint32 TextIndexCount = 0;
		TextVertex *TextVertexBufferBase = nullptr;
		TextVertex *TextVertexBufferPtr = nullptr;

		// Textures
		uint32 TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		glm::vec4 QuadVertexPositions[4];
		Ref<RenderPass> RenderPass;

		bool DepthTest = true;
		glm::mat4 CameraProjection = glm::mat4(1.0f);
	};

	static Renderer2DData *s_2DData;

	void Renderer2D::Init()
	{
		HL_PROFILE_FUNCTION();

		s_2DData = new Renderer2DData();

		// Set all texture slots to 0
		s_2DData->WhiteTexture = Renderer::GetWhiteTexture();
		s_2DData->TextureSlots[0] = s_2DData->WhiteTexture;

		s_2DData->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[1] = { -0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[3] = {  0.5f, -0.5f, 0.0f, 1.0f };

		static auto cameraBuffer = UniformBuffer::Create(
			"Camera",
			{ UniformVariable("u_ViewProjection", sizeof(glm::mat4)) },
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32) HL_UB_SLOT::VS_CAMERA_BUFFER);

		std::vector<int32> quadIndices;
		uint32 offset = 0;
		for (uint32 i = 0; i < s_2DData->MaxIndices; ++i)
		{
			quadIndices.push_back(offset + 0);
			quadIndices.push_back(offset + 1);
			quadIndices.push_back(offset + 2);

			quadIndices.push_back(offset + 2);
			quadIndices.push_back(offset + 3);
			quadIndices.push_back(offset + 0);

			offset += 4;
		}

		std::vector<int32> lineIndices;
		for (uint32 i = 0; i < s_2DData->MaxLineIndices; ++i)
		{
			lineIndices.push_back(i);
		}

		// Textures/Quads
		s_2DData->TextureShader = Renderer::GetShaderLibrary()->Get("Renderer2DQuad");
		s_2DData->TextureShader->AddBuffer("CameraBuffer", cameraBuffer);
		s_2DData->QuadVertexBufferBase = new QuadVertex[s_2DData->MaxVertices];

		s_2DData->QuadVertexArray = VertexArray::Create();
		Ref<VertexBuffer> quadVbo = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(QuadVertex));
		quadVbo->SetLayout(BufferLayout::GetTextureLayout());
		s_2DData->QuadVertexArray->AddVertexBuffer(quadVbo);
		s_2DData->QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(&quadIndices[0], s_2DData->MaxIndices));

		// Lines
		s_2DData->LineShader = Renderer::GetShaderLibrary()->Get("Renderer2DLine");
		s_2DData->LineShader->AddBuffer("CameraBuffer", cameraBuffer);
		s_2DData->LineVertexBufferBase = new LineVertex[s_2DData->MaxLineVertices];

		s_2DData->LineVertexArray = VertexArray::Create();
		Ref<VertexBuffer> lineVbo = VertexBuffer::Create(s_2DData->MaxLineVertices * sizeof(LineVertex));
		lineVbo->SetLayout(BufferLayout::GetLineLayout());
		s_2DData->LineVertexArray->AddVertexBuffer(lineVbo);
		s_2DData->LineVertexArray->SetIndexBuffer(IndexBuffer::Create(&lineIndices[0], s_2DData->MaxLineIndices));

		// Circles
		s_2DData->CircleShader = Renderer::GetShaderLibrary()->Get("Renderer2DCircle");
		s_2DData->CircleShader->AddBuffer("CameraBuffer", cameraBuffer);
		s_2DData->CircleVertexBufferBase = new CircleVertex[s_2DData->MaxVertices]; // TODO: check maxVertices

		/*
		s_2DData->CircleVertexArray = VertexArray::Create();
		Ref<VertexBuffer> circleVbo = VertexBuffer::Create();
		circleVbo->SetLayout(BufferLayout::GetCircleLayout());
		s_2DData->CircleVertexArray->AddVertexBuffer(circleVbo);
		s_2DData->CircleVertexArray->SetIndexBuffer(IndexBuffer::Create());
		*/
	}

	void Renderer2D::Shutdown()
	{
		HL_PROFILE_FUNCTION();

		delete[] s_2DData->QuadVertexBufferBase;
		delete[] s_2DData->LineVertexBufferBase;
		delete[] s_2DData->CircleVertexBufferBase;

		delete s_2DData;
	}

	void Renderer2D::BeginScene(const glm::mat4 &proj, bool depthTest)
	{
		HL_PROFILE_FUNCTION();

		s_2DData->CameraProjection = proj;
		s_2DData->DepthTest = depthTest;

		s_2DData->TextureShader->Bind();
		Ref<UniformBuffer> buffer = s_2DData->TextureShader->GetBuffer("CameraBuffer");
		buffer->SetBufferValue(&s_2DData->CameraProjection);
		buffer->UploadToShader();

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		HL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
	{
		HL_PROFILE_FUNCTION();

		if (s_2DData->QuadIndexCount == 0)
			return;

		// Disable warning "Conversion from __int64 to uint32 - possible loss of data"
	#pragma warning(push)
	#pragma warning(disable : 4244)
		uint32 dataSize = (uint8*)s_2DData->QuadVertexBufferPtr - (uint8*)s_2DData->QuadVertexBufferBase;
	#pragma warning(pop)
		if (dataSize)
		{
			s_2DData->TextureShader->Bind();
			s_2DData->QuadVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->QuadVertexBufferBase, dataSize);
			
			for (uint32 i = 0; i < s_2DData->TextureSlotIndex; ++i)
			{
				s_2DData->TextureSlots[i]->Bind(i);
			}

			s_2DData->QuadVertexArray->GetVertexBuffers()[0]->Bind();
			s_2DData->QuadVertexArray->GetIndexBuffer()->Bind();
			s_2DData->QuadVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->QuadIndexCount, PrimitiveType::Triangles, s_2DData->DepthTest);
		}

	#pragma warning(push)
	#pragma warning(disable : 4244)
		dataSize = (uint8*)s_2DData->LineVertexBufferPtr - (uint8*)s_2DData->LineVertexBufferBase;
	#pragma warning(pop)
		if (dataSize)
		{
			s_2DData->LineShader->Bind();
			s_2DData->LineVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->LineVertexBufferBase, dataSize);

			s_2DData->LineVertexArray->GetVertexBuffers()[0]->Bind();
			s_2DData->LineVertexArray->GetIndexBuffer()->Bind();
			s_2DData->LineVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->LineIndexCount, PrimitiveType::Lines, false);
		}

	#pragma warning(push)
	#pragma warning(disable : 4244)
		dataSize = (uint8*)s_2DData->CircleVertexBufferPtr - (uint8*)s_2DData->CircleVertexBufferBase;
	#pragma warning(pop)
		if (dataSize)
		{
			s_2DData->CircleShader->Bind();
			s_2DData->CircleVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->CircleVertexBufferBase, dataSize);

			s_2DData->CircleVertexArray->GetVertexBuffers()[0]->Bind();
			s_2DData->CircleVertexArray->GetIndexBuffer()->Bind();
			s_2DData->CircleVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->CircleIndexCount, PrimitiveType::Triangles, s_2DData->DepthTest);
		}

		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
		HL_PROFILE_FUNCTION();

		s_2DData->QuadIndexCount = 0;
		s_2DData->QuadVertexBufferPtr = s_2DData->QuadVertexBufferBase;

		s_2DData->LineIndexCount = 0;
		s_2DData->LineVertexBufferPtr = s_2DData->LineVertexBufferBase;

		s_2DData->CircleIndexCount = 0;
		s_2DData->CircleVertexBufferPtr = s_2DData->CircleVertexBufferBase;

		s_2DData->TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		HL_PROFILE_FUNCTION();

		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		Transform transform = Transform::FromPosition(position).Scale({ size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Transform &transform, const glm::vec4 &color)
	{
		HL_PROFILE_FUNCTION();

		if (s_2DData->QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr uint32 quadVertexCount = 4;
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		for (uint32 i = 0; i < quadVertexCount; ++i)
		{
			s_2DData->QuadVertexBufferPtr->Position = transform.GetTransform() * s_2DData->QuadVertexPositions[i];
			s_2DData->QuadVertexBufferPtr->Color = color;
			s_2DData->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_2DData->QuadVertexBufferPtr->EntityID = 0;
			s_2DData->QuadVertexBufferPtr++;
		}

		s_2DData->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		Transform transform = Transform::FromPosition(position).Scale({ size.x, size.y, 1.0f });
		DrawQuad(transform, texture, tilingFactor, tintColor);
	}
	
	void Renderer2D::DrawQuad(const Transform &transform, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		HL_PROFILE_FUNCTION();

		if (s_2DData->QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr uint32 quadVertexCount = 4;

		float textureIndex = 0.0f;
		for (uint32 i = 1; i < s_2DData->TextureSlotIndex; ++i)
		{
			if (*s_2DData->TextureSlots[i].Get() == *texture.Get())
			{
				textureIndex = (float) i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float) s_2DData->TextureSlotIndex;
			s_2DData->TextureSlots[s_2DData->TextureSlotIndex] = texture;
			s_2DData->TextureSlotIndex++;
		}

		for (uint32 i = 0; i < quadVertexCount; ++i)
		{
			s_2DData->QuadVertexBufferPtr->Position = transform.GetTransform() * s_2DData->QuadVertexPositions[i];
			s_2DData->QuadVertexBufferPtr->Color = tintColor;
			s_2DData->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_2DData->QuadVertexBufferPtr->EntityID = 0;
			s_2DData->QuadVertexBufferPtr++;
		}

		s_2DData->QuadIndexCount += 6;
	}

	void Renderer2D::DrawLine(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec4 &color)
	{
		DrawLine({ p1.x, p1.y, 0.0f }, { p2.x, p2.y, 0.0f }, color);
	}

	void Renderer2D::DrawLine(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec4 &color)
	{
		s_2DData->LineVertexBufferPtr->Position = p1;
		s_2DData->LineVertexBufferPtr->Color = color;
		s_2DData->LineVertexBufferPtr->EntityID = 0;
		s_2DData->LineVertexBufferPtr++;

		s_2DData->LineVertexBufferPtr->Position = p2;
		s_2DData->LineVertexBufferPtr->Color = color;
		s_2DData->LineVertexBufferPtr->EntityID = 0;
		s_2DData->LineVertexBufferPtr++;

		s_2DData->LineIndexCount += 2;
	}

	void Renderer2D::DrawCircle(const glm::vec2 &p1, float radius, const glm::vec4 &color)
	{
		DrawCircle({ p1.x, p1.y, 0.0f }, radius, color);
	}

	void Renderer2D::DrawCircle(const glm::vec3 &p1, float radius, const glm::vec4 &color)
	{


		s_2DData->CircleIndexCount++;
	}

	void Renderer2D::DrawText(const HLString &text, const glm::vec3 &position, float maxWidth, const glm::vec4 &color)
	{
		DrawText(text, FontManager::Get()->GetDefaultFont(), position, maxWidth, color);
	}

	void Renderer2D::DrawText(const HLString &text, const Ref<Font> &font, const glm::vec3 &position, float maxWidth, const glm::vec4 &color)
	{
		DrawText(text, font, Transform::FromPosition(position), maxWidth, color);
	}

	void Renderer2D::DrawText(const HLString &text, const Ref<Font> &font, const Transform &transform, float maxWidth, const glm::vec4 &color, float lineHeightOffset, float kerningOffset)
	{
		if (text.IsEmpty())
			return;

		float textureIndex = 0.0f;

		std::u32string utf32Str = utils::ConvertToUtf32Str(*text);

		Ref<Texture2D> fontAtlas = font->GetTextureAtlas();
		HL_ASSERT(fontAtlas);

		for (uint32 i = 0; i < s_2DData->FontTextureSlotIndex; ++i)
		{
			if (*s_2DData->FontTextureSlots[i].Get() == *fontAtlas.Get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_2DData->FontTextureSlotIndex;
			s_2DData->FontTextureSlots[s_2DData->FontTextureSlotIndex] = fontAtlas;
			++s_2DData->FontTextureSlotIndex;
		}

		auto &fontGeometry = font->GetMSDFData()->FontGeometry;
		const auto &metrics = fontGeometry.getMetrics();
		std::vector<int32> nextLines;

		{
			double x = 0.0;
			double fsScale = 1 / (metrics.ascenderY - metrics.descenderY);
			double y = -fsScale * metrics.ascenderY;
			int32 lastSpace = -1;

			for (int32 i = 0; i < utf32Str.size(); ++i)
			{
				char32_t ch = utf32Str[i];
				if (ch == '\n')
				{
					x = 0.0;
					y = fsScale * metrics.lineHeight + lineHeightOffset;
					continue;
				}

				auto glyph = fontGeometry.getGlyph(ch);
				if (!glyph)
					glyph = fontGeometry.getGlyph('?');
				if (!glyph)
					continue;

				if (ch != ' ')
				{
					// Calc geometry
					double pl, pb, pr, pt;
					glyph->getQuadPlaneBounds(pl, pb, pr, pt);
					glm::vec2 quadMin((float)pl, (float)pb);
					glm::vec2 quadMax((float)pr, (float)pt);

					quadMin *= fsScale;
					quadMax *= fsScale;
					quadMin += glm::vec2(x, y);
					quadMax += glm::vec2(x, y);

					if (quadMax.x > maxWidth && lastSpace != -1)
					{
						i = lastSpace;
						nextLines.emplace_back(lastSpace);
						lastSpace = -1;
						x = 0.0;
						y -= fsScale * metrics.lineHeight + lineHeightOffset;
					}
				}
				else
				{
					lastSpace = i;
				}

				double advance = glyph->getAdvance();
				fontGeometry.getAdvance(advance, ch, utf32Str[i + 1]);
				x += fsScale * advance + kerningOffset;
			}
		}

		{
			double x = 0.0;
			double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
			double y = 0.0;

			for (int32 i = 0; i < utf32Str.size(); ++i)
			{
				char32_t ch = utf32Str[i];
				if (ch == '\n' || utils::NextLine(i, nextLines))
				{
					x = 0.0;
					y -= fsScale * metrics.lineHeight + lineHeightOffset;
					continue;
				}

				auto glyph = fontGeometry.getGlyph(ch);
				if (!glyph)
					glyph = fontGeometry.getGlyph('?');
				if (!glyph)
					continue;

				double l, b, r, t;
				glyph->getQuadAtlasBounds(l, b, r, t);

				double pl, pb, pr, pt;
				glyph->getQuadPlaneBounds(pl, pb, pr, pt);

				pl *= fsScale, pb *= fsScale, pr *= fsScale, pt *= fsScale;
				pl += x, pb += y, pr += x, pt += y;

				double texelWidth = 1. / fontAtlas->GetWidth();
				double texelHeight = 1. / fontAtlas->GetHeight();
				l *= texelWidth, b *= texelHeight, r *= texelWidth, t *= texelHeight;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(pl, pb, 0.0f, 1.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = { l, b };
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(pl, pt, 0.0f, 1.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = { l, t };
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(pr, pt, 0.0f, 1.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = { r, t };
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(pr, pb, 0.0f, 1.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = { r, b };
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextIndexCount += 6;

				double advance = glyph->getAdvance();
				fontGeometry.getAdvance(advance, ch, utf32Str[i + 1]);
				x += fsScale * advance + kerningOffset;
			}
		}
	}


}