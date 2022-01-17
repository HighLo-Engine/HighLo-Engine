// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.8 (2022-01-12) Added DocumentWriter
//     - 1.7 (2021-11-18) Removed HighLo-Unit
//     - 1.6 (2021-11-01) Added Threading headers
//     - 1.5 (2021-10-23) Added Atomic and Service
//     - 1.4 (2021-10-19) Added RenderCommandQueue
//     - 1.3 (2021-10-04) Added FileSystemWatcher
//     - 1.2 (2021-09-26) Added Color, AssetEditor, AssetEditorPanel include
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
#include "Engine/Core/Atomic.h"
#include "Engine/Core/Service.h"
#include "Engine/Core/Assert.h"
#include "Engine/Core/Allocator.h"
#include "Engine/Core/DataTypes/DataTypes.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"
#include "Engine/Core/Timer.h"
#include "Engine/Core/SharedReference.h"
#include "Engine/Core/FileSystem.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Core/FileSystemWatcher.h"
#include "Engine/Core/VirtualFileSystem.h"
#include "Engine/Core/Profiler/Profiler.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"

#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/ImGuiTreeNode.h"
#include "Engine/ImGui/ImGuiWidgets.h"
#include "Engine/ImGui/ImGuiScopeHelpers.h"
#include "Engine/ImGui/ImGuizmo.h"

#include "Engine/Math/Color.h"
#include "Engine/Math/Math.h"
#include "Engine/Math/AABB.h"
#include "Engine/Math/Ray.h"
#include "Engine/Math/Transform.h"

#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/RenderingContext.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/CoreRenderer.h"
#include "Engine/Renderer/RenderingAPI.h"
#include "Engine/Renderer/Texture2D.h"
#include "Engine/Renderer/Texture3D.h"
#include "Engine/Renderer/TextureFormat.h"
#include "Engine/Renderer/Environment.h"
#include "Engine/Renderer/Shaders/Shader.h"
#include "Engine/Renderer/Shaders/ShaderLibrary.h"
#include "Engine/Renderer/Shaders/UniformBuffer.h"
#include "Engine/Renderer/Meshes/StaticModel.h"
#include "Engine/Renderer/Meshes/DynamicModel.h"
#include "Engine/Factories/MeshFactory.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/MaterialTable.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/CommandBuffer.h"
#include "Engine/Renderer/RenderCommandQueue.h"
#include "Engine/Renderer/Font.h"
#include "Engine/Renderer/FontManager.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Skybox.h"
#include "Engine/ImGui/ImGui.h"
#include "Engine/ImGui/ImGuiScopeHelpers.h"

#include "Engine/Camera/Camera.h"
#include "Engine/Camera/FPSCamera.h"
#include "Engine/Camera/FreeFlyCamera.h"
#include "Engine/Camera/EditorCamera.h" // TODO: this should be editor-only

#include "Engine/Threading/LocalThread.h"
#include "Engine/Threading/Runnable.h"
#include "Engine/Threading/Task.h"
#include "Engine/Threading/Thread.h"
#include "Engine/Threading/ThreadLocker.h"
#include "Engine/Threading/ThreadRegistry.h"
#include "Engine/Threading/ThreadPool.h"

#include "Engine/Editor/EditorColors.h"
#include "Engine/Editor/SceneHierarchyPanel.h"
#include "Engine/Editor/EditorConsolePanel.h"
#include "Engine/Editor/AssetEditor.h"
#include "Engine/Editor/AssetEditorPanel.h"

#include "Engine/Editor/AssetBrowser/AssetBrowserItem.h"
#include "Engine/Editor/AssetBrowser/AssetBrowserItemList.h"
#include "Engine/Editor/AssetBrowser/SelectionStack.h"
#include "Engine/Editor/AssetBrowser/AssetBrowserPanel.h"

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

#include "Engine/Loaders/MeshLoader.h"
#include "Engine/Loaders/DocumentWriter.h"

#include "Engine/Encryption/Base64.h"
#include "Engine/Encryption/Encryptor.h"

#include "Engine/Serialization/Serializable.h"
#include "Engine/Serialization/Serializer.h"

#include "Engine/Utils/Utils.h"

