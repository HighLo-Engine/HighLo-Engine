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

	void YAMLWriter::BeginArray()
	{
	}

	void YAMLWriter::EndArray(const HLString &key)
	{
	}

	void YAMLWriter::BeginObject()
	{
	}

	void YAMLWriter::EndObject()
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

	bool YAMLWriter::Write(const HLString &key, std::vector<HLString> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<int32> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<uint32> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<int64> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<uint64> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<bool> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<float> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<double> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<glm::vec2> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<glm::vec3> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<glm::vec4> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<glm::mat2> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<glm::mat3> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<glm::mat4> &value)
	{
		return false;
	}

	bool YAMLWriter::Write(const HLString &key, std::vector<glm::quat> &value)
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

	bool YAMLWriter::ReadStringArray(const HLString &key, std::vector<HLString> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadInt32Array(const HLString &key, std::vector<int32> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadInt64Array(const HLString &key, std::vector<int64> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadBoolArray(const HLString &key, std::vector<bool> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadFloatArray(const HLString &key, std::vector<float> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadDoubleArray(const HLString &key, std::vector<double> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadQuatArray(const HLString &key, std::vector<glm::quat> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &value)
	{
		return false;
	}

	bool YAMLWriter::Readint64ArrayMap(const HLString &key, std::map<HLString, int64> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadUint64ArrayMap(const HLString &key, std::map<HLString, uint64> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &value)
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

	bool YAMLWriter::HasKey(const HLString &key) const
	{
		return false;
	}

	bool YAMLWriter::WriteOut() const
	{
		return false;
	}

	bool YAMLWriter::ReadContents(const FileSystemPath &filePath)
	{
		if (!filePath.String().IsEmpty())
			m_FilePath = filePath;

		m_Document = YAML::LoadFile(**m_FilePath);
		return true;
	}

	HLString YAMLWriter::GetContent() const
	{
		return HLString();
	}
}

