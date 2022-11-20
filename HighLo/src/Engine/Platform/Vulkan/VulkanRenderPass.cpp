// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderPass.h"

namespace highlo
{
    VulkanRenderPass::VulkanRenderPass(const RenderPassSpecification &spec)
        : m_Specification(spec)
    {
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
    }
}

