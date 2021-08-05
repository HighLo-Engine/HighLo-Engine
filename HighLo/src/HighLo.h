#pragma once

#include "Engine/Application/HLApplication.h"

#include "Engine/Core/defines/HLDefines.h"
#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLAssert.h"
#include "Engine/Core/HLAllocator.h"
#include "Engine/Core/DataTypes/HLDataTypes.h"
#include "Engine/Core/HLLog.h"
#include "Engine/Core/HLInput.h"
#include "Engine/Core/HLTime.h"
#include "Engine/Core/HLTimer.h"
#include "Engine/Core/SharedReference.h"
#include "Engine/Core/HLFileSystem.h"
#include "Engine/Core/HLVirtualFileSystem.h"
#include "Engine/Core/Profiler/HLProfiler.h"
#include "Engine/Core/Profiler/HLProfilerTimer.h"

#include "Engine/Math/HLMath.h"
#include "Engine/Math/HLAABB.h"
#include "Engine/Math/HLRay.h"
#include "Engine/Math/Transform.h"

#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/RenderingContext.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexData.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/CoreRenderer.h"
#include "Engine/Renderer/RenderingAPI.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/TextureFormat.h"
#include "Engine/Renderer/Environment.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/ShaderLibrary.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "Engine/Renderer/UniformBufferSlotMappings.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/MeshFactory.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommandBuffer.h"
#include "Engine/Renderer/ImGuiRenderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Skybox.h"

#include "Engine/Camera/Camera.h"
#include "Engine/Camera/FPSCamera.h"
#include "Engine/Camera/FreeFlyCamera.h"

#include "Engine/Window/Window.h"
#include "Engine/Window/FileDialogue.h"
#include "Engine/Window/FileDialogueFilter.h"
#include "Engine/Window/MenuItem.h"
#include "Engine/Window/PopupMenuItem.h"
#include "Engine/Window/FileMenu.h"
#include "Engine/Window/PopupMenu.h"
#include "Engine/Window/MenuBar.h"

#include "Engine/ECS/Entity.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Loaders/AssetLoader.h"

#include "Engine/Encryption/Base64.h"
#include "Engine/Encryption/Encryptor.h"

#include "Engine/Serialization/Serializable.h"
#include "Engine/Serialization/Serializer.h"

// TODO: include this only if flag is active
#include "HighLo-Unit.h"

