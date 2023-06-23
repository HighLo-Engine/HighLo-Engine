// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-06-21) initial release
//

#pragma once

#include "AssimpUtils.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Engine/Animation/Animation.h"
#include "Engine/Animation/Skeleton.h"

namespace highlo::utils
{
	class BoneHierarchy
	{
	public:
		BoneHierarchy(const aiScene *scene)
			: m_Scene(scene)
		{

		}

		void ExtractBones()
		{
			// Note: ASSIMP does not appear to support import of digital content files that contain _only_ an armature/skeleton and no mesh.
			for (uint32 meshIndex = 0; meshIndex < m_Scene->mNumMeshes; ++meshIndex)
			{
				const aiMesh *mesh = m_Scene->mMeshes[meshIndex];
				for (uint32 boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
				{
					m_Bones.emplace(mesh->mBones[boneIndex]->mName.C_Str());
				}
			}
		}

		void TraverseNode(aiNode *node, Skeleton *skeleton)
		{
			if (m_Bones.find(node->mName.C_Str()) != m_Bones.end())
			{
				TraverseBone(node, skeleton, Skeleton::NullIndex);
			}
			else
			{
				for (uint32 nodeIndex = 0; nodeIndex < node->mNumChildren; ++nodeIndex)
				{
					TraverseNode(node->mChildren[nodeIndex], skeleton);
				}
			}
		}

		void TraverseBone(aiNode *node, Skeleton *skeleton, uint32 parentIndex)
		{
			uint32 boneIndex = skeleton->AddBone(node->mName.C_Str(), parentIndex, utils::Mat4FromAIMatrix4x4(node->mTransformation));
			for (uint32 nodeIndex = 0; nodeIndex < node->mNumChildren; ++nodeIndex)
			{
				TraverseBone(node->mChildren[nodeIndex], skeleton, boneIndex);
			}
		}

		UniqueReference<Skeleton> CreateSkeleton()
		{
			if (!m_Scene)
			{
				return nullptr;
			}

			ExtractBones();
			if (m_Bones.empty())
			{
				return nullptr;
			}

			auto skeleton = UniqueReference<Skeleton>::Create(static_cast<uint32>(m_Bones.size()));
			TraverseNode(m_Scene->mRootNode, skeleton.Get());

			return skeleton;
		}

	private:

		std::set<HLString> m_Bones;
		const aiScene *m_Scene;
	};

	template<typename T> 
	struct KeyFrame
	{
		float FrameTime;
		T Value;
		KeyFrame(const float frameTime, const T &value) : FrameTime(frameTime), Value(value) {}
	};


	struct Channel
	{
		std::vector<KeyFrame<glm::vec3>> Translations;
		std::vector<KeyFrame<glm::quat>> Rotations;
		std::vector<KeyFrame<glm::vec3>> Scales;
		uint32 Index;
	};

	static auto ExtractKeys(const std::vector<std::pair<float, TranslationKey>> &translationKeysTemp, const std::vector<std::pair<float, RotationKey>> &rotationKeysTemp, const std::vector<std::pair<float, ScaleKey>> &scaleKeysTemp)
	{
		std::vector<TranslationKey> translationKeys;
		std::vector<RotationKey> rotationKeys;
		std::vector<ScaleKey> scaleKeys;
		translationKeys.reserve(translationKeysTemp.size());
		rotationKeys.reserve(rotationKeysTemp.size());
		scaleKeys.reserve(scaleKeysTemp.size());
		for (const auto &translation : translationKeysTemp)
		{
			translationKeys.emplace_back(translation.second);
		}
		for (const auto &rotation : rotationKeysTemp)
		{
			rotationKeys.emplace_back(rotation.second);
		}
		for (const auto &scale : scaleKeysTemp)
		{
			scaleKeys.emplace_back(scale.second);
		}

		return std::tuple{ translationKeys, rotationKeys, scaleKeys };
	}

