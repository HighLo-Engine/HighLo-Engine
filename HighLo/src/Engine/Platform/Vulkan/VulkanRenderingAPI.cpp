#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"
#include "VulkanContext.h"
#include "VulkanShaderDefs.h"

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

			return "unknown";
		}
	}

	struct VulkanRendererData
	{
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		VulkanShaderMaterialDescriptorSet QuadDescriptorSet;



		// Default samplers
		VkSampler SamplerClamp = nullptr;
		VkSampler SamplerPoint = nullptr;

		// UniformBufferSet -> Shader Hash -> Frame -> WriteDescriptor
		std::unordered_map<UniformBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> UniformBufferWriteDescriptorCache;
		std::unordered_map<StorageBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> StorageBufferWriteDescriptorCache;

		int32 SelectedDrawCall = -1;
		int32 DrawCallCount = 0;
	};

	static VulkanRendererData *s_VulkanRendererData = nullptr;

	void VulkanRenderingAPI::Init()
	{
		utils::InitAllocator(VulkanContext::GetCurrentDevice());
		s_VulkanRendererData = new VulkanRendererData();

		auto &caps = Renderer::GetCapabilities();
		auto &properties = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetProperties();
		caps.Vendor = utils::VulkanVendorIDToString(properties.vendorID);
		caps.Device = properties.deviceName;
		caps.Version = HLString::ToString(properties.driverVersion);
		utils::DumpGPUInfos();


	}
	
	void VulkanRenderingAPI::Shutdown()
	{
		utils::ShutdownAllocator();
		delete s_VulkanRendererData;
		s_VulkanRendererData = nullptr;
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}
	
	void VulkanRenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
	}
	
	void VulkanRenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
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
	
	void VulkanRenderingAPI::DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type, bool depthTest, const glm::mat4 &localTransform)
	{
	}
	
	void VulkanRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4 &transform)
	{
	}
	
	void VulkanRenderingAPI::DrawStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawDynamicMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMeshWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> overrideMaterial)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMeshWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> overrideMaterial)
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
		return Ref<Environment>();
	}
	
	Ref<Texture3D> VulkanRenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		return Ref<Texture3D>();
	}
	
	VkDescriptorSet VulkanRenderingAPI::AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo)
	{
		return VkDescriptorSet();
	}

	void VulkanRenderingAPI::UpdateMaterialForRendering(Ref<VulkanMaterial> &material, Ref<UniformBufferSet> &uniformBufferSet, Ref<StorageBufferSet> &storageBufferSet)
	{
	}
	
	VkSampler VulkanRenderingAPI::GetClampSampler()
	{
		return VkSampler();
	}
	
	VkSampler VulkanRenderingAPI::GetPointSampler()
	{
		return VkSampler();
	}
	
	uint32 VulkanRenderingAPI::GetDescriptorAllocationCount(uint32 frameIndex)
	{
		return 0;
	}
	
	int32 &VulkanRenderingAPI::GetSelectedDrawCall()
	{
		return s_VulkanRendererData->SelectedDrawCall;
	}
}

#endif // HIGHLO_API_VULKAN

