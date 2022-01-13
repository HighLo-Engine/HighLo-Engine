// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Renderer2D.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shaders/Shader.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "FontData.h"

#include "Engine/Utils/StringUtils.h"
#include "Engine/Renderer/Shaders/UniformBufferSet.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/CommandBuffer.h"

#include <codecvt>

namespace highlo
{
	namespace utils
	{
		static HLString32 ToUTF32(const HLString &str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
			std::u32string result = conv.from_bytes(*str);
			return HLString32(result.c_str(), result.length());
		}
	}

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

	struct UniformBufferCamera
	{
		glm::mat4 ViewProjection;
	};

	struct Renderer2DData
	{
		static const uint32 MaxQuads = 20000;
		static const uint32 MaxVertices = MaxQuads * 4;
		static const uint32 MaxIndices = MaxQuads * 6;
		static const uint32 MaxTextureSlots = 32; // TODO: This is platform dependent, so get it from the RenderingAPI

		static const uint32 MaxLines = 10000;
		static const uint32 MaxLineVertices = MaxLines * 2;
		static const uint32 MaxLineIndices = MaxLines * 6;

		Ref<CommandBuffer> RenderCommandBuffer;
		Ref<Texture2D> WhiteTexture;

		// Quads
		Ref<Shader> TextureShader;
		Ref<VertexArray> QuadVertexArray;
		Ref<Material> TextureMaterial;
		uint32 QuadIndexCount = 0;
		QuadVertex *QuadVertexBufferBase = nullptr;
		QuadVertex *QuadVertexBufferPtr = nullptr;

		// Lines
		Ref<Shader> LineShader;
		Ref<VertexArray> LineVertexArray;
		Ref<Material> LineMaterial;
		LineVertex *LineVertexBufferBase = nullptr;
		LineVertex *LineVertexBufferPtr = nullptr;
		uint32 LineIndexCount = 0;

		// Circles
		Ref<Shader> CircleShader;
		Ref<VertexArray> CircleVertexArray;
		Ref<Material> CircleMaterial;
		CircleVertex *CircleVertexBufferBase = nullptr;
		CircleVertex *CircleVertexBufferPtr = nullptr;
		uint32 CircleIndexCount = 0;

		// Text
		Ref<Shader> TextShader;
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
		bool SwapChainTarget = false;
		glm::mat4 CameraProjection = glm::mat4(1.0f);
		Ref<UniformBufferSet> UniformBufferSet;
	};

	static Renderer2DData *s_2DData;

	void Renderer2D::Init()
	{
		HL_PROFILE_FUNCTION();

		s_2DData = new Renderer2DData();

		if (s_2DData->SwapChainTarget)
		{
			s_2DData->RenderCommandBuffer = CommandBuffer::CreateFromSwapChain("Renderer2D");
		}
		else
		{
			s_2DData->RenderCommandBuffer = CommandBuffer::Create("Renderer2D");
		}

		// Set all texture slots to 0
		s_2DData->WhiteTexture = Renderer::GetWhiteTexture();
		s_2DData->TextureSlots[0] = s_2DData->WhiteTexture;

		s_2DData->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[1] = { -0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[3] = {  0.5f, -0.5f, 0.0f, 1.0f };

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

		// Framebuffer and renderpass
		FramebufferSpecification framebufferSpec;
		framebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::Depth };
		framebufferSpec.Samples = 1;
		framebufferSpec.ClearOnLoad = false;
		framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		framebufferSpec.DebugName = "Renderer2D Framebuffer";

		Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);
		
		RenderPassSpecification renderPassSpec;
		renderPassSpec.DebugName = "Renderer2D RenderPass";
		renderPassSpec.Framebuffer = framebuffer;
		Ref<RenderPass> renderPass = RenderPass::Create(renderPassSpec);

		std::vector<int32> textIndices;
		offset = 0;
		for (uint32 i = 0; i < s_2DData->MaxIndices; ++i)
		{
			textIndices.push_back(offset + 0);
			textIndices.push_back(offset + 1);
			textIndices.push_back(offset + 2);

			textIndices.push_back(offset + 2);
			textIndices.push_back(offset + 3);
			textIndices.push_back(offset + 0);

			offset += 4;
		}

