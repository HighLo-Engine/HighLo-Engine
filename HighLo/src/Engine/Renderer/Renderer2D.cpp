// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Renderer2D.h"

#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Graphics/Framebuffer.h"
#include "Engine/Graphics/Shaders/UniformBufferSet.h"
#include "Engine/Graphics/Shaders/UniformDefinitions.h"
#include "Engine/Graphics/Material.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/FontData.h"

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
		static const uint32 MaxTextureSlots = 32; // TODO: This is platform dependent, so get it from the RenderingAPI

		static const uint32 MaxLines = 10000;
		static const uint32 MaxLineVertices = MaxLines * 2;
		static const uint32 MaxLineIndices = MaxLines * 6;

		Ref<Texture2D> WhiteTexture = nullptr;

		// Quads
		Ref<Shader> TextureShader = nullptr;
		Ref<VertexArray> QuadVertexArray = nullptr;
		std::vector<Ref<VertexBuffer>> QuadVertexBuffers;
		Ref<IndexBuffer> QuadIndexBuffer = nullptr;
		std::vector<QuadVertex*> QuadVertexBufferBase;
		Ref<Material> TextureMaterial = nullptr;
		QuadVertex *QuadVertexBufferPtr = nullptr;
		uint32 QuadIndexCount = 0;

		// Lines
		Ref<Shader> LineShader = nullptr;
		Ref<VertexArray> LineVertexArray = nullptr;
		std::vector<Ref<VertexBuffer>> LineVertexBuffers;
		Ref<IndexBuffer> LineIndexBuffer = nullptr;
		std::vector<LineVertex*> LineVertexBufferBase;
		Ref<Material> LineMaterial = nullptr;
		LineVertex *LineVertexBufferPtr = nullptr;
		uint32 LineIndexCount = 0;

		// Circles
		Ref<Shader> CircleShader = nullptr;
		Ref<VertexArray> CircleVertexArray = nullptr;
		std::vector<Ref<VertexBuffer>> CircleVertexBuffers;
		std::vector<CircleVertex*> CircleVertexBufferBase;
		Ref<Material> CircleMaterial = nullptr;
		CircleVertex *CircleVertexBufferPtr = nullptr;
		uint32 CircleIndexCount = 0;

		// Text
		Ref<Shader> TextShader = nullptr;
		std::vector<Ref<VertexBuffer>> TextVertexBuffers;
		Ref<IndexBuffer> TextIndexBuffer = nullptr;
		std::vector<TextVertex*> TextVertexBufferBase;
		Ref<VertexArray> TextVertexArray = nullptr;
		Ref<Material> TextMaterial = nullptr;
		TextVertex *TextVertexBufferPtr = nullptr;
		uint32 TextIndexCount = 0;

		// Font Atlas
		uint32 FontTextureSlotIndex = 0;
		std::array<Ref<Texture2D>, MaxTextureSlots> FontTextureSlots;

		// Textures
		uint32 TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		glm::vec4 QuadVertexPositions[4];
		Ref<RenderPass> ActiveRenderPass = nullptr;
		Ref<CommandBuffer> ActiveCommandBuffer = nullptr;

		bool DepthTest = true;
		bool SwapChainTarget = false;
		Ref<UniformBufferSet> UniformBufferSet = nullptr;
		Renderer2DStats Statistics;
	};

	static Renderer2DData *s_2DData;

	void Renderer2D::Init()
	{
		HL_PROFILE_FUNCTION();

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;

		s_2DData = new Renderer2DData();
		s_2DData->ActiveCommandBuffer = CommandBuffer::Create("Renderer2D");

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

		RenderPassSpecification renderPassSpec;
		renderPassSpec.DebugName = "Renderer2D RenderPass";
		renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
		s_2DData->ActiveRenderPass = RenderPass::Create(renderPassSpec);

		std::vector<int32> textIndices;
		offset = 0;
		for (uint32 i = 0; i < s_2DData->MaxIndices; ++i)
		{
			textIndices.push_back(offset + 2);
			textIndices.push_back(offset + 1);
			textIndices.push_back(offset + 0);

			textIndices.push_back(offset + 3);
			textIndices.push_back(offset + 0);
			textIndices.push_back(offset + 1);

			offset += 4;
		}

		// Quads
		s_2DData->TextureShader = Renderer::GetShaderLibrary()->Get("Renderer2DQuad");
		s_2DData->TextureMaterial = Material::Create(s_2DData->TextureShader, "TextureMaterial");

		// Quad Vertex array
		VertexArraySpecification quadVertexArraySpec;
		quadVertexArraySpec.Layout = BufferLayout::GetTextureLayout();
		quadVertexArraySpec.Shader = s_2DData->TextureShader;
		quadVertexArraySpec.RenderPass = s_2DData->ActiveRenderPass;
		s_2DData->QuadVertexArray = VertexArray::Create(quadVertexArraySpec);
		s_2DData->QuadVertexArray->Bind();

		// Quad Vertex buffer and storage for batch data
		s_2DData->QuadVertexBuffers.resize(framesInFlight);
		s_2DData->QuadVertexBufferBase.resize(framesInFlight);
		for (uint32 i = 0; i < framesInFlight; ++i)
		{
			s_2DData->QuadVertexBuffers[i] = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(QuadVertex));
			s_2DData->QuadVertexBufferBase[i] = new QuadVertex[s_2DData->MaxVertices];
			s_2DData->QuadVertexArray->AddVertexBuffer(s_2DData->QuadVertexBuffers[i]);
		}

		// Quad Index buffer
		s_2DData->QuadIndexBuffer = IndexBuffer::Create(&quadIndices[0], s_2DData->MaxIndices);
		s_2DData->QuadVertexArray->SetIndexBuffer(s_2DData->QuadIndexBuffer);

		// Circles
		s_2DData->CircleShader = Renderer::GetShaderLibrary()->Get("Renderer2DCircle");
		s_2DData->CircleMaterial = Material::Create(s_2DData->CircleShader, "CircleMaterial");

		VertexArraySpecification circleVertexArraySpec;
		circleVertexArraySpec.Layout = BufferLayout::GetCircleLayout();
		circleVertexArraySpec.Shader = s_2DData->CircleShader;
		circleVertexArraySpec.RenderPass = s_2DData->ActiveRenderPass;
		s_2DData->CircleVertexArray = VertexArray::Create(circleVertexArraySpec);
		s_2DData->CircleVertexArray->Bind();

		s_2DData->CircleVertexBuffers.resize(framesInFlight);
		s_2DData->CircleVertexBufferBase.resize(framesInFlight);
		for (uint32 i = 0; i < framesInFlight; ++i)
		{
			s_2DData->CircleVertexBuffers[i] = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(CircleVertex));
			s_2DData->CircleVertexBufferBase[i] = new CircleVertex[s_2DData->MaxVertices];
			s_2DData->CircleVertexArray->AddVertexBuffer(s_2DData->CircleVertexBuffers[i]);
		}

		// Lines
		s_2DData->LineShader = Renderer::GetShaderLibrary()->Get("Renderer2DLine");
		s_2DData->LineMaterial = Material::Create(s_2DData->LineShader, "LineMaterial");

		VertexArraySpecification lineVertexArraySpec;
		lineVertexArraySpec.Layout = BufferLayout::GetLineLayout();
		lineVertexArraySpec.Shader = s_2DData->LineShader;
		lineVertexArraySpec.RenderPass = s_2DData->ActiveRenderPass;
		s_2DData->LineVertexArray = VertexArray::Create(lineVertexArraySpec);
		s_2DData->LineVertexArray->Bind();

		s_2DData->LineVertexBuffers.resize(framesInFlight);
		s_2DData->LineVertexBufferBase.resize(framesInFlight);
		for (uint32 i = 0; i < framesInFlight; ++i)
		{
			s_2DData->LineVertexBuffers[i] = VertexBuffer::Create(s_2DData->MaxLineVertices * sizeof(LineVertex));
			s_2DData->LineVertexBufferBase[i] = new LineVertex[s_2DData->MaxLineVertices];
			s_2DData->LineVertexArray->AddVertexBuffer(s_2DData->LineVertexBuffers[i]);
		}

		s_2DData->LineIndexBuffer = IndexBuffer::Create(&lineIndices[0], s_2DData->MaxLineIndices);
		s_2DData->LineVertexArray->SetIndexBuffer(s_2DData->LineIndexBuffer);

		// Text
		s_2DData->TextShader = Renderer::GetShaderLibrary()->Get("Renderer2DText");
		s_2DData->TextMaterial = Material::Create(s_2DData->TextShader, "TextMaterial");

		VertexArraySpecification textVertexArraySpec;
		textVertexArraySpec.Layout = BufferLayout::GetTextLayout();
		textVertexArraySpec.Shader = s_2DData->TextShader;
		textVertexArraySpec.RenderPass = s_2DData->ActiveRenderPass;
		s_2DData->TextVertexArray = VertexArray::Create(textVertexArraySpec);
		s_2DData->TextVertexArray->Bind();

		s_2DData->TextVertexBuffers.resize(framesInFlight);
		s_2DData->TextVertexBufferBase.resize(framesInFlight);
		for (uint32 i = 0; i < framesInFlight; ++i)
		{
			s_2DData->TextVertexBuffers[i] = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(TextVertex));
			s_2DData->TextVertexBufferBase[i] = new TextVertex[s_2DData->MaxVertices];
			s_2DData->TextVertexArray->AddVertexBuffer(s_2DData->TextVertexBuffers[i]);
		}

		s_2DData->TextIndexBuffer = IndexBuffer::Create(&textIndices[0], s_2DData->MaxIndices);
		s_2DData->TextVertexArray->SetIndexBuffer(s_2DData->TextIndexBuffer);

		// Uniform Buffer
		s_2DData->UniformBufferSet = UniformBufferSet::Create(framesInFlight);
		s_2DData->UniformBufferSet->CreateUniform(sizeof(UniformBufferCamera), 0, UniformLayout::GetCameraLayout());
	}

	void Renderer2D::Shutdown()
	{
		HL_PROFILE_FUNCTION();

		for (auto &buffer : s_2DData->QuadVertexBufferBase)
			delete[] buffer;

		for (auto &buffer : s_2DData->TextVertexBufferBase)
			delete[] buffer;

		for (auto &buffer : s_2DData->LineVertexBufferBase)
			delete[] buffer;

		for (auto &buffer : s_2DData->CircleVertexBufferBase)
			delete[] buffer;

		delete s_2DData;
	}

	void Renderer2D::BeginScene(const Camera &camera, bool depthTest)
	{
		HL_PROFILE_FUNCTION();

		s_2DData->DepthTest = depthTest;
		UniformBufferCamera cameraStruct;
		cameraStruct.ViewProjection = camera.GetProjection() * camera.GetViewMatrix();
		cameraStruct.Projection = camera.GetProjection();
		cameraStruct.View = camera.GetViewMatrix();
		cameraStruct.InverseViewProjection = glm::inverse(cameraStruct.ViewProjection);

		s_2DData->TextureShader->Bind();

		// Load Camera Projection into Uniform Buffer block
		Renderer::Submit([cameraStruct]()
		{
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->SetData(&cameraStruct, sizeof(cameraStruct));
		});

		StartBatch();
		ResetStatistics();
	}

	void Renderer2D::BeginScene(const EditorCamera &camera, bool depthTest)
	{
		HL_PROFILE_FUNCTION();

		s_2DData->DepthTest = depthTest;
		UniformBufferCamera cameraStruct;
		cameraStruct.ViewProjection = camera.GetProjection() * camera.GetViewMatrix();
		cameraStruct.Projection = camera.GetProjection();
		cameraStruct.View = camera.GetViewMatrix();
		cameraStruct.InverseViewProjection = glm::inverse(cameraStruct.ViewProjection);

		s_2DData->TextureShader->Bind();

		// Load Camera Projection into Uniform Buffer block
		Renderer::Submit([cameraStruct]()
		{
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->SetData(&cameraStruct, sizeof(cameraStruct));
		});

		StartBatch();
		ResetStatistics();
	}

	void Renderer2D::EndScene()
	{
		HL_PROFILE_FUNCTION();

		Renderer::Submit([]()
		{
			Flush();
		});
	}

	void Renderer2D::Flush()
	{
		HL_PROFILE_FUNCTION();

		s_2DData->ActiveCommandBuffer->Begin();
		Renderer::BeginRenderPass(s_2DData->ActiveCommandBuffer, s_2DData->ActiveRenderPass);

		FlushQuads();
		FlushCircles();
		FlushLines();
		FlushText();

		Renderer::EndRenderPass(s_2DData->ActiveCommandBuffer);
		s_2DData->ActiveCommandBuffer->End();
		s_2DData->ActiveCommandBuffer->Submit();
	}

	void Renderer2D::FlushQuads()
	{
		if (s_2DData->QuadIndexCount == 0)
			return;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		uint32 dataSize = (uint32)((uint8*)s_2DData->QuadVertexBufferPtr - (uint8*)s_2DData->QuadVertexBufferBase[frameIndex]);
		if (dataSize)
		{
			s_2DData->QuadVertexBuffers[frameIndex]->SetData(s_2DData->QuadVertexBufferBase[frameIndex], dataSize);

			for (uint32 i = 0; i < s_2DData->TextureSlots.size(); ++i)
			{
				if (s_2DData->TextureSlots[i])
					s_2DData->TextureMaterial->Set("u_Textures", s_2DData->TextureSlots[i], i);
				else
					s_2DData->TextureMaterial->Set("u_Textures", s_2DData->WhiteTexture, i);
			}

			// Bind Camera Uniform Buffer block
			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->TextureShader->Bind();
			s_2DData->QuadVertexBuffers[frameIndex]->Bind();
			s_2DData->QuadVertexArray->Bind();
			s_2DData->QuadIndexBuffer->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->QuadIndexCount, s_2DData->TextureMaterial, s_2DData->UniformBufferSet, PrimitiveType::Triangles, s_2DData->DepthTest);
			s_2DData->Statistics.DrawCalls++;
		}
	}

	void Renderer2D::FlushCircles()
	{
		if (s_2DData->CircleIndexCount == 0)
			return;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		uint32 dataSize = (uint32)((uint8*)s_2DData->CircleVertexBufferPtr - (uint8*)s_2DData->CircleVertexBufferBase[frameIndex]);
		if (dataSize)
		{
			s_2DData->CircleVertexBuffers[frameIndex]->SetData(s_2DData->CircleVertexBufferBase[frameIndex], dataSize);

			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->CircleShader->Bind();
			s_2DData->CircleVertexBuffers[frameIndex]->Bind();
			s_2DData->CircleVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->CircleIndexCount, s_2DData->CircleMaterial, s_2DData->UniformBufferSet, PrimitiveType::Triangles, s_2DData->DepthTest);
			s_2DData->Statistics.DrawCalls++;
		}
	}

	void Renderer2D::FlushLines()
	{
		if (s_2DData->LineIndexCount == 0)
			return;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		uint32 dataSize = (uint32)((uint8*)s_2DData->LineVertexBufferPtr - (uint8*)s_2DData->LineVertexBufferBase[frameIndex]);
		if (dataSize)
		{
			s_2DData->LineVertexBuffers[frameIndex]->SetData(s_2DData->LineVertexBufferBase[frameIndex], dataSize);

			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->LineShader->Bind();
			s_2DData->LineVertexBuffers[frameIndex]->Bind();
			s_2DData->LineVertexArray->Bind();
			s_2DData->LineIndexBuffer->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->LineIndexCount, s_2DData->LineMaterial, s_2DData->UniformBufferSet, PrimitiveType::Lines, s_2DData->DepthTest);
			s_2DData->Statistics.DrawCalls++;
		}
	}

	void Renderer2D::FlushText()
	{
		if (s_2DData->TextIndexCount == 0)
			return;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		uint32 dataSize = (uint32)((uint8*)s_2DData->TextVertexBufferPtr - (uint8*)s_2DData->TextVertexBufferBase[frameIndex]);
		if (dataSize)
		{
			s_2DData->TextVertexBuffers[frameIndex]->SetData(s_2DData->TextVertexBufferBase[frameIndex], dataSize);
			s_2DData->TextShader->Bind();

			for (uint32 i = 0; i < s_2DData->FontTextureSlots.size(); ++i)
			{
				if (s_2DData->FontTextureSlots[i])
					s_2DData->TextMaterial->Set("u_FontAtlases", s_2DData->FontTextureSlots[i], i);
				else
					s_2DData->TextMaterial->Set("u_FontAtlases", s_2DData->WhiteTexture, i);
			}

			s_2DData->UniformBufferSet->GetUniform(0, 0, frameIndex)->Bind();

			s_2DData->TextVertexBuffers[frameIndex]->Bind();
			s_2DData->TextVertexArray->Bind();
			s_2DData->TextIndexBuffer->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->TextIndexCount, s_2DData->TextMaterial, s_2DData->UniformBufferSet, PrimitiveType::Triangles, s_2DData->DepthTest);
			s_2DData->Statistics.DrawCalls++;
		}
	}

	void Renderer2D::StartBatch()
	{
		HL_PROFILE_FUNCTION();

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		s_2DData->QuadIndexCount = 0;
		s_2DData->QuadVertexBufferPtr = s_2DData->QuadVertexBufferBase[frameIndex];

		s_2DData->LineIndexCount = 0;
		s_2DData->LineVertexBufferPtr = s_2DData->LineVertexBufferBase[frameIndex];

		s_2DData->CircleIndexCount = 0;
		s_2DData->CircleVertexBufferPtr = s_2DData->CircleVertexBufferBase[frameIndex];

		s_2DData->TextIndexCount = 0;
		s_2DData->TextVertexBufferPtr = s_2DData->TextVertexBufferBase[frameIndex];

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

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, int32 entityId)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, entityId);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, int32 entityId)
	{
		Transform transform = Transform::FromPosition(position).Scale({ size.x, size.y, 1.0f });
		DrawQuad(transform, color, entityId);
	}

	void Renderer2D::DrawQuad(const Transform &transform, const glm::vec4 &color, int32 entityId)
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
			s_2DData->QuadVertexBufferPtr->EntityID = entityId;
			s_2DData->QuadVertexBufferPtr++;
		}

		s_2DData->QuadIndexCount += 6;
		s_2DData->Statistics.QuadCount++;
	}

	void Renderer2D::DrawTexture(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor, int32 entityId)
	{
		DrawTexture({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor, entityId);
	}

	void Renderer2D::DrawTexture(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor, int32 entityId)
	{
		Transform transform = Transform::FromPosition(position).Scale({ size.x, size.y, 1.0f });
		DrawTexture(transform, texture, tilingFactor, tintColor, entityId);
	}
	
	void Renderer2D::DrawTexture(const Transform &transform, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor, int32 entityId)
	{
		HL_PROFILE_FUNCTION();

		if (s_2DData->QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr uint32 quadVertexCount = 4;

	//	transform = transform.Rotate(-90, { 0, 0, 1 });

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), transform.GetPosition()) 
			* glm::rotate(glm::mat4(1.0f), transform.GetRotation().x, { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), transform.GetRotation().y, { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), transform.GetRotation().z, { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), transform.GetScale());

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
			s_2DData->QuadVertexBufferPtr->Position = trans * s_2DData->QuadVertexPositions[i];
			s_2DData->QuadVertexBufferPtr->Color = tintColor;
			s_2DData->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_2DData->QuadVertexBufferPtr->EntityID = entityId;
			s_2DData->QuadVertexBufferPtr++;
		}

		s_2DData->QuadIndexCount += 6;
		s_2DData->Statistics.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec4 &color, int32 entityId)
	{
		DrawLine({ p1.x, p1.y, 0.0f }, { p2.x, p2.y, 0.0f }, color, entityId);
	}

	void Renderer2D::DrawLine(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec4 &color, int32 entityId)
	{
		if (s_2DData->LineIndexCount >= Renderer2DData::MaxLineIndices)
			Flush();

		s_2DData->LineVertexBufferPtr->Position = p1;
		s_2DData->LineVertexBufferPtr->Color = color;
		s_2DData->LineVertexBufferPtr->EntityID = entityId;
		s_2DData->LineVertexBufferPtr++;

		s_2DData->LineVertexBufferPtr->Position = p2;
		s_2DData->LineVertexBufferPtr->Color = color;
		s_2DData->LineVertexBufferPtr->EntityID = entityId;
		s_2DData->LineVertexBufferPtr++;

		s_2DData->LineIndexCount += 2;
		s_2DData->Statistics.LineCount++;
	}

	void Renderer2D::FillCircle(const glm::vec2 &position, float radius, float thickness, const glm::vec4 &color, int32 entityId)
	{
		FillCircle({ position.x, position.y, 0.0f }, radius, thickness, color, entityId);
	}

	void Renderer2D::FillCircle(const glm::vec3 &position, float radius, float thickness, const glm::vec4 &color, int32 entityId)
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
			s_2DData->CircleVertexBufferPtr->EntityID = entityId;
			s_2DData->CircleVertexBufferPtr++;
		}

		s_2DData->CircleIndexCount += 6;
		s_2DData->Statistics.CircleCount++;
	}

	void Renderer2D::FillCircle(const Transform &transform, float radius, float thickness, const glm::vec4 &color, int32 entityId)
	{
		FillCircle(transform.GetPosition(), radius, thickness, color, entityId);
	}

	void Renderer2D::DrawCircle(const Transform &transform, float radius, const glm::vec4 &color, int32 entityId)
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
			DrawLine(p0, p1, color, entityId);
		}

		s_2DData->Statistics.CircleCount++;
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
		const Ref<Texture2D> &texture = font->GetAtlas();
		for (uint32 i = 1; i < s_2DData->FontTextureSlotIndex; ++i)
		{
			if (*s_2DData->FontTextureSlots[i].Get() == *texture.Get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_2DData->FontTextureSlotIndex;
			s_2DData->FontTextureSlots[s_2DData->FontTextureSlotIndex] = texture;
			s_2DData->FontTextureSlotIndex++;
		}

		// TODO: Change this with actual text from parameter
		uint32 charTextLength = text.Length();
		uint32 textLengthUTF8 = HLStringUTF8::UTF8StringLength(text);
		const std::vector<FontGlyph> &allGlyphs = font->GetAllGlyphs();
		const std::vector<FontKerning> &allKernings = font->GetAllKernings();

		if (textLengthUTF8 < 1)
			return;

		float x = 0.0f;
		float y = 0.0f;
		for (uint32 c = 0, uc = 0; c < charTextLength; ++c)
		{
			int32 codepoint = (int32)text.At(c);

			if (codepoint == '\n')
			{
				x = 0;
				y += font->GetLineHeight();
				uc++;
				continue;
			}

			if (codepoint == '\t')
			{
				x += font->GetTabXAdvance();
				uc++;
				continue;
			}

			uint8 advance = 0;
			if (!HLStringUTF8::FromString(text, c, &codepoint, &advance))
			{
				HL_CORE_WARN("Could not convert character {0} to codepoint! Falling back to -1", text.At(c));
				codepoint = -1;
			}

			FontGlyph *g = nullptr;
			for (uint32 i = 0; i < allGlyphs.size(); ++i)
			{
				FontGlyph glyph = allGlyphs[i];
				if (glyph.Codepoint == codepoint)
				{
					g = &glyph;
					break;
				}
			}

			// If no codepoint was found, try -1 codepoint
			if (!g)
			{
				HL_CORE_WARN("Could not convert character {0} to codepoint! Falling back to -1", text.At(c));

				codepoint = -1;
				for (uint32 i = 0; i < allGlyphs.size(); ++i)
				{
					FontGlyph glyph = allGlyphs[i];
					if (glyph.Codepoint == codepoint)
					{
						g = &glyph;
						break;
					}
				}
			}

			if (g)
			{
				// Found the glyph. generate points.
				float minx = x + g->XOffset;
				float miny = y + g->YOffset;
				float maxx = minx + g->Width;
				float maxy = miny + g->Height;
				float tminx = (float)g->X / font->GetAtlasSizeX();
				float tmaxx = (float)(g->X + g->Width) / font->GetAtlasSizeX();
				float tminy = (float)g->Y / font->GetAtlasSizeY();
				float tmaxy = (float)(g->Y + g->Height) / font->GetAtlasSizeY();
				
				// Flip the y axis for true type fonts
				if (font->GetFontType() == FontType::TRUE_TYPE_FONT)
				{
					tminy = 1.0f - tminy;
					tmaxy = 1.0f - tmaxy;
				}

				// The third component of the position will control the zIndex, but for now we don't support that
				// the fourth component will get stripped away, but has to be 1 in order for the matrix multiplication to not be affected.
				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(minx, miny, 0.0f, 1.0f);
			//	s_2DData->TextVertexBufferPtr->Position = glm::vec3(minx, miny, 0.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = glm::vec2(tminx, tminy);
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(maxx, miny, 0.0f, 1.0f);
			//	s_2DData->TextVertexBufferPtr->Position = glm::vec3(maxx, miny, 0.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = glm::vec2(tmaxx, tminy);
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(maxx, maxy, 0.0f, 1.0f);
			//	s_2DData->TextVertexBufferPtr->Position = glm::vec3(maxx, maxy, 0.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = glm::vec2(tmaxx, tmaxy);
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(minx, maxy, 0.0f, 1.0f);
			//	s_2DData->TextVertexBufferPtr->Position = glm::vec3(minx, maxy, 0.0f);
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexCoord = glm::vec2(tminx, tmaxy);
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextIndexCount += 6;

				// Try to find the kerning
				int32 kerning = 0;
				uint32 offsetOfNextChar = c + advance;
				if (offsetOfNextChar < textLengthUTF8 - 1)
				{
					int32 nextCodepoint = 0;
					uint8 nextAdvance = 0;

					if (!HLStringUTF8::FromString(text, offsetOfNextChar, &nextCodepoint, &nextAdvance))
					{
						HL_CORE_WARN("Could not convert character {0} to codepoint! Falling back to -1", text.At(offsetOfNextChar));
						nextCodepoint = -1;
					}
					else
					{
						for (uint32 i = 0; i < allKernings.size(); ++i)
						{
							FontKerning currentKerning = allKernings[i];
							if (currentKerning.Codepoint1 == codepoint && currentKerning.Codepoint2 == nextCodepoint)
							{
								kerning = currentKerning.Amount;
								break;
							}
						}
					}
				}

				x += g->XAdvance + kerning;
			}
			else
			{
				HL_CORE_ERROR("Could not find any matching glyph for character {0}. Skipping.", text.At(c));
				++uc;
				continue;
			}

			c += advance - 1;
			uc++;
		}

		s_2DData->Statistics.TextCount++;

#if 0
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

			for (uint32 i = 0; i < utf8TextLength; ++i)
			{
				char32_t ch = (char32_t)utf8Text[i];
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
				fontGeometry.getAdvance(advance, ch, utf8Text[i + 1]);
				x += fsScale * advance + kerningOffset;
			}
		}

		{
			double x = 0.0;
			double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
			double y = 0.0;

			for (uint32 i = 0; i < utf8TextLength; ++i)
			{
				char32_t ch = (char32_t)utf8Text[i];
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
				s_2DData->TextVertexBufferPtr->TexCoord = { l, b };
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(pl, pt, 0.0f, 1.0f);
				s_2DData->TextVertexBufferPtr->TexCoord = { l, t };
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(pr, pt, 0.0f, 1.0f);
				s_2DData->TextVertexBufferPtr->TexCoord = { r, t };
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextVertexBufferPtr->Position = transform.GetTransform() * glm::vec4(pr, pb, 0.0f, 1.0f);
				s_2DData->TextVertexBufferPtr->TexCoord = { r, b };
				s_2DData->TextVertexBufferPtr->Color = color;
				s_2DData->TextVertexBufferPtr->TexIndex = textureIndex;
				s_2DData->TextVertexBufferPtr++;

				s_2DData->TextIndexCount += 6;

				double advance = glyph->getAdvance();
				fontGeometry.getAdvance(advance, ch, utf8Text[i + 1]);
				x += fsScale * advance + kerningOffset;
			}
		}

		s_2DData->Statistics.TextCount++;
