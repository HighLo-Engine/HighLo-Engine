#include "HighLoPch.h"
#include "Renderer2D.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

namespace highlo
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		float Thickness;
		glm::vec2 LocalPosition;
		glm::vec4 Color;
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

		Ref<Shader> TextureShader;
		Ref<Shader> LineShader;
		Ref<Shader> CircleShader;

		Ref<Texture2D> WhiteTexture;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexArray> LineVertexArray;
		Ref<VertexArray> CircleVertexArray;

		// Quads
		uint32 QuadIndexCount = 0;
		QuadVertex *QuadVertexBufferBase = nullptr;
		QuadVertex *QuadVertexBufferPtr = nullptr;

		// Textures
		uint32 TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		// Circle
		uint32 CircleIndexCount = 0;
		CircleVertex *CircleVertexBufferBase = nullptr;
		CircleVertex *CircleVertexBufferPtr = nullptr;

		// Lines
		uint32 LineIndexCount = 0;
		LineVertex *LineVertexBufferBase = nullptr;
		LineVertex *LineVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];

		bool DepthTest = true;
		glm::mat4 CameraProjection = glm::mat4(1.0f);
		Renderer2D::Statistics Stats;
	};

	static Renderer2DData *s_2DData;

	void Renderer2D::Init()
	{
		s_2DData = new Renderer2DData();
		s_2DData->WhiteTexture = Renderer::GetWhiteTexture();

		// Create Shaders
		BufferLayout textureLayout = {
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_Color", ShaderDataType::Float4 },
			{ "in_TexCoord", ShaderDataType::Float2 },
			{ "in_TexIndex", ShaderDataType::Float },
			{ "in_TilingFactor", ShaderDataType::Float },
		};
		BufferLayout lineLayout = {
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_Color", ShaderDataType::Float4 }
		};
		BufferLayout circleLayout = {
			{ "in_WorldPosition", ShaderDataType::Float3 },
			{ "in_Thickness", ShaderDataType::Float },
			{ "in_LocalPosition", ShaderDataType::Float2 },
			{ "in_Color", ShaderDataType::Float4 },
		};

		s_2DData->TextureShader = Renderer::GetShaderLibrary()->Get("Renderer2DQuad");
		s_2DData->LineShader = Renderer::GetShaderLibrary()->Get("Renderer2DLine");
		s_2DData->CircleShader = Renderer::GetShaderLibrary()->Get("Renderer2DCircle");

		static auto cameraBuffer = UniformBuffer::Create(
			"Camera",
			{ UniformVariable("u_ViewProjection", sizeof(glm::mat4)) },
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::VS_CAMERA_BUFFER);

		s_2DData->TextureShader->AddBuffer("CameraBuffer", cameraBuffer);
		s_2DData->LineShader->AddBuffer("CameraBuffer", cameraBuffer);
		s_2DData->CircleShader->AddBuffer("CameraBuffer", cameraBuffer);

		// Create Vertex pointers
		s_2DData->QuadVertexBufferBase = new QuadVertex[s_2DData->MaxVertices];
		s_2DData->LineVertexBufferBase = new LineVertex[s_2DData->MaxLineVertices];
		s_2DData->CircleVertexBufferBase = new CircleVertex[s_2DData->MaxVertices];

		// Create indices
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
			lineIndices.push_back(i);

		// Create VertexArray, VertexBuffers and IndexBuffers
		s_2DData->QuadVertexArray = VertexArray::Create();
		Ref<VertexBuffer> quadVertexbuffer = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(QuadVertex));
		quadVertexbuffer->SetLayout(textureLayout);
		s_2DData->QuadVertexArray->AddVertexBuffer(quadVertexbuffer);
		s_2DData->QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(&quadIndices[0], s_2DData->MaxIndices));

		s_2DData->LineVertexArray = VertexArray::Create();
		Ref<VertexBuffer> lineVertexbuffer = VertexBuffer::Create(s_2DData->MaxLineVertices * sizeof(LineVertex));
		lineVertexbuffer->SetLayout(lineLayout);
		s_2DData->LineVertexArray->AddVertexBuffer(lineVertexbuffer);
		s_2DData->LineVertexArray->SetIndexBuffer(IndexBuffer::Create(&lineIndices[0], (uint32)(lineIndices.size() * sizeof(int32))));

		s_2DData->CircleVertexArray = VertexArray::Create();
		Ref<VertexBuffer> circleVertexbuffer = VertexBuffer::Create(s_2DData->MaxVertices * sizeof(CircleVertex));
		circleVertexbuffer->SetLayout(circleLayout);
		s_2DData->CircleVertexArray->AddVertexBuffer(circleVertexbuffer);

		s_2DData->TextureSlots[0] = s_2DData->WhiteTexture;
		s_2DData->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete s_2DData;
	}

	void Renderer2D::BeginScene(const glm::mat4 &viewProj, bool depthTest)
	{
		s_2DData->CameraProjection = viewProj;
		s_2DData->DepthTest = depthTest;

		s_2DData->TextureShader->Bind();

		Ref<UniformBuffer> buffer = s_2DData->TextureShader->GetBuffer("CameraBuffer");
		buffer->SetBufferValue(&s_2DData->CameraProjection);
		buffer->UploadToShader();

		s_2DData->QuadIndexCount = 0;
		s_2DData->QuadVertexBufferPtr = s_2DData->QuadVertexBufferBase;

		s_2DData->LineIndexCount = 0;
		s_2DData->LineVertexBufferPtr = s_2DData->LineVertexBufferBase;

		s_2DData->CircleIndexCount = 0;
		s_2DData->CircleVertexBufferPtr = s_2DData->CircleVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		uint32 dataSize = (uint8*)s_2DData->QuadVertexBufferPtr - (uint8*)s_2DData->QuadVertexBufferBase;
		if (dataSize)
		{
			s_2DData->QuadVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->QuadVertexBufferBase, dataSize);

			// Update camera projection
			s_2DData->TextureShader->Bind();
			Ref<UniformBuffer> buffer = s_2DData->TextureShader->GetBuffer("CameraBuffer");
			buffer->SetBufferValue(&s_2DData->CameraProjection);
			buffer->UploadToShader();

			// Bind textures
			for (uint32 i = 0; i < s_2DData->TextureSlotIndex; ++i)
				s_2DData->TextureSlots[i]->Bind(i);

			s_2DData->QuadVertexArray->GetVertexBuffers()[0]->Bind();
			s_2DData->QuadVertexArray->GetIndexBuffer()->Bind();
			s_2DData->QuadVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->QuadIndexCount, PrimitiveType::Triangles, s_2DData->DepthTest);
			s_2DData->Stats.DrawCalls++;
		}

		dataSize = (uint8*)s_2DData->LineVertexBufferPtr - (uint8*)s_2DData->LineVertexBufferBase;
		if (dataSize)
		{
			s_2DData->LineVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->LineVertexBufferBase, dataSize);

			// Update camera projection
			s_2DData->LineShader->Bind();
			Ref<UniformBuffer> buffer = s_2DData->LineShader->GetBuffer("CameraBuffer");
			buffer->SetBufferValue(&s_2DData->CameraProjection);
			buffer->UploadToShader();

			s_2DData->LineVertexArray->GetVertexBuffers()[0]->Bind();
			s_2DData->LineVertexArray->GetIndexBuffer()->Bind();
			s_2DData->LineVertexArray->Bind();
			Renderer::s_RenderingAPI->SetLineThickness(2.0f);
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->LineIndexCount, PrimitiveType::Lines, s_2DData->DepthTest);
			s_2DData->Stats.DrawCalls++;
		}

		dataSize = (uint8*)s_2DData->CircleVertexBufferPtr - (uint8*)s_2DData->CircleVertexBufferBase;
		if (dataSize)
		{
			s_2DData->CircleVertexArray->GetVertexBuffers()[0]->UpdateContents(s_2DData->CircleVertexBufferBase, dataSize);

			// Update camera projection
			s_2DData->CircleShader->Bind();
			Ref<UniformBuffer> buffer = s_2DData->CircleShader->GetBuffer("CameraBuffer");
			buffer->SetBufferValue(&s_2DData->CameraProjection);
			buffer->UploadToShader();

			s_2DData->CircleVertexArray->GetVertexBuffers()[0]->Bind();
			s_2DData->CircleVertexArray->Bind();
			Renderer::s_RenderingAPI->DrawIndexed(s_2DData->CircleIndexCount, PrimitiveType::Triangles, s_2DData->DepthTest);
			s_2DData->Stats.DrawCalls++;
		}
	}
	
	void Renderer2D::Flush()
	{
	}

	void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color)
	{
		constexpr uint32 quadVertexCount = 4;
		const float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_2DData->QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		for (uint32 i = 0; i < quadVertexCount; ++i)
		{
			s_2DData->QuadVertexBufferPtr->Position = transform * s_2DData->QuadVertexPositions[i];
			s_2DData->QuadVertexBufferPtr->Color = color;
			s_2DData->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_2DData->QuadVertexBufferPtr++;
		}

		s_2DData->QuadIndexCount += 6;
		s_2DData->Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		constexpr uint32 quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (s_2DData->QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

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
			if (s_2DData->TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_2DData->TextureSlotIndex;
			s_2DData->TextureSlots[s_2DData->TextureSlotIndex] = texture;
			s_2DData->TextureSlotIndex++;
		}

		for (uint32 i = 0; i < quadVertexCount; ++i)
		{
			s_2DData->QuadVertexBufferPtr->Position = transform * s_2DData->QuadVertexPositions[i];
			s_2DData->QuadVertexBufferPtr->Color = color;
			s_2DData->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_2DData->QuadVertexBufferPtr++;
		}

		s_2DData->QuadIndexCount += 6;
		s_2DData->Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Transform &transform, const glm::vec4 &color)
	{
		DrawQuad(transform.GetTransform(), color);
	}

	void Renderer2D::DrawQuad(const Transform &transform, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		DrawQuad(transform.GetTransform(), texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		Transform transform = Transform::FromPosition({ position.x, position.y, 0.0f }).Scale({ size.x, size.y, 1.0f });
		DrawQuad(transform.GetTransform(), color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		Transform transform = Transform::FromPosition({ position.x, position.y, position.z }).Scale({ size.x, size.y, 1.0f });
		DrawQuad(transform.GetTransform(), color);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		Transform transform = Transform::FromPosition({ position.x, position.y, 0.0f }).Scale({ size.x, size.y, 1.0f });
		DrawQuad(transform.GetTransform(), texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		Transform transform = Transform::FromPosition({ position.x, position.y, position.z }).Scale({ size.x, size.y, 1.0f });
		DrawQuad(transform.GetTransform(), texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawCircle(const glm::vec2 &p0, float radius, const glm::vec4 &color, float thickness)
	{
		DrawCircle({ p0.x, p0.y, 0.0f }, radius, color, thickness);
	}
	
	void Renderer2D::DrawCircle(const glm::vec3 &p0, float radius, const glm::vec4 &color, float thickness)
	{
		if (s_2DData->CircleIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		Transform transform = Transform::FromPosition({ p0.x, p0.y, p0.z }).Scale({ radius * 2.0f, radius * 2.0f, 1.0f });

		for (int32 i = 0; i < 4; ++i)
		{
			s_2DData->CircleVertexBufferPtr->WorldPosition = transform.GetTransform() * s_2DData->QuadVertexPositions[i];
			s_2DData->CircleVertexBufferPtr->Thickness = thickness;
			s_2DData->CircleVertexBufferPtr->LocalPosition = s_2DData->QuadVertexPositions[i] * 2.0f;
			s_2DData->CircleVertexBufferPtr->Color = color;
			s_2DData->CircleVertexBufferPtr++;

			s_2DData->CircleIndexCount += 6;
			s_2DData->Stats.CircleCount++;
		}
	}

	void Renderer2D::DrawLine(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec4 &color)
	{
		DrawLine(glm::vec3{ p0.x, p0.y, 0.0f }, glm::vec3{ p1.x, p1.y, 0.0f }, color);
	}
	
	void Renderer2D::DrawLine(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color)
	{
		if (s_2DData->LineIndexCount >= Renderer2DData::MaxLineIndices)
			FlushAndResetLines();

		s_2DData->LineVertexBufferPtr->Position = p0;
		s_2DData->LineVertexBufferPtr->Color = color;
		s_2DData->LineVertexBufferPtr++;

		s_2DData->LineVertexBufferPtr->Position = p1;
		s_2DData->LineVertexBufferPtr->Color = color;
		s_2DData->LineVertexBufferPtr++;

		s_2DData->LineIndexCount += 2;
		s_2DData->Stats.LineCount++;
	}

	void Renderer2D::ResetStatistics()
	{
		memset(&s_2DData->Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_2DData->Stats;
	}
	
	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_2DData->QuadIndexCount = 0;
		s_2DData->TextureSlotIndex = 1;

		s_2DData->QuadVertexBufferPtr = s_2DData->QuadVertexBufferBase;
	}
	
	void Renderer2D::FlushAndResetLines()
	{
	}
}