	static auto ConcatenateChannelsAndSort(const std::vector<Channel> &channels)
	{
		// We concatenate the translations for all the channels into one big long vector, and then sort
		// it on _previous_ frame time.  This gives us an efficient way to sample the key frames later on.
		// (taking advantage of fact that animation almost always plays forwards)

		uint32 numTranslations = 0;
		uint32 numRotations = 0;
		uint32 numScales = 0;

		for (auto channel : channels)
		{
			numTranslations += static_cast<uint32>(channel.Translations.size());
			numRotations += static_cast<uint32>(channel.Rotations.size());
			numScales += static_cast<uint32>(channel.Scales.size());
		}

		std::vector<std::pair<float, TranslationKey>> translationKeysTemp;
		std::vector<std::pair<float, RotationKey>> rotationKeysTemp;
		std::vector<std::pair<float, ScaleKey>> scaleKeysTemp;
		translationKeysTemp.reserve(numTranslations);
		rotationKeysTemp.reserve(numRotations);
		scaleKeysTemp.reserve(numScales);
		for (const auto &channel : channels)
		{
			float prevFrameTime = -1.0f;
			for (const auto &translation : channel.Translations)
			{
				translationKeysTemp.emplace_back(prevFrameTime, TranslationKey{ translation.FrameTime, channel.Index, translation.Value });
				prevFrameTime = translation.FrameTime;
			}

			prevFrameTime = -1.0f;
			for (const auto &rotation : channel.Rotations)
			{
				rotationKeysTemp.emplace_back(prevFrameTime, RotationKey{ rotation.FrameTime, channel.Index, rotation.Value });
				prevFrameTime = rotation.FrameTime;
			}

			prevFrameTime = -1.0f;
			for (const auto &scale : channel.Scales)
			{
				scaleKeysTemp.emplace_back(prevFrameTime, ScaleKey{ scale.FrameTime, channel.Index, scale.Value });
				prevFrameTime = scale.FrameTime;
			}
		}
		std::sort(translationKeysTemp.begin(), translationKeysTemp.end(), [](const auto &a, const auto &b) { return (a.first < b.first) || ((a.first == b.first) && a.second.Track < b.second.Track); });
		std::sort(rotationKeysTemp.begin(), rotationKeysTemp.end(), [](const auto &a, const auto &b) { return (a.first < b.first) || ((a.first == b.first) && a.second.Track < b.second.Track); });
		std::sort(scaleKeysTemp.begin(), scaleKeysTemp.end(), [](const auto &a, const auto &b) { return (a.first < b.first) || ((a.first == b.first) && a.second.Track < b.second.Track); });

		return std::tuple{ translationKeysTemp, rotationKeysTemp, scaleKeysTemp };
	}

