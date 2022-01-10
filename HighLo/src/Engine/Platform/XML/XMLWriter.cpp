// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "XMLWriter.h"

namespace highlo
{
	XMLWriter::XMLWriter(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{

	}

	XMLWriter::~XMLWriter()
	{
	}

	bool XMLWriter::ReadVector2(const HLString &key, glm::vec2 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadVector3(const HLString &key, glm::vec3 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadVector4(const HLString &key, glm::vec4 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadMatrix2(const HLString &key, glm::mat2 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadMatrix3(const HLString &key, glm::mat3 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadMatrix4(const HLString &key, glm::mat4 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadQuaternion(const HLString &key, glm::quat *value)
	{
		return false;
	}
	
	std::vector<DocumentEntry> XMLWriter::GetAll()
	{
		return std::vector<DocumentEntry>();
	}
	
	bool XMLWriter::HasKey(const HLString &key) const
	{
		return false;
	}
	
	bool XMLWriter::WriteOut() const
	{
		return false;
	}
	
	HLString XMLWriter::GetContent() const
	{
		return HLString();
	}
	
	bool XMLWriter::ReadFloat(const HLString &key, float *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadDouble(const HLString &key, double *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadInt32(const HLString &key, int32 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadUInt32(const HLString &key, uint32 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadInt64(const HLString &key, int64 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadUInt64(const HLString &key, uint64 *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadBool(const HLString &key, bool *value)
	{
		return false;
	}
	
	bool XMLWriter::ReadString(const HLString &key, HLString *value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, std::vector<DocumentEntry> &value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const glm::vec2 &value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const glm::vec3 &value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const glm::vec4 &value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const glm::mat2 &value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const glm::mat3 &value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const glm::mat4 &value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const glm::quat &value)
	{
		return false;
	}

	bool XMLWriter::Write(const HLString &key, float value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, double value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, int32 value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, uint32 value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, int64 value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, uint64 value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, bool value)
	{
		return false;
	}
	
	bool XMLWriter::Write(const HLString &key, const HLString &value)
	{
		return false;
	}
}

