// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VertexArrayBuilder.h"

namespace highlo
{
	VertexArrayBuilder::VertexArrayBuilder()
	{
		m_TargetSpec.BackfaceCulling = false;
		m_TargetSpec.DepthTest = false;
		m_TargetSpec.DepthWrite = false;
		m_TargetSpec.Wireframe = false;
		m_TargetSpec.Layout = {};
		m_TargetSpec.InstanceLayout = {};
	}

	VertexArrayBuilder::~VertexArrayBuilder()
	{
	}

	VertexArrayBuilder &VertexArrayBuilder::SetShader(const Ref<Shader> &shader)
	{
		m_TargetSpec.Shader = shader;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::SetRenderPass(const Ref<RenderPass> &renderPass)
	{
		m_TargetSpec.RenderPass = renderPass;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::SetPrimitiveType(PrimitiveType renderType)
	{
		m_TargetSpec.RenderType = renderType;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::SetDepthCompareOperator(DepthCompareOperator depthOperator)
	{
		m_TargetSpec.DepthCompareOp = depthOperator;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::Layout(BufferLayout layout)
	{
		m_TargetSpec.Layout = layout;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::InstanceLayout(BufferLayout instanceLayout)
	{
		m_TargetSpec.InstanceLayout = instanceLayout;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::LineWidth(float width)
	{
		m_TargetSpec.LineWidth = width;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::DebugName(const HLString &debugName)
	{
		m_TargetSpec.DebugName = debugName;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::WithBackFaceCulling()
	{
		m_TargetSpec.BackfaceCulling = true;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::WithDepthTest()
	{
		m_TargetSpec.DepthTest = true;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::WithDepthWrite()
	{
		m_TargetSpec.DepthWrite = true;
		return *this;
	}

	VertexArrayBuilder &VertexArrayBuilder::WithWireframe()
	{
		m_TargetSpec.Wireframe = true;
		return *this;
	}

	Ref<VertexArray> VertexArrayBuilder::Build(const VertexArraySpecification *overrideSpec)
	{
		if (overrideSpec)
			return VertexArray::Create(*overrideSpec);

		return VertexArray::Create(m_TargetSpec);
	}
}