	// Import all of the channels from anim that refer to bones in skeleton
	static auto ImportChannels(const aiAnimation *anim, const Skeleton &skeleton)
	{
		std::vector<Channel> channels;

		std::unordered_map<std::string_view, uint32> boneIndices;
		for (uint32 i = 0; i < skeleton.GetNumBones(); ++i)
		{
			boneIndices.emplace(skeleton.GetBoneName(i), i);
		}

		std::map<uint32, aiNodeAnim *> validChannels;
		for (uint32 channelIndex = 0; channelIndex < anim->mNumChannels; ++channelIndex)
		{
			aiNodeAnim *nodeAnim = anim->mChannels[channelIndex];
			auto it = boneIndices.find(nodeAnim->mNodeName.C_Str());
			if (it != boneIndices.end())
			{
				validChannels.emplace(it->second, nodeAnim);
			}
		}

		channels.resize(skeleton.GetNumBones());
		for (uint32 boneIndex = 0; boneIndex < skeleton.GetNumBones(); ++boneIndex)
		{
			channels[boneIndex].Index = boneIndex;
			if (auto validChannel = validChannels.find(boneIndex); validChannel != validChannels.end())
			{
				auto nodeAnim = validChannel->second;
				channels[boneIndex].Translations.reserve(nodeAnim->mNumPositionKeys + 2); // +2 because worst case we insert two more keys
				channels[boneIndex].Rotations.reserve(nodeAnim->mNumRotationKeys + 2);
				channels[boneIndex].Scales.reserve(nodeAnim->mNumScalingKeys + 2);

				// Note: There is no need to check for duplicate keys (i.e. multiple keys all at same frame time)
				//       because Assimp throws these out for us
				for (uint32 keyIndex = 0; keyIndex < nodeAnim->mNumPositionKeys; ++keyIndex)
				{
					aiVectorKey key = nodeAnim->mPositionKeys[keyIndex];
					float frameTime = std::clamp(static_cast<float>(key.mTime / anim->mDuration), 0.0f, 1.0f);
					if ((keyIndex == 0) && (frameTime > 0.0f))
					{
						channels[boneIndex].Translations.emplace_back(0.0f, glm::vec3{ static_cast<float>(key.mValue.x), static_cast<float>(key.mValue.y), static_cast<float>(key.mValue.z) });
					}
					channels[boneIndex].Translations.emplace_back(frameTime, glm::vec3{ static_cast<float>(key.mValue.x), static_cast<float>(key.mValue.y), static_cast<float>(key.mValue.z) });
				}
				if (channels[boneIndex].Translations.empty())
				{
					HL_CORE_WARN("No translation track found for bone '{}'", *skeleton.GetBoneName(boneIndex));
					channels[boneIndex].Translations = { {0.0f, glm::vec3{0.0f}}, {1.0f, glm::vec3{0.0f}} };
				}
				else if (channels[boneIndex].Translations.back().FrameTime < 1.0f)
				{
					channels[boneIndex].Translations.emplace_back(1.0f, channels[boneIndex].Translations.back().Value);
				}
				for (uint32 keyIndex = 0; keyIndex < nodeAnim->mNumRotationKeys; ++keyIndex)
				{
					aiQuatKey key = nodeAnim->mRotationKeys[keyIndex];
					float frameTime = std::clamp(static_cast<float>(key.mTime / anim->mDuration), 0.0f, 1.0f);

					// WARNING: constructor parameter order for a quat is still WXYZ even if you have defined GLM_FORCE_QUAT_DATA_XYZW
					if ((keyIndex == 0) && (frameTime > 0.0f))
					{
						channels[boneIndex].Rotations.emplace_back(0.0f, glm::quat{ static_cast<float>(key.mValue.w), static_cast<float>(key.mValue.x), static_cast<float>(key.mValue.y), static_cast<float>(key.mValue.z) });
					}
					channels[boneIndex].Rotations.emplace_back(frameTime, glm::quat{ static_cast<float>(key.mValue.w), static_cast<float>(key.mValue.x), static_cast<float>(key.mValue.y), static_cast<float>(key.mValue.z) });
					HL_ASSERT(fabs(glm::length(channels[boneIndex].Rotations.back().Value) - 1.0f) < 0.00001f);   // check rotations are normalized (I think assimp ensures this, but not 100% sure)
				}
				if (channels[boneIndex].Rotations.empty())
				{
					HL_CORE_WARN("No rotation track found for bone '{}'", *skeleton.GetBoneName(boneIndex));
					channels[boneIndex].Rotations = { {0.0f, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}}, {1.0f, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}} };
				}
				else if (channels[boneIndex].Rotations.back().FrameTime < 1.0f)
				{
					channels[boneIndex].Rotations.emplace_back(1.0f, channels[boneIndex].Rotations.back().Value);
				}
				for (uint32 keyIndex = 0; keyIndex < nodeAnim->mNumScalingKeys; ++keyIndex)
				{
					aiVectorKey key = nodeAnim->mScalingKeys[keyIndex];
					float frameTime = std::clamp(static_cast<float>(key.mTime / anim->mDuration), 0.0f, 1.0f);
					if (keyIndex == 0 && frameTime > 0.0f)
					{
						channels[boneIndex].Scales.emplace_back(0.0f, glm::vec3{ static_cast<float>(key.mValue.x), static_cast<float>(key.mValue.y), static_cast<float>(key.mValue.z) });
					}
					channels[boneIndex].Scales.emplace_back(frameTime, glm::vec3{ static_cast<float>(key.mValue.x), static_cast<float>(key.mValue.y), static_cast<float>(key.mValue.z) });
				}
				if (channels[boneIndex].Scales.empty())
				{
					HL_CORE_WARN("No scale track found for bone '{}'", *skeleton.GetBoneName(boneIndex));
					channels[boneIndex].Scales = { {0.0f, glm::vec3{1.0f}}, {1.0f, glm::vec3{1.0f}} };
				}
				else if (channels[boneIndex].Scales.back().FrameTime < 1.0f)
				{
					channels[boneIndex].Scales.emplace_back(1.0f, channels[boneIndex].Scales.back().Value);
				}
			}
			else
			{
				HL_CORE_WARN("No animation tracks found for bone '{}'", *skeleton.GetBoneName(boneIndex));
				channels[boneIndex].Translations = { {0.0f, glm::vec3{0.0f}}, {1.0f, glm::vec3{0.0f}} };
				channels[boneIndex].Rotations = { {0.0f, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}}, {1.0f, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}} };
				channels[boneIndex].Scales = { {0.0f, glm::vec3{1.0f}}, {1.0f, glm::vec3{1.0f}} };
			}
		}

		return channels;
	}
}