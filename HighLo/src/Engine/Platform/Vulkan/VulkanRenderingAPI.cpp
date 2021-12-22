// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#include "Engine/Renderer/SceneRenderer.h"
#include "Engine/Renderer/Shaders/UniformBufferSet.h"
#include "Engine/Renderer/Shaders/StorageBufferSet.h"
#include "Engine/Utils/ImageUtils.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanShader.h"
#include "VulkanComputePipeline.h"
#include "VulkanTexture2D.h"
#include "VulkanTexture3D.h"

namespace highlo
{
	namespace utils
	{
		static const char *VulkanVendorIDToString(uint32 vendorID)
		{
			switch (vendorID)
			{
				case 0x10DE: return "NVIDIA";
				case 0x1002: return "AMD";
				case 0x8086: return "INTEL";
				case 0x13B5: return "ARM";
			}
			return "Unknown";
		}
	}

	struct VulkanRendererData
	{
		int32 SelectedDrawCall = -1;
		int32 DrawCallCount = 0;

		std::unordered_map<SceneRenderer*, std::vector<VulkanShader::ShaderMaterialDescriptorSet>> RenderDescriptorSets;
		VkDescriptorSet ActiveRenderDescriptorSet = nullptr;
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32> DescriptorPoolAllocationCounts;

		// Default Sampler
		VkSampler SamplerClamp = nullptr;

		// UniformBufferSet -> Shader Hash -> Frame -> WriteDescriptor
		std::unordered_map<UniformBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> UniformBufferWriteDescriptorCache;
		std::unordered_map<StorageBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> StorageBufferWriteDescriptorCache;
	};

	static VulkanRendererData *s_VulkanRendererData = nullptr;

