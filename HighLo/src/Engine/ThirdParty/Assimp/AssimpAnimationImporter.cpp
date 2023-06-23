// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssimpAnimationImporter.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "AssimpLogStream.h"
#include "AssimpUtils.h"
#include "AssimpAnimationUtils.h"

namespace highlo
{
	static const uint32 s_AnimationImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate |             // Make sure we're triangles
		aiProcess_SortByPType |             // Split meshes by primitive type
		aiProcess_GenNormals |              // Make sure we have legit normals
		aiProcess_GenUVCoords |             // Convert UVs if required
		//		aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes |          // Batch draws where possible
		aiProcess_JoinIdenticalVertices |
		aiProcess_LimitBoneWeights |        // If more than N (=4) bone weights, discard least influencing bones and renormalise sum to 1
		aiProcess_GlobalScale |             // e.g. convert cm to m for fbx import (and other formats where cm is native)
		//		aiProcess_PopulateArmatureData |    // not currently using this data
		aiProcess_ValidateDataStructure;    // Validation 

	UniqueReference<Skeleton> ImportSkeleton(const HLString &filename)
	{
		AssimpLogStream::Initialize();

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(*filename, s_AnimationImportFlags);
		return ImportSkeleton(scene);
	}

	UniqueReference<Skeleton> ImportSkeleton(const aiScene *scene)
	{
		utils::BoneHierarchy boneHierarchy(scene);
		return boneHierarchy.CreateSkeleton();
	}
	
	UniqueReference<Animation> ImportAnimation(const HLString &filename, const Skeleton &skeleton)
	{
		AssimpLogStream::Initialize();

		HL_CORE_INFO("Loading animation: {0}", filename);
		UniqueReference<Animation> animation;

		if (skeleton.GetNumBones() == 0)
		{
			HL_CORE_ERROR("Empty skeleton passed to animation asset for file '{0}'", filename);
			return animation;
		}

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(*filename, s_AnimationImportFlags);
		if (!scene || !scene->HasAnimations())
		{
			HL_CORE_ERROR("Failed to load animation from file '{0}'", *filename);
			return animation;
		}

		// If the file contains a skeleton, check is same as skeleton we want to apply the animation to
		// TODO: Later we might want some sort of re-targeting functionality to deal with this.
		auto localSkeleton = ImportSkeleton(scene);
		if (localSkeleton && *localSkeleton != skeleton)
		{
			HL_CORE_ERROR("Skeleton found in animation file '{0}' differs from expected.  All animations in an animation controller must share the same skeleton!", filename);
			return animation;
		}

		auto animationNames = GetAnimationNames(scene);
		if (animationNames.empty())
		{
			// shouldnt ever get here, since we already checked scene.HasAnimations()...
			HL_CORE_ERROR("Failed to load animation from file: {0}", *filename);
			return animation;
		}

		if (animationNames.size() > 1)
		{
			HL_CORE_WARN("File '{0}' contains {1} animations.  Only the first will be imported", *filename, animationNames.size());
		}

		aiString animationName;
		animation = ImportAnimation(scene, animationNames.front(), skeleton);
		if (!animation)
		{
			HL_CORE_ERROR("Failed to extract animation '{}' from file '{}'", animationNames.front(), *filename);
		}
		return animation;
	}
	
	std::vector<HLString> GetAnimationNames(const aiScene *scene)
	{
		std::vector<HLString> animationNames;
		if (scene)
		{
			animationNames.reserve(scene->mNumAnimations);
			for (size_t i = 0; i < scene->mNumAnimations; ++i)
			{
				if (scene->mAnimations[i]->mDuration > 0.0f)
				{
					animationNames.emplace_back(scene->mAnimations[i]->mName.C_Str());
				}
				else
				{
					HL_CORE_ERROR("Animation '{0}' duration is zero or negative.  This animation was ignored!", scene->mAnimations[i]->mName.C_Str());
				}
			}
		}
		return animationNames;
	}
	
	UniqueReference<Animation> ImportAnimation(const aiScene *scene, const HLString &animationName, const Skeleton &skeleton)
	{
		if (!scene)
		{
			return nullptr;
		}

		UniqueReference<Animation> animation = nullptr;

		for (uint32 animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			const aiAnimation *anim = scene->mAnimations[animIndex];
			if (animationName == anim->mName.C_Str())
			{
				auto channels = utils::ImportChannels(anim, skeleton);
				auto [translationKeysTemp, rotationKeysTemp, scaleKeysTemp] = utils::ConcatenateChannelsAndSort(channels);
				auto [translationKeys, rotationKeys, scaleKeys] = utils::ExtractKeys(translationKeysTemp, rotationKeysTemp, scaleKeysTemp);

				double samplingRate = anim->mTicksPerSecond;
				if (samplingRate < 0.0001)
				{
					samplingRate = 1.0;
				}

				animation = UniqueReference<Animation>::Create(animationName, static_cast<float>(anim->mDuration / samplingRate));
				animation->SetKeyFrames(std::move(translationKeys), std::move(rotationKeys), std::move(scaleKeys));
				break;
			}
		}

		return animation;
	}
}

