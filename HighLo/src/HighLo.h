// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-21) Added Font include
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Application/Application.h"
#include "Engine/Application/ApplicationLayer.h"
#include "Engine/Application/ApplicationLayerStack.h"

#include "Engine/Assets/Asset.h"
#include "Engine/Assets/AssetExtensions.h"
#include "Engine/Assets/AssetManager.h"
#include "Engine/Assets/AssetTypes.h"

#include "Engine/Core/Defines/Defines.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/Assert.h"
#include "Engine/Core/Allocator.h"
#include "Engine/Core/DataTypes/DataTypes.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"
#include "Engine/Core/Timer.h"
#include "Engine/Core/SharedReference.h"
#include "Engine/Core/FileSystem.h"
#include "Engine/Core/File.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Core/VirtualFileSystem.h"
#include "Engine/Core/Profiler/Profiler.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"

#include "Engine/Math/Math.h"
#include "Engine/Math/AABB.h"
#include "Engine/Math/Ray.h"
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
#include "Engine/Renderer/MaterialTable.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommandBuffer.h"
#include "Engine/Renderer/Font.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Skybox.h"
#include "Engine/ImGui/ImGui.h"
#include "Engine/ImGui/ImGuiScopeHelpers.h"

#include "Engine/Camera/Camera.h"
#include "Engine/Camera/FPSCamera.h"
#include "Engine/Camera/FreeFlyCamera.h"
#include "Engine/Camera/EditorCamera.h" // TODO: this should be editor-only

#include "Engine/Editor/EditorColors.h"
#include "Engine/Editor/SceneHierarchyPanel.h"

#include "Engine/Window/Window.h"
#include "Engine/Window/FileDialogue.h"
#include "Engine/Window/FileDialogueFilter.h"
#include "Engine/Window/MenuItem.h"
#include "Engine/Window/PopupMenuItem.h"
#include "Engine/Window/FileMenu.h"
#include "Engine/Window/PopupMenu.h"
#include "Engine/Window/MenuBar.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/ECS_Registry.h"
#include "Engine/ECS/ECS_SystemManager.h"
#include "Engine/ECS/RenderSystem.h"
#include "Engine/ECS/Prefab.h"

#include "Engine/Core/Exceptions/Exceptions.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Project.h"
#include "Engine/Scene/UserSettings.h"

#include "Engine/Loaders/AssetLoader.h"

#include "Engine/Encryption/Base64.h"
#include "Engine/Encryption/Encryptor.h"

#include "Engine/Serialization/Serializable.h"
#include "Engine/Serialization/Serializer.h"

#include "Engine/Utils/Utils.h"

#include "HighLo-Unit.h" // TODO: include this only if flag is active