	void VulkanRenderingAPI::Init()
	{
		s_VulkanRendererData = new VulkanRendererData();

		const auto &config = Renderer::GetConfig();
		auto &properties = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetProperties();
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		s_VulkanRendererData->DescriptorPools.resize(config.FramesInFlight);
		s_VulkanRendererData->DescriptorPoolAllocationCounts.resize(config.FramesInFlight);

		auto &caps = Renderer::GetCapabilities();
		caps.Vendor = utils::VulkanVendorIDToString(properties.vendorID);
		caps.Device = properties.deviceName;
		caps.Version = HLString::ToString(properties.driverVersion);
		caps.MaxSamples = properties.limits.maxSamplerAllocationCount;
		caps.MaxAnisotropy = properties.limits.maxSamplerAnisotropy;
		
		// TODO
		//caps.MaxTextureUnits = properties.limits.;

		utils::DumpGPUInfos();

		// Create Descriptor Pool
		VkDescriptorPoolSize poolSizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 100000;
		poolInfo.poolSizeCount = (uint32)IM_ARRAYSIZE(poolSizes);
		poolInfo.pPoolSizes = poolSizes;
		for (uint32 i = 0; i < config.FramesInFlight; i++)
		{
			VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &s_VulkanRendererData->DescriptorPools[i]));
			s_VulkanRendererData->DescriptorPoolAllocationCounts[i] = 0;
		}
	}
	
	void VulkanRenderingAPI::Shutdown()
	{
		VulkanShader::ClearUniformBuffers();
		delete s_VulkanRendererData;
	}
	
	void VulkanRenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
	}
	
	void VulkanRenderingAPI::ClearScreenBuffers()
	{
	}
	
	void VulkanRenderingAPI::DrawIndexed(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexed(uint32 indexCount, PrimitiveType type, bool depthTest)
	{
	}
	
	void VulkanRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
		Ref<VulkanSwapChain> &swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();

		// Reset descriptor pools here
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		uint32 bufferIndex = swapChain->GetCurrentBufferIndex();
		vkResetDescriptorPool(device, s_VulkanRendererData->DescriptorPools[bufferIndex], 0);
		memset(s_VulkanRendererData->DescriptorPoolAllocationCounts.data(), 0, s_VulkanRendererData->DescriptorPoolAllocationCounts.size() * sizeof(uint32));

		s_VulkanRendererData->DrawCallCount = 0;
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}
	
	void VulkanRenderingAPI::SetWireframe(bool wf)
	{
	}
	
	void VulkanRenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
	}
	
	void VulkanRenderingAPI::SetBlendMode(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetMultiSample(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetDepthTest(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetLineThickness(float thickness)
	{
	}

	Ref<Environment> VulkanRenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		if (!Renderer::GetConfig().ComputeEnvironmentMaps)
			return Renderer::GetEmptyEnvironment();

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		Ref<Texture2D> envEquirect = Texture2D::LoadFromFile(filePath);
		HL_ASSERT(envEquirect->GetFormat() == TextureFormat::RGBA32F, "Texture is not HDR!");

		Ref<Texture3D> envUnfiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> envFiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);

		// ========================================================================================================================================
		// ============================================ Convert equirectangular to cubemap ========================================================
		// ========================================================================================================================================
		Ref<VulkanComputePipeline> equirectangularConversionPipeline = Ref<VulkanComputePipeline>::Create(Renderer::GetShaderLibrary()->Get("EquirectangularToCubeMap"));
		Ref<VulkanShader> equirectangularConversionShader = equirectangularConversionPipeline->GetShader();


		std::array<VkWriteDescriptorSet, 2> equirectangularConversionWriteDescriptors;
		auto equirectangularConversionDescriptorSet = equirectangularConversionShader->CreateDescriptorSets();

		Ref<VulkanTexture3D> envUnfilteredCubemap = envUnfiltered.As<VulkanTexture3D>();
		equirectangularConversionWriteDescriptors[0] = *equirectangularConversionShader->GetDescriptorSet("o_CubeMap");
		equirectangularConversionWriteDescriptors[0].dstSet = equirectangularConversionDescriptorSet.DescriptorSets[0];
		equirectangularConversionWriteDescriptors[0].pImageInfo = &envUnfilteredCubemap->GetVulkanDescriptorInfo();

		Ref<VulkanTexture2D> envEquirectVK = envEquirect.As<VulkanTexture2D>();
		equirectangularConversionWriteDescriptors[1] = *equirectangularConversionShader->GetDescriptorSet("o_EquirectangularTex");
		equirectangularConversionWriteDescriptors[1].dstSet = equirectangularConversionDescriptorSet.DescriptorSets[0];
		equirectangularConversionWriteDescriptors[1].pImageInfo = &envEquirectVK->GetVulkanDescriptorInfo();

		vkUpdateDescriptorSets(device, (uint32)equirectangularConversionWriteDescriptors.size(), equirectangularConversionWriteDescriptors.data(), 0, NULL);
		equirectangularConversionPipeline->Execute(equirectangularConversionDescriptorSet.DescriptorSets.data(),
												   (uint32)equirectangularConversionDescriptorSet.DescriptorSets.size(),
												   cubemapSize / 32,
												   cubemapSize / 32,
												   6);

		envUnfilteredCubemap->GenerateMips(true);

		// ========================================================================================================================================
		// ===================================== Create Environment Mips and calculate compute passes =============================================
		// ========================================================================================================================================

		Ref<VulkanComputePipeline> environmentMipFilterPipeline = Ref<VulkanComputePipeline>::Create(Renderer::GetShaderLibrary()->Get("EnvironmentMipFilter"));
		Ref<VulkanShader> environmentMipFilterShader = environmentMipFilterPipeline->GetShader();

		Ref<VulkanTexture3D> envFilteredCubemap = envFiltered.As<VulkanTexture3D>();
		VkDescriptorImageInfo imageInfo = envFilteredCubemap->GetVulkanDescriptorInfo();

		uint32 mipCount = utils::CalculateMipCount(cubemapSize, cubemapSize);

		std::vector<VkWriteDescriptorSet> environmentMipWriteDescriptors(mipCount * 2);
		std::vector<VkDescriptorImageInfo> mipImageInfos(mipCount);
		auto environmentMipFilterDescriptorSet = environmentMipFilterShader->CreateDescriptorSets(0, 12);
		for (uint32 i = 0; i < mipCount; i++)
		{
			VkDescriptorImageInfo &mipImageInfo = mipImageInfos[i];
			mipImageInfo = imageInfo;
			mipImageInfo.imageView = envFilteredCubemap->CreateImageViewSingleMip(i);

			environmentMipWriteDescriptors[i * 2 + 0] = *environmentMipFilterShader->GetDescriptorSet("outputTexture");
			environmentMipWriteDescriptors[i * 2 + 0].dstSet = environmentMipFilterDescriptorSet.DescriptorSets[i]; // Should this be set inside the shader?
			environmentMipWriteDescriptors[i * 2 + 0].pImageInfo = &mipImageInfo;

			Ref<VulkanTexture3D> envUnfilteredCubemap = envUnfiltered.As<VulkanTexture3D>();
			environmentMipWriteDescriptors[i * 2 + 1] = *environmentMipFilterShader->GetDescriptorSet("inputTexture");
			environmentMipWriteDescriptors[i * 2 + 1].dstSet = environmentMipFilterDescriptorSet.DescriptorSets[i]; // Should this be set inside the shader?
			environmentMipWriteDescriptors[i * 2 + 1].pImageInfo = &envUnfilteredCubemap->GetVulkanDescriptorInfo();
		}

		vkUpdateDescriptorSets(device, (uint32)environmentMipWriteDescriptors.size(), environmentMipWriteDescriptors.data(), 0, NULL);

		environmentMipFilterPipeline->Begin(); // begin compute pass
		const float deltaRoughness = 1.0f / glm::max((float)envFiltered->GetMipLevelCount() - 1.0f, 1.0f);
		for (uint32 i = 0, size = cubemapSize; i < mipCount; i++, size /= 2)
		{
			uint32 numGroups = glm::max(1u, size / 32);
			float roughness = i * deltaRoughness;
			roughness = glm::max(roughness, 0.05f);
			environmentMipFilterPipeline->SetPushConstants(&roughness, sizeof(float));
			environmentMipFilterPipeline->Dispatch(environmentMipFilterDescriptorSet.DescriptorSets[i], numGroups, numGroups, 6);
		}
		environmentMipFilterPipeline->End();

		// ========================================================================================================================================
		// =================================================== Create Irradiance map ==============================================================
		// ========================================================================================================================================

		Ref<Texture3D> irradianceMap = Texture3D::Create(TextureFormat::RGBA32F, irradianceMapSize, irradianceMapSize);
		Ref<VulkanComputePipeline> environmentIrradiancePipeline = Ref<VulkanComputePipeline>::Create(Renderer::GetShaderLibrary()->Get("EnvironmentIrradiance"));
		Ref<VulkanShader> environmentIrradianceShader = environmentIrradiancePipeline->GetShader();

		Ref<VulkanTexture3D> irradianceCubemap = irradianceMap.As<VulkanTexture3D>();
		auto environmentIrradianceDescriptorSet = environmentIrradianceShader->CreateDescriptorSets();

		std::array<VkWriteDescriptorSet, 2> irradianceWriteDescriptors;
		irradianceWriteDescriptors[0] = *environmentIrradianceShader->GetDescriptorSet("o_IrradianceMap");
		irradianceWriteDescriptors[0].dstSet = environmentIrradianceDescriptorSet.DescriptorSets[0];
		irradianceWriteDescriptors[0].pImageInfo = &irradianceCubemap->GetVulkanDescriptorInfo();

		irradianceWriteDescriptors[1] = *environmentIrradianceShader->GetDescriptorSet("u_RadianceMap");
		irradianceWriteDescriptors[1].dstSet = environmentIrradianceDescriptorSet.DescriptorSets[0];
		irradianceWriteDescriptors[1].pImageInfo = &envFilteredCubemap->GetVulkanDescriptorInfo();

		vkUpdateDescriptorSets(device, (uint32)irradianceWriteDescriptors.size(), irradianceWriteDescriptors.data(), 0, NULL);
		environmentIrradiancePipeline->Begin();
		environmentIrradiancePipeline->SetPushConstants(&Renderer::GetConfig().IrradianceMapComputeSamples, sizeof(uint32));
		environmentIrradiancePipeline->Dispatch(environmentIrradianceDescriptorSet.DescriptorSets[0], irradianceMap->GetWidth() / 32, irradianceMap->GetHeight() / 32, 6);
		environmentIrradiancePipeline->End();

		irradianceCubemap->GenerateMips();

		return Ref<Environment>::Create(envUnfiltered, envFiltered, irradianceMap, nullptr);
	}
	
	VkDescriptorSet VulkanRenderingAPI::AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo)
	{
		VkDescriptorSet result;
		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		allocInfo.descriptorPool = s_VulkanRendererData->DescriptorPools[bufferIndex];
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &result));
		s_VulkanRendererData->DescriptorPoolAllocationCounts[bufferIndex] += allocInfo.descriptorSetCount;
		return result;
	}
}

#endif // HIGHLO_API_VULKAN

