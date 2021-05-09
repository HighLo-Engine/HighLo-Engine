#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "Engine/Core/HLTime.h"
#include "Engine/Math/HLMath.h"
#include "Engine/Math/Transform.h"

namespace highlo
{
	class BoneTransform
	{
	public:
		glm::vec3 Translation;
		glm::quat Rotation;

		HLAPI Transform GetLocalTransform();

		HLAPI static BoneTransform Interpolate(BoneTransform first, BoneTransform second, float progression);

	private:
		static glm::quat Interpolate(glm::quat a, glm::quat b, float blend);
	};

	struct Keyframe
	{
		float Timestamp;
		BoneTransform Transform;
	};

	struct Bone
	{
		HLString Name;
		uint32 ID = -1;

		std::vector<Bone> Children;

		glm::mat4 OffsetMatrix = glm::mat4(1.0f);
		glm::mat4 UserTransformation = glm::mat4(1.0f);
		glm::mat4 FinalTransform = glm::mat4(1.0f);

		std::vector<Keyframe> Keyframes;

		Bone() = default;
	};

	class Animation : public IsSharedReference
	{
	public:
		HLAPI Animation() = default;
		HLAPI Animation(const HLString& name, float duration, float ticks_per_second, glm::mat4 inverse_transform, int bone_count, Bone root_bone, glm::mat4 correction_matrix);

		HLString Name;

		float Duration = 0;
		float TicksPerSecond = 0;

		float AnimationSpeed = 1;
		float CurrentAnimationTime = 0;
		float AnimationSpeedDenominator = 1;

		bool ShouldLoopAnimation = true;

		HLAPI bool IsPlaying() const { return m_IsPlaying; }

		HLAPI void Play();
		HLAPI void Pause();
		HLAPI void Stop();
		HLAPI void Update(Timestep ts);

		HLAPI std::vector<glm::mat4>& GetCurrentPoseTransforms();

		HLAPI glm::mat4 GetCorrectionMatrix() const { return m_CorrectionMatrix; }

		HLAPI Bone* FindBone(const HLString& name);
		HLAPI void ForEachBone(Bone& bone, std::function<void(Bone&)> lambda);

		HLAPI Bone& GetRootBone() { return m_RootBone; }

	private:
		bool m_IsPlaying = false;

	private:
		glm::mat4 m_InverseTransform = glm::mat4(1.0f);
		Bone m_RootBone = Bone();
		uint32 m_BoneCount = 0;
		glm::mat4 m_CorrectionMatrix = glm::mat4(1.0f);

	private:
		std::vector<glm::mat4> m_BoneFrameTransforms;

	private:
		void CalculateFinalBoneTransforms(Bone& bone, glm::mat4 parent_transform, float animation_time);
		void AddBoneTransform(Bone& bone);

		std::pair<uint64, uint64> GetPreviousAndNextFrames(Bone& bone, float animation_time);
		float CalculateProgression(Bone& bone, uint64 previous_frame_index, uint64 next_frame_index, float animation_time);
		Transform InterpolatePoses(BoneTransform previous_pose, BoneTransform next_pose, float progression);
	};
}
