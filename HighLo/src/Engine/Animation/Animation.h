// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-06-21) initial release
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace highlo
{
	class MeshFile;

	template<typename T>
	struct AnimationKey
	{
		T Value;
		float FrameTime;       // 0.0f = beginning of animation clip, 1.0f = end of animation clip 
		uint32 Track;

		AnimationKey() = default;
		AnimationKey(const float frameTime, const uint32 track, const T &value)
			: FrameTime(frameTime), Track(track), Value(value)
		{
		}
	};

	using TranslationKey = AnimationKey<glm::vec3>;
	using RotationKey = AnimationKey<glm::quat>;
	using ScaleKey = AnimationKey<glm::vec3>;

	class Animation
	{
	public:

		Animation() = default;
		Animation(const HLString &name, const float duration);

		const HLString &GetName() const { return m_Name; }
		float GetDuration() const { return m_Duration; }

		void SetKeyFrames(std::vector<TranslationKey> translations, std::vector<RotationKey> rotations, std::vector<ScaleKey> scales);

		const auto &GetTranslationKeys() const { return m_TranslationKeys; }
		const auto &GetRotationKeys() const { return m_RotationKeys; }
		const auto &GetScaleKeys() const { return m_ScaleKeys; }

	private:

		std::vector<TranslationKey> m_TranslationKeys;
		std::vector<RotationKey> m_RotationKeys;
		std::vector<ScaleKey> m_ScaleKeys;
		HLString m_Name;
		float m_Duration;
	};
}

