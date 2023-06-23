// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Skeleton.h"

#include "Engine/Math/Math.h"

namespace highlo
{
	Skeleton::Skeleton(uint32 size)
	{
		m_BoneNames.reserve(size);
		m_ParentBoneIndices.reserve(size);
	}

	uint32 Skeleton::AddBone(const HLString &name, uint32 parentIndex, const glm::mat4 &transform)
	{
		uint32 index = static_cast<uint32>(m_BoneNames.size());
		m_BoneNames.emplace_back(name);
		m_ParentBoneIndices.emplace_back(parentIndex);
		m_BoneTranslations.emplace_back();
		m_BoneRotations.emplace_back();
		m_BoneScales.emplace_back();
		
		Math::DecomposeQuat(transform, m_BoneTranslations.back(), m_BoneScales.back(), m_BoneRotations.back());

		return index;
	}
	
	uint32 Skeleton::GetBoneIndex(const HLString &name) const
	{
		for (size_t i = 0; i < m_BoneNames.size(); ++i)
		{
			if (m_BoneNames[i] == name)
			{
				return static_cast<uint32>(i);
			}
		}

		return Skeleton::NullIndex;
	}
	
	bool Skeleton::operator==(const Skeleton &other) const
	{
		bool areSame = false;
		if (GetNumBones() == other.GetNumBones())
		{
			areSame = true;
			for (uint32 i = 0; i < GetNumBones(); ++i)
			{
				if (GetBoneName(i) != other.GetBoneName(i))
				{
					areSame = false;
					break;
				}
			}
		}
		return areSame;
	}
	
	bool Skeleton::operator!=(const Skeleton &other) const
	{
		return !(*this == other);
	}
}

