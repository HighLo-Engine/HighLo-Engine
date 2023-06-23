// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Animation.h"

namespace highlo
{
	Animation::Animation(const HLString &name, const float duration)
		: m_Name(name), m_Duration(duration)
	{
	}
	
	void Animation::SetKeyFrames(std::vector<TranslationKey> translations, std::vector<RotationKey> rotations, std::vector<ScaleKey> scales)
	{
		m_TranslationKeys = std::move(translations);
		m_RotationKeys = std::move(rotations);
		m_ScaleKeys = std::move(scales);
	}
}

