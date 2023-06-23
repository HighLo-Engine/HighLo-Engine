// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-06-21) initial release
//

#pragma once

namespace highlo
{
	class Skeleton
	{
	public:

		static const uint32 NullIndex = ~0;

		Skeleton() = default;
		Skeleton(uint32 size);

		uint32 AddBone(const HLString &name, uint32 parentIndex, const glm::mat4 &transform);
		uint32 GetBoneIndex(const HLString &name) const;

		uint32 GetParentBoneIndex(const uint32 boneIndex) const { HL_ASSERT(boneIndex < m_ParentBoneIndices.size(), "bone index out of range in Skeleton::GetParentIndex()!"); return m_ParentBoneIndices[boneIndex]; }
		uint32 GetNumBones() const { return static_cast<uint32>(m_BoneNames.size()); }
		
		const HLString &GetBoneName(const uint32 boneIndex) const { HL_ASSERT(boneIndex < m_BoneNames.size(), "bone index out of range in Skeleton::GetBoneName()!"); return m_BoneNames[boneIndex]; }
		const auto &GetBoneNames() const { return m_BoneNames; }

		const std::vector<glm::vec3> &GetBoneTranslations() const { return m_BoneTranslations; }
		const std::vector<glm::quat> &GetBoneRotations() const { return m_BoneRotations; }
		const std::vector<glm::vec3> &GetBoneScales() const { return m_BoneScales; }

		bool operator==(const Skeleton &other) const;
		bool operator!=(const Skeleton &other) const;

	private:

		std::vector<HLString> m_BoneNames;
		std::vector<uint32> m_ParentBoneIndices;

		// rest pose of skeleton. All in bone-local space (i.e. translation/rotation/scale relative to parent)
		std::vector<glm::vec3> m_BoneTranslations;
		std::vector<glm::quat> m_BoneRotations;
		std::vector<glm::vec3> m_BoneScales;
	};
}

