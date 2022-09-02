// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-02) initial release
//


#pragma once

#include "VertexArray.h"

namespace highlo
{
	class VertexArrayBuilder
	{
	public:

		HLAPI VertexArrayBuilder();
		HLAPI virtual ~VertexArrayBuilder();

		HLAPI VertexArrayBuilder &SetShader(const Ref<Shader> &shader);
		HLAPI VertexArrayBuilder &SetRenderPass(const Ref<RenderPass> &renderPass);
		HLAPI VertexArrayBuilder &SetPrimitiveType(PrimitiveType renderType);
		HLAPI VertexArrayBuilder &SetDepthCompareOperator(DepthCompareOperator depthOperator);
		HLAPI VertexArrayBuilder &Layout(BufferLayout layout);
		HLAPI VertexArrayBuilder &InstanceLayout(BufferLayout instanceLayout);
		HLAPI VertexArrayBuilder &LineWidth(float width);
		HLAPI VertexArrayBuilder &DebugName(const HLString &debugName);
		
		HLAPI VertexArrayBuilder &WithBackFaceCulling();
		HLAPI VertexArrayBuilder &WithDepthTest();
		HLAPI VertexArrayBuilder &WithDepthWrite();
		HLAPI VertexArrayBuilder &WithWireframe();

		HLAPI Ref<VertexArray> Build(const VertexArraySpecification *overrideSpec = nullptr);

	private:

		VertexArraySpecification m_TargetSpec;
	};
}

