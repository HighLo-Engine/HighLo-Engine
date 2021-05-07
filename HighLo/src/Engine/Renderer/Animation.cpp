#include "HighLoPch.h"
#include "Animation.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace highlo
{
	glm::mat4 BoneTransform::GetLocalTransform()
	{
		glm::mat4 mat = glm::mat4(1.0f);

		mat = glm::translate(mat, Translation);
		mat = mat * glm::toMat4(Rotation);

		return mat;
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

		if (dot < 0)
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

	Animation::Animation(const HLString& name, float duration, float ticks_per_second, glm::mat4 inverse_transform, int bone_count, Bone root_bone, glm::mat4 correction_matrix)
		: Name(name), Duration(duration), TicksPerSecond(ticks_per_second), m_InverseTransform(inverse_transform), m_RootBone(root_bone),
		m_BoneCount((uint32)bone_count), m_CorrectionMatrix(correction_matrix)
	{
		for (uint64 i = 0; i < bone_count; i++)
			m_BoneFrameTransforms.push_back(glm::mat4(1.0f));
	}

	void Animation::CalculateFinalBoneTransforms(Bone& bone, glm::mat4 parent_transform, float animation_time)
	{
		auto frames = GetPreviousAndNextFrames(bone, animation_time);
		auto progression = CalculateProgression(bone, frames.first, frames.second, animation_time);

		// Sanity check
		if (frames.first > bone.Keyframes.size() - 1)
			frames.first = 0;

		if (frames.second > bone.Keyframes.size() - 1)
			frames.second = 0;

		glm::mat4 current_pose = InterpolatePoses(bone.Keyframes[frames.first].Transform, bone.Keyframes[frames.second].Transform, progression);

		glm::mat4 transform = parent_transform * current_pose;

		bone.FinalTransform = m_InverseTransform * transform * bone.OffsetMatrix;

		for (auto& child : bone.Children)
			CalculateFinalBoneTransforms(child, transform, animation_time);
	}

	void Animation::AddBoneTransform(Bone& bone)
	{
		m_BoneFrameTransforms[bone.ID] = bone.UserTransformation * m_CorrectionMatrix * bone.FinalTransform;

		for (auto& child : bone.Children)
			AddBoneTransform(child);
	}

	std::vector<glm::mat4>& Animation::GetCurrentPoseTransforms()
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

	Bone* Animation::FindBone(const HLString& name)
	{
		Bone* result = nullptr;

		ForEachBone(m_RootBone, [&name, &result](Bone& bone) {
			if (bone.Name == (name))
				result = &bone;
		});

		return result;
	}

	void Animation::ForEachBone(Bone& bone, std::function<void(Bone&)> lambda)
	{
		lambda(bone);

		for (auto& child : bone.Children)
			ForEachBone(child, lambda);
	}

	std::pair<uint64, uint64> Animation::GetPreviousAndNextFrames(Bone& bone, float animation_time)
	{
		float PreviousFrame = bone.Keyframes[0].Timestamp;
		float NextFrame = bone.Keyframes[0].Timestamp;

		uint64 PreviousFrameIndex = 0;
		uint64 NextFrameIndex = 0;

		for (uint64 i = 1; i < bone.Keyframes.size(); i++)
		{
			NextFrame = bone.Keyframes[i].Timestamp;
			NextFrameIndex = i;

			if (NextFrame > animation_time)
				break;

			PreviousFrame = bone.Keyframes[i].Timestamp;
			PreviousFrameIndex = i;
		}

		return { PreviousFrameIndex, NextFrameIndex };
	}

	float Animation::CalculateProgression(Bone& bone, uint64 previous_frame_index, uint64 next_frame_index, float animation_time)
	{
		float TotalTime = bone.Keyframes[next_frame_index].Timestamp - bone.Keyframes[previous_frame_index].Timestamp;
		float CurrentTime = animation_time - bone.Keyframes[previous_frame_index].Timestamp;
		return CurrentTime / TotalTime;
	}

	glm::mat4 Animation::InterpolatePoses(BoneTransform previous_pose, BoneTransform next_pose, float progression)
	{
		return BoneTransform::Interpolate(previous_pose, next_pose, progression).GetLocalTransform();
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

		CurrentAnimationTime += ((float)ts * AnimationSpeed) / AnimationSpeedDenominator;
	}
}