#endif
	}

	void Renderer2D::SetTargetRenderPass(const Ref<RenderPass> &renderPass)
	{
		if (renderPass != s_2DData->ActiveRenderPass)
		{
			VertexArraySpecification &quadSpec = s_2DData->QuadVertexArray->GetSpecification();
			quadSpec.RenderPass = renderPass;
			s_2DData->QuadVertexArray = VertexArray::Create(quadSpec);

			VertexArraySpecification &lineSpec = s_2DData->LineVertexArray->GetSpecification();
			lineSpec.RenderPass = renderPass;
			s_2DData->LineVertexArray = VertexArray::Create(lineSpec);

			VertexArraySpecification &circleSpec = s_2DData->CircleVertexArray->GetSpecification();
			circleSpec.RenderPass = renderPass;
			s_2DData->CircleVertexArray = VertexArray::Create(circleSpec);

			VertexArraySpecification &textSpec = s_2DData->TextVertexArray->GetSpecification();
			textSpec.RenderPass = renderPass;
			s_2DData->TextVertexArray = VertexArray::Create(textSpec);

			s_2DData->ActiveRenderPass = renderPass;
		}
	}

	const Ref<RenderPass> &Renderer2D::GetTargetRenderPass()
	{
		return s_2DData->ActiveRenderPass;
	}

	Renderer2DStats Renderer2D::GetStatistics()
	{
		return s_2DData->Statistics;
	}

	void Renderer2D::ResetStatistics()
	{
		memset(&s_2DData->Statistics, 0, sizeof(Renderer2DStats));
	}


}
