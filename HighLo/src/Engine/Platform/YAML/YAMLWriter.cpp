// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "YAMLWriter.h"

namespace highlo
{
	YAMLWriter::YAMLWriter(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}

	YAMLWriter::~YAMLWriter()
	{
	}

	bool YAMLWriter::Write(const HLString &key, float value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, double value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, int32 value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, uint32 value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, int64 value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, uint64 value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, bool value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const HLString &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<DocumentEntry> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const glm::vec2 &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const glm::vec3 &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const glm::vec4 &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const glm::mat2 &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const glm::mat3 &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const glm::mat4 &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, const glm::quat &value)
	{
		return false;
	}

	bool YAMLWriter::ReadFloat(const HLString &key, float *value)
	{
		return false;
	}

	bool YAMLWriter::ReadDouble(const HLString &key, double *value)
	{
		return false;
	}

	bool YAMLWriter::ReadInt32(const HLString &key, int32 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt32(const HLString &key, uint32 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadInt64(const HLString &key, int64 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt64(const HLString &key, uint64 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadBool(const HLString &key, bool *value)
	{
		return false;
	}

	bool YAMLWriter::ReadString(const HLString &key, HLString *value)
	{
		return false;
	}

	bool YAMLWriter::ReadVector2(const HLString &key, glm::vec2 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadVector3(const HLString &key, glm::vec3 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadVector4(const HLString &key, glm::vec4 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadMatrix2(const HLString &key, glm::mat2 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadMatrix3(const HLString &key, glm::mat3 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadMatrix4(const HLString &key, glm::mat4 *value)
	{
		return false;
	}

	bool YAMLWriter::ReadQuaternion(const HLString &key, glm::quat *value)
	{
		return false;
	}

	std::vector<DocumentEntry> YAMLWriter::GetAll()
	{
		return std::vector<DocumentEntry>();
	}

	bool YAMLWriter::HasKey(const HLString &key) const
	{
		return false;
	}

	bool YAMLWriter::WriteOut() const
	{
		return false;
	}

	HLString YAMLWriter::GetContent() const
	{
		return HLString();
	}
}

