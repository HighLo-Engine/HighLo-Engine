// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Animation.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace highlo
{
	Transform BoneTransform::GetLocalTransform()
	{
		return Transform::FromPosition(Translation).Rotate(Rotation);
	}

	BoneTransform BoneTransform::Interpolate(BoneTransform first, BoneTransform second, float progression)
	{
		float x = first.Translation.x + (second.Translation.x - first.Translation.x) * progression;
		float y = first.Translation.y + (second.Translation.y - first.Translation.y) * progression;
		float z = first.Translation.z + (second.Translation.z - first.Translation.z) * progression;

		glm::vec3 pos = { x, y, z };
		glm::quat rot = Interpolate(first.Rotation, second.Rotation, progression);

		BoneTransform result;
		result.Translation = pos;
		result.Rotation = rot;

		return result;
	}

	glm::quat BoneTransform::Interpolate(glm::quat a, glm::quat b, float blend)
	{
		glm::quat result = glm::quat(1, 0, 0, 0);

		float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		float blendI = 1.0f - blend;

		if (dot < 0.0f)
		{
			result.w = blendI * a.w + blend * -b.w;
			result.x = blendI * a.x + blend * -b.x;
			result.y = blendI * a.y + blend * -b.y;
			result.z = blendI * a.z + blend * -b.z;
		}
		else
		{
			result.w = blendI * a.w + blend * b.w;
			result.x = blendI * a.x + blend * b.x;
			result.y = blendI * a.y + blend * b.y;
			result.z = blendI * a.z + blend * b.z;
		}

		result = glm::normalize(result);
		return result;
	}

	Animation::Animation(const HLString &name, float duration, float ticksPerSecond, glm::mat4 inverseTransform, int32 bone_count, Bone rootBone, glm::mat4 correctionMatrix)
		: Name(name), Duration(duration), TicksPerSecond(ticksPerSecond), m_InverseTransform(inverseTransform), m_RootBone(rootBone),
		m_BoneCount((uint32)bone_count), m_CorrectionMatrix(correctionMatrix)
	{
		for (uint64 i = 0; i < HL_MAX_SKELETAL_BONES; i++)
			m_BoneFrameTransforms[i] = glm::mat4(1.0f);
	}

	void Animation::CalculateFinalBoneTransforms(Bone &bone, glm::mat4 parentTransform, float animationTime)
	{
		auto frames = GetPreviousAndNextFrames(bone, animationTime);
		auto progression = CalculateProgression(bone, frames.first, frames.second, animationTime);

		// Sanity check
		if (frames.first > bone.Keyframes.size() - 1)
			frames.first = 0;

		if (frames.second > bone.Keyframes.size() - 1)
			frames.second = 0;

		Transform current_pose = InterpolatePoses(bone.Keyframes[frames.first].Transform, bone.Keyframes[frames.second].Transform, progression);
		glm::mat4 transform = parentTransform * current_pose.GetTransform();

		bone.FinalTransform = m_InverseTransform * transform * bone.OffsetMatrix;

		for (auto &child : bone.Children)
			CalculateFinalBoneTransforms(child, transform, animationTime);
	}

	void Animation::AddBoneTransform(Bone &bone)
	{
		glm::mat4 boneFrameTransform = bone.UserTransformation * m_CorrectionMatrix * bone.FinalTransform;

		if (!isnan(boneFrameTransform[0][0]))
			m_BoneFrameTransforms[bone.ID] = boneFrameTransform;

		for (auto& child : bone.Children)
			AddBoneTransform(child);
	}

	glm::mat4 *Animation::GetCurrentPoseTransforms()
	{
		if (m_IsPlaying)
		{
			CalculateFinalBoneTransforms(m_RootBone, glm::mat4(1.0f), CurrentAnimationTime);
			AddBoneTransform(m_RootBone);
		}
		else
		{
			for (uint64 i = 0; i < m_BoneCount; i++)
				m_BoneFrameTransforms[i] = m_CorrectionMatrix;
		}

		return m_BoneFrameTransforms;
	}

	Bone *Animation::FindBone(const HLString &name)
	{
		Bone *result = nullptr;

		ForEachBone(m_RootBone, [&name, &result](Bone &bone) {
			if (bone.Name == (name))
				result = &bone;
		});

		return result;
	}

	void Animation::ForEachBone(Bone &bone, const std::function<void(Bone&)> &lambda)
	{
		lambda(bone);

		for (auto &child : bone.Children)
			ForEachBone(child, lambda);
	}

	Ref<Animation> Animation::Create(const HLString &name, float duration, float ticksPerSecond, glm::mat4 inverseTransform, int32 boneCount, Bone rootBone, glm::mat4 correctionMatrix)
	{
		return Ref<Animation>::Create(name, duration, ticksPerSecond, inverseTransform, boneCount, rootBone, correctionMatrix);
	}

	std::pair<uint64, uint64> Animation::GetPreviousAndNextFrames(Bone &bone, float animationTime)
	{
		float previousFrame = bone.Keyframes[0].Timestamp;
		float nextFrame = bone.Keyframes[0].Timestamp;

		uint64 previousFrameIndex = 0;
		uint64 nextFrameIndex = 0;

		for (uint64 i = 1; i < bone.Keyframes.size(); i++)
		{
			nextFrame = bone.Keyframes[i].Timestamp;
			nextFrameIndex = i;

			if (nextFrame > animationTime)
				break;

			previousFrame = bone.Keyframes[i].Timestamp;
			previousFrameIndex = i;
		}

		return { previousFrameIndex, nextFrameIndex };
	}

	float Animation::CalculateProgression(Bone &bone, uint64 previousFrameIndex, uint64 nextFrameIndex, float animationTime)
	{
		float totalTime = bone.Keyframes[nextFrameIndex].Timestamp - bone.Keyframes[previousFrameIndex].Timestamp;
		float currentTime = animationTime - bone.Keyframes[previousFrameIndex].Timestamp;
		return currentTime / totalTime;
	}

	Transform Animation::InterpolatePoses(BoneTransform previousPose, BoneTransform nextPose, float progression)
	{
		return BoneTransform::Interpolate(previousPose, nextPose, progression).GetLocalTransform();
	}

	void Animation::Play()
	{
		m_IsPlaying = true;
	}

	void Animation::Pause()
	{
		m_IsPlaying = false;
	}

	void Animation::Stop()
	{
		m_IsPlaying = false;
		CurrentAnimationTime = 0;
	}

	void Animation::Update(Timestep ts)
	{
		if (!m_IsPlaying) return;

		if (CurrentAnimationTime > Duration)
		{
			if (ShouldLoopAnimation)
			{
				CurrentAnimationTime = 0;
			}
			else
			{
				Stop();
				return;
			}
		}

		if (CurrentAnimationTime < 0)
			CurrentAnimationTime = 0;

		CurrentAnimationTime += (ts * AnimationSpeed) / AnimationSpeedDenominator;
	}
}