		// Quads
		s_2DData->TextureShader = Renderer::GetShaderLibrary()->Get("Renderer2DQuad");
		s_2DData->TextureMaterial = Material::Create(s_2DData->TextureShader, "TextureMaterial");
		s_2DData->QuadVertexBufferBase = new QuadVertex[s_2DData->MaxVertices];

		VertexArraySpecification quadVertexArraySpec;
		quadVertexArraySpec.Layout = BufferLayout::GetTextureLayout();
		quadVertexArraySpec.Shader = s_2DData->TextureShader;
		quadVertexArraySpec.RenderPass = renderPass;
		s_2DData->QuadVertexArray = VertexArray::Create(quadVertexArraySpec);
		s_2DData->QuadVertexArray->Bind();
	
		auto vb = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(QuadVertex));
		s_2DData->QuadVertexArray->AddVertexBuffer(vb);
	
		s_2DData->QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(&quadIndices[0], s_2DData->MaxIndices));
		s_2DData->QuadVertexArray->Unbind();

		// Circles
		s_2DData->CircleShader = Renderer::GetShaderLibrary()->Get("Renderer2DCircle");
		s_2DData->CircleMaterial = Material::Create(s_2DData->CircleShader, "CircleMaterial");
		s_2DData->CircleVertexBufferBase = new CircleVertex[s_2DData->MaxVertices];

		VertexArraySpecification circleVertexArraySpec;
		circleVertexArraySpec.Layout = BufferLayout::GetCircleLayout();
		circleVertexArraySpec.Shader = s_2DData->CircleShader;
		circleVertexArraySpec.RenderPass = renderPass;
		s_2DData->CircleVertexArray = VertexArray::Create(circleVertexArraySpec);
		s_2DData->CircleVertexArray->Bind();

		auto circlesVb = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(CircleVertex));
		s_2DData->CircleVertexArray->AddVertexBuffer(circlesVb);
		s_2DData->CircleVertexArray->SetIndexBuffer(s_2DData->QuadVertexArray->GetIndexBuffer());
		s_2DData->CircleVertexArray->Unbind();

		// Lines
		s_2DData->LineShader = Renderer::GetShaderLibrary()->Get("Renderer2DLine");
		s_2DData->LineMaterial = Material::Create(s_2DData->LineShader, "LineMaterial");
		s_2DData->LineVertexBufferBase = new LineVertex[s_2DData->MaxLines];

		VertexArraySpecification lineVertexArraySpec;
		lineVertexArraySpec.Layout = BufferLayout::GetLineLayout();
		lineVertexArraySpec.Shader = s_2DData->LineShader;
		lineVertexArraySpec.RenderPass = renderPass;
		s_2DData->LineVertexArray = VertexArray::Create(lineVertexArraySpec);
		s_2DData->LineVertexArray->Bind();

		auto linesVb = VertexBuffer::Create(s_2DData->MaxLineVertices * sizeof(LineVertex));
		s_2DData->LineVertexArray->AddVertexBuffer(linesVb);
		s_2DData->LineVertexArray->SetIndexBuffer(IndexBuffer::Create(&lineIndices[0], s_2DData->MaxLineIndices));
		s_2DData->LineVertexArray->Unbind();

		// Text
		s_2DData->TextShader = Renderer::GetShaderLibrary()->Get("Renderer2DText");
		s_2DData->TextMaterial = Material::Create(s_2DData->TextShader, "TextMaterial");
		s_2DData->TextVertexBufferBase = new TextVertex[s_2DData->MaxVertices];

		VertexArraySpecification textVertexArraySpec;
		textVertexArraySpec.Layout = BufferLayout::GetTextLayout();
		textVertexArraySpec.Shader = s_2DData->TextShader;
		textVertexArraySpec.RenderPass = renderPass;
		s_2DData->TextVertexArray = VertexArray::Create(textVertexArraySpec);
		s_2DData->TextVertexArray->Bind();

		auto textVb = VertexBuffer::Create(s_2DData->MaxQuads * sizeof(TextVertex));
		s_2DData->TextVertexArray->AddVertexBuffer(textVb);
		s_2DData->TextVertexArray->SetIndexBuffer(IndexBuffer::Create(&textIndices[0], s_2DData->MaxIndices));
		s_2DData->TextVertexArray->Unbind();

		// Uniform Buffer
		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		s_2DData->UniformBufferSet = UniformBufferSet::Create(framesInFlight);
		s_2DData->UniformBufferSet->CreateUniform(sizeof(UniformBufferCamera), 0);
	}

	void Renderer2D::Shutdown()
	{
		HL_PROFILE_FUNCTION();

		delete[] s_2DData->QuadVertexBufferBase;
		delete[] s_2DData->LineVertexBufferBase;
		delete[] s_2DData->CircleVertexBufferBase;
		delete[] s_2DData->TextVertexBufferBase;

		delete s_2DData;
	}

	void Renderer2D::BeginScene(const glm::mat4 &proj, bool depthTest)
	{
		HL_PROFILE_FUNCTION();

		s_2DData->CameraProjection = proj;
		s_2DData->DepthTest = depthTest;

		s_2DData->TextureShader->Bind();

		// Load Camera Projection into Uniform Buffer block
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->SetData(&proj, sizeof(UniformBufferCamera));

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

		s_2DData->RenderCommandBuffer->Begin();
		Renderer::BeginRenderPass(s_2DData->RenderCommandBuffer, s_2DData->QuadVertexArray->GetSpecification().RenderPass);

		FlushQuads();
		FlushCircles();
		FlushLines();
		FlushText();

		Renderer::EndRenderPass(s_2DData->RenderCommandBuffer);
		s_2DData->RenderCommandBuffer->End();
		s_2DData->RenderCommandBuffer->Submit();
	}

	void Renderer2D::FlushQuads()
	{
		if (s_2DData->QuadIndexCount == 0)
			return;

		uint32 dataSize = (uint32)((uint8*)s_2DData->QuadVertexBufferPtr - (uint8*)s_2DData->QuadVertexBufferBase);
		if (dataSize)
		{
			s_2DData->QuadVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->QuadVertexBufferBase, dataSize);

			for (uint32 i = 0; i < s_2DData->TextureSlots.size(); ++i)
			{
				if (s_2DData->TextureSlots[i])
					s_2DData->TextureMaterial->Set("u_Textures", s_2DData->TextureSlots[i], i);
				else
					s_2DData->TextureMaterial->Set("u_Textures", s_2DData->WhiteTexture, i);
			}

			// Bind Camera Uniform Buffer block
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->TextureShader->Bind();
			s_2DData->QuadVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->QuadIndexCount, s_2DData->TextureMaterial, s_2DData->UniformBufferSet, PrimitiveType::Triangles, s_2DData->DepthTest);
		}
	}

	void Renderer2D::FlushCircles()
	{
		if (s_2DData->CircleIndexCount == 0)
			return;

		uint32 dataSize = (uint32)((uint8*)s_2DData->CircleVertexBufferPtr - (uint8*)s_2DData->CircleVertexBufferBase);
		if (dataSize)
		{
			s_2DData->CircleVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->CircleVertexBufferBase, dataSize);

			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->CircleShader->Bind();
			s_2DData->CircleVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->CircleIndexCount, s_2DData->CircleMaterial, s_2DData->UniformBufferSet, PrimitiveType::Triangles, s_2DData->DepthTest);
		}
	}

	void Renderer2D::FlushLines()
	{
		if (s_2DData->LineIndexCount == 0)
			return;

		uint32 dataSize = (uint32)((uint8*)s_2DData->LineVertexBufferPtr - (uint8*)s_2DData->LineVertexBufferBase);
		if (dataSize)
		{
			s_2DData->LineVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->LineVertexBufferBase, dataSize);

			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->LineShader->Bind();
			s_2DData->LineVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->LineIndexCount, s_2DData->LineMaterial, s_2DData->UniformBufferSet, PrimitiveType::Lines, s_2DData->DepthTest);
		}
	}

	void Renderer2D::FlushText()
	{
		if (s_2DData->TextIndexCount == 0)
			return;

		uint32 dataSize = (uint32)((uint8*)s_2DData->TextVertexBufferPtr - (uint8*)s_2DData->TextVertexBufferBase);
		if (dataSize)
		{
			s_2DData->TextVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->TextVertexBufferBase, dataSize);

			for (uint32 i = 0; i < s_2DData->FontTextureSlots.size(); ++i)
			{
				if (s_2DData->FontTextureSlots[i])
					s_2DData->TextMaterial->Set("u_FontAtlases", s_2DData->FontTextureSlots[i], i);
				else
					s_2DData->TextMaterial->Set("u_FontAtlases", s_2DData->WhiteTexture, i);
			}

			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->TextShader->Bind();
			s_2DData->TextVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->TextIndexCount, s_2DData->TextMaterial, s_2DData->UniformBufferSet, PrimitiveType::Triangles, s_2DData->DepthTest);
		}
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

		s_2DData->TextIndexCount = 0;
		s_2DData->TextVertexBufferPtr = s_2DData->TextVertexBufferBase;

		s_2DData->TextureSlotIndex = 1;
		s_2DData->FontTextureSlotIndex = 0;

		for (uint32 i = 1; i < s_2DData->TextureSlots.size(); ++i)
			s_2DData->TextureSlots[i] = nullptr;

		for (uint32 i = 0; i < s_2DData->FontTextureSlots.size(); ++i)
			s_2DData->FontTextureSlots[i] = nullptr;
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
		if (s_2DData->LineIndexCount >= Renderer2DData::MaxLineIndices)
			Flush();

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

	void Renderer2D::FillCircle(const glm::vec2 &position, float radius, float thickness, const glm::vec4 &color)
	{
		FillCircle({ position.x, position.y, 0.0f }, radius, thickness, color);
	}

	void Renderer2D::FillCircle(const glm::vec3 &position, float radius, float thickness, const glm::vec4 &color)
	{
		if (s_2DData->CircleIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { radius * 2.0f, radius * 2.0f, 1.0f });

		for (int32 i = 0; i < 4; ++i)
		{
			s_2DData->CircleVertexBufferPtr->WorldPosition = transform * s_2DData->QuadVertexPositions[i];
			s_2DData->CircleVertexBufferPtr->Thickness = thickness;
			s_2DData->CircleVertexBufferPtr->LocalPosition = s_2DData->QuadVertexPositions[i] * 2.0f;
			s_2DData->CircleVertexBufferPtr->Color = color;
			s_2DData->CircleVertexBufferPtr->EntityID = 0;
			s_2DData->CircleVertexBufferPtr++;
		}

		s_2DData->CircleIndexCount += 6;
	}

	void Renderer2D::FillCircle(const Transform &transform, float radius, float thickness, const glm::vec4 &color)
	{
		FillCircle(transform.GetPosition(), radius, thickness, color);
	}

	void Renderer2D::DrawCircle(const Transform &transform, float radius, const glm::vec4 &color)
	{
		glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), transform.GetPosition())
			* glm::rotate(glm::mat4(1.0f), transform.GetRotation().x, { 1.0f, 0.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), transform.GetRotation().y, { 0.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), transform.GetRotation().z, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(radius));

		uint32 segments = 32;
		for (uint32 i = 0; i < segments; ++i)
		{
			float angle = 2.0f * glm::pi<float>() * (float)i / segments;
			glm::vec4 startPos = { glm::cos(angle), glm::sin(angle), 0.0f, 1.0f };

			angle = 2.0f * glm::pi<float>() * (float)((i + 1) % segments) / segments;
			glm::vec4 endPos = { glm::cos(angle), glm::sin(angle), 0.0f, 1.0f };

			glm::vec3 p0 = localTransform * startPos;
			glm::vec3 p1 = localTransform * endPos;
			DrawLine(p0, p1, color);
		}
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
		HLString32 utf32Text = utils::ToUTF32(text);

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

			for (uint32 i = 0; i < utf32Text.Length(); ++i)
			{
				char32_t ch = utf32Text[i];
				if (ch == '\n')
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
				fontGeometry.getAdvance(advance, ch, utf32Text[i + 1]);
				x += fsScale * advance + kerningOffset;
			}
		}

		{
			double x = 0.0;
			double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
			double y = 0.0;

			for (uint32 i = 0; i < utf32Text.Length(); ++i)
			{
				char32_t ch = utf32Text[i];
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

			#ifdef HL_DEBUG
				/*
				ImGui::Begin("Font Debugger");
				ImGui::Text("Size: %dx%d", fontAtlas->GetWidth(), fontAtlas->GetHeight());
				UI::Image(fontAtlas, ImVec2((float)fontAtlas->GetWidth(), (float)fontAtlas->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::End();
				*/
			#endif // HL_DEBUG

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
				fontGeometry.getAdvance(advance, ch, utf32Text[i + 1]);
				x += fsScale * advance + kerningOffset;
			}
		}
	}


}