#include "HighLoPch.h"
#include "Renderer2D.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Framebuffer.h"

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

		// Textures
		uint32 TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		glm::vec4 QuadVertexPositions[4];
		Ref<RenderPass> RenderPass;

		bool DepthTest = true;
		float LineWidth = 1.0f;
		glm::mat4 CameraProjection = glm::mat4(1.0f);
	};

	static Renderer2DData *s_2DData;

	void Renderer2D::Init()
	{
		s_2DData = new Renderer2DData;

		// Set all texture slots to 0
		s_2DData->WhiteTexture = Renderer::GetWhiteTexture();
		s_2DData->TextureSlots[0] = s_2DData->WhiteTexture;

		s_2DData->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[1] = { -0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[3] = {  0.5f, -0.5f, 0.0f, 1.0f };

		/*
		FramebufferSpecification framebufferSpec;
		framebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::Depth };
		framebufferSpec.Samples = 1;
		framebufferSpec.ClearColor = { 0.1f, 0.5f, 0.5f, 1.0f };

		Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);

		RenderPassSpecification spec;
		spec.Framebuffer = framebuffer;
		s_2DData->RenderPass = RenderPass::Create(spec);
		*/

		s_2DData->TextureShader = Renderer::GetShaderLibrary()->Get("Renderer2DQuad");
		s_2DData->QuadVertexArray = VertexArray::Create();
		s_2DData->QuadVertexBufferBase = new QuadVertex[s_2DData->MaxVertices];

		BufferLayout textureLayout = {
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_Color", ShaderDataType::Float4 },
			{ "in_TexCoord", ShaderDataType::Float2 },
			{ "in_TexIndex", ShaderDataType::Float },
			{ "in_TilingFactor", ShaderDataType::Float },
			{ "in_EntityID", ShaderDataType::Int },
		};

		static auto cameraBuffer = UniformBuffer::Create(
			"Camera",
			{ UniformVariable("u_ViewProjection", sizeof(glm::mat4)) },
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32) HL_UB_SLOT::VS_CAMERA_BUFFER);

		s_2DData->TextureShader->AddBuffer("CameraBuffer", cameraBuffer);

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

		Ref<VertexBuffer> quadVbo = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(QuadVertex));
		quadVbo->SetLayout(textureLayout);
		s_2DData->QuadVertexArray->AddVertexBuffer(quadVbo);
		s_2DData->QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(&quadIndices[0], s_2DData->MaxIndices));
	}

	void Renderer2D::Shutdown()
	{
		delete s_2DData;
	}

	Ref<RenderPass> Renderer2D::GetRenderPass()
	{
		return s_2DData->RenderPass;
	}

	void Renderer2D::SetRenderPass(Ref<RenderPass> renderPass)
	{
		HL_ASSERT(renderPass != nullptr, "RenderPass can not be null!");
		s_2DData->RenderPass = renderPass;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_2DData->LineWidth = width;
	}

	void Renderer2D::BeginScene(const glm::mat4 &proj, bool depthTest)
	{
		s_2DData->CameraProjection = proj;
		s_2DData->DepthTest = depthTest;

		s_2DData->TextureSlotIndex = 1;

		s_2DData->TextureShader->Bind();
		Ref<UniformBuffer> buffer = s_2DData->TextureShader->GetBuffer("CameraBuffer");
		buffer->SetBufferValue(&s_2DData->CameraProjection);
		buffer->UploadToShader();

		s_2DData->QuadIndexCount = 0;
		s_2DData->QuadVertexBufferPtr = s_2DData->QuadVertexBufferBase;

	}

	void Renderer2D::EndScene()
	{
		uint32 dataSize = (uint8*)s_2DData->QuadVertexBufferPtr - (uint8*)s_2DData->QuadVertexBufferBase;
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
	}

	void Renderer2D::Flush()
	{
		if (s_2DData->QuadIndexCount == 0)
			return;

		EndScene();

		s_2DData->QuadIndexCount = 0;
		s_2DData->QuadVertexBufferPtr = s_2DData->QuadVertexBufferBase;
		s_2DData->TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		if (s_2DData->QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr uint32 quadVertexCount = 4;
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		Transform transform = Transform::FromPosition(position).Scale({ size.x, size.y, 1.0f });

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
		if (s_2DData->QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr uint32 quadVertexCount = 4;

		float textureIndex = 0.0f;
		for (uint32 i = 1; i < s_2DData->TextureSlotIndex; ++i)
		{
			if (*s_2DData->TextureSlots[i].Get() == *texture.Get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_2DData->TextureSlotIndex;
			s_2DData->TextureSlots[s_2DData->TextureSlotIndex] = texture;
			s_2DData->TextureSlotIndex++;
		}

		Transform transform = Transform::FromPosition(position).Scale({ size.x, size.y, 1.0f });

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
}