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

	void XMLWriter::BeginArray()
	{
	}

	void XMLWriter::EndArray(const HLString &key)
	{
	}

	void XMLWriter::BeginObject()
	{
	}

	void XMLWriter::EndObject()
	{
	}

	bool XMLWriter::WriteFloat(const HLString &key, float value)
	{
		return false;
	}

	bool XMLWriter::WriteDouble(const HLString &key, double value)
	{
		return false;
	}

	bool XMLWriter::WriteInt32(const HLString &key, int32 value)
	{
		return false;
	}

	bool XMLWriter::WriteUInt32(const HLString &key, uint32 value)
	{
		return false;
	}

	bool XMLWriter::WriteInt64(const HLString &key, int64 value)
	{
		return false;
	}

	bool XMLWriter::WriteUInt64(const HLString &key, uint64 value)
	{
		return false;
	}

	bool XMLWriter::WriteBool(const HLString &key, bool value)
	{
		return false;
	}

	bool XMLWriter::WriteString(const HLString &key, const HLString &value)
	{
		return false;
	}

	bool XMLWriter::WriteVec2(const HLString &key, const glm::vec2 &value)
	{
		return false;
	}

	bool XMLWriter::WriteVec3(const HLString &key, const glm::vec3 &value)
	{
		return false;
	}

	bool XMLWriter::WriteVec4(const HLString &key, const glm::vec4 &value)
	{
		return false;
	}

	bool XMLWriter::WriteMat2(const HLString &key, const glm::mat2 &value)
	{
		return false;
	}

	bool XMLWriter::WriteMat3(const HLString &key, const glm::mat3 &value)
	{
		return false;
	}

	bool XMLWriter::WriteMat4(const HLString &key, const glm::mat4 &value)
	{
		return false;
	}

	bool XMLWriter::WriteQuaternion(const HLString &key, const glm::quat &value)
	{
		return false;
	}

	bool XMLWriter::WriteStringArray(const HLString &key, std::vector<HLString> &value)
	{
		return false;
	}

	bool XMLWriter::WriteInt32Array(const HLString &key, std::vector<int32> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		return false;
	}

	bool XMLWriter::WriteInt64Array(const HLString &key, std::vector<int64> &value)
	{
		return false;
	}

	bool XMLWriter::WriteUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		return false;
	}

	bool XMLWriter::WriteBoolArray(const HLString &key, std::vector<bool> &value)
	{
		return false;
	}

	bool XMLWriter::WriteFloatArray(const HLString &key, std::vector<float> &value)
	{
		return false;
	}

	bool XMLWriter::WriteDoubleArray(const HLString &key, std::vector<double> &value)
	{
		return false;
	}

	bool XMLWriter::WriteVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		return false;
	}

	bool XMLWriter::WriteVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		return false;
	}

	bool XMLWriter::WriteVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		return false;
	}

	bool XMLWriter::WriteMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		return false;
	}

	bool XMLWriter::WriteMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		return false;
	}

	bool XMLWriter::WriteMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		return false;
	}

	bool XMLWriter::WriteQuaternionArray(const HLString & key, std::vector<glm::quat> &value)
	{
		return false;
	}

	bool XMLWriter::ReadStringArray(const HLString &key, std::vector<HLString> &result)
	{
		return false;
	}

	bool XMLWriter::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return false;
	}

	bool XMLWriter::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return false;
	}

	bool XMLWriter::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return false;
	}

	bool XMLWriter::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return false;
	}

	bool XMLWriter::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return false;
	}

	bool XMLWriter::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return false;
	}

	bool XMLWriter::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return false;
	}

	bool XMLWriter::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return false;
	}

	bool XMLWriter::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return false;
	}

	bool XMLWriter::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return false;
	}

	bool XMLWriter::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return false;
	}

	bool XMLWriter::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return false;
	}

	bool XMLWriter::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return false;
	}

	bool XMLWriter::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return false;
	}

	bool XMLWriter::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return false;
	}

	bool XMLWriter::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return false;
	}

	bool XMLWriter::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return false;
	}

	bool XMLWriter::Readint64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return false;
	}

	bool XMLWriter::ReadUint64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return false;
	}

	bool XMLWriter::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return false;
	}

	bool XMLWriter::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return false;
	}

	bool XMLWriter::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return false;
	}

	bool XMLWriter::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return false;
	}

	bool XMLWriter::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return false;
	}

	bool XMLWriter::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return false;
	}

	bool XMLWriter::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return false;
	}

	bool XMLWriter::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return false;
	}

	bool XMLWriter::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return false;
	}

	bool XMLWriter::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return false;
	}

	bool XMLWriter::ReadFloat(const HLString &key, float *result)
	{
		return false;
	}

	bool XMLWriter::ReadDouble(const HLString &key, double *result)
	{
		return false;
	}

	bool XMLWriter::ReadInt32(const HLString &key, int32 *result)
	{
		return false;
	}

	bool XMLWriter::ReadUInt32(const HLString &key, uint32 *result)
	{
		return false;
	}

	bool XMLWriter::ReadInt64(const HLString &key, int64 *result)
	{
		return false;
	}

	bool XMLWriter::ReadUInt64(const HLString &key, uint64 *result)
	{
		return false;
	}

	bool XMLWriter::ReadBool(const HLString &key, bool *result)
	{
		return false;
	}

	bool XMLWriter::ReadString(const HLString &key, HLString *result)
	{
		return false;
	}

	bool XMLWriter::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return false;
	}

	bool XMLWriter::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return false;
	}

	bool XMLWriter::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return false;
	}

	bool XMLWriter::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return false;
	}

	bool XMLWriter::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return false;
	}

	bool XMLWriter::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return false;
	}

	bool XMLWriter::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return false;
	}

	bool XMLWriter::HasKey(const HLString &key) const
	{
		return false;
	}

	bool XMLWriter::WriteOut() const
	{
		return false;
	}

	bool XMLWriter::ReadContents(const FileSystemPath &filePath)
	{
		return false;
	}

	HLString XMLWriter::GetContent() const
	{
		return HLString();
	}
}

