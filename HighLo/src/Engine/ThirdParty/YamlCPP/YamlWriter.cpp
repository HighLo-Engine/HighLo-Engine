// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "YamlWriter.h"

#include <yaml-cpp/yaml.h>
#include "YamlHelper.h"

namespace highlo
{
	YamlWriter::YamlWriter(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}

	YamlWriter::~YamlWriter()
	{
	}
	
	void YamlWriter::BeginArray()
	{
	}
	
	void YamlWriter::EndArray(const HLString &key, bool rawData)
	{
	}
	
	void YamlWriter::BeginObject()
	{
	}
	
	void YamlWriter::EndObject(bool rawData)
	{
	}
	
	bool YamlWriter::WriteFloat(const HLString &key, float value)
	{
		return false;
	}
	
	bool YamlWriter::WriteDouble(const HLString &key, double value)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt32(const HLString &key, int32 value)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt32(const HLString &key, uint32 value)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt64(const HLString &key, int64 value)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt64(const HLString &key, uint64 value)
	{
		return false;
	}
	
	bool YamlWriter::WriteBool(const HLString &key, bool value)
	{
		return false;
	}
	
	bool YamlWriter::WriteString(const HLString &key, const HLString &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec2(const HLString &key, const glm::vec2 &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec3(const HLString &key, const glm::vec3 &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec4(const HLString &key, const glm::vec4 &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat2(const HLString &key, const glm::mat2 &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat3(const HLString &key, const glm::mat3 &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat4(const HLString &key, const glm::mat4 &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteQuaternion(const HLString &key, const glm::quat &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteStringArray(const HLString &key, const std::vector<HLString> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt32Array(const HLString &key, const std::vector<int32> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt32Array(const HLString &key, const std::vector<uint32> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt64Array(const HLString &key, const std::vector<int64> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt64Array(const HLString &key, const std::vector<uint64> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteBoolArray(const HLString &key, const std::vector<bool> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteFloatArray(const HLString &key, const std::vector<float> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteDoubleArray(const HLString &key, const std::vector<double> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec2Array(const HLString &key, const std::vector<glm::vec2> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec3Array(const HLString &key, const std::vector<glm::vec3> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec4Array(const HLString &key, const std::vector<glm::vec4> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat2Array(const HLString &key, const std::vector<glm::mat2> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat3Array(const HLString &key, const std::vector<glm::mat3> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat4Array(const HLString &key, const std::vector<glm::mat4> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteQuaternionArray(const HLString &key, const std::vector<glm::quat> &value)
	{
		return false;
	}
	
	bool YamlWriter::WriteStringArrayMap(const HLString &key, const std::map<HLString, HLString> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt32ArrayMap(const HLString &key, const std::map<HLString, int32> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt32ArrayMap(const HLString &key, const std::map<HLString, uint32> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt64ArrayMap(const HLString &key, const std::map<HLString, int64> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt64ArrayMap(const HLString &key, const std::map<HLString, uint64> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteBoolArrayMap(const HLString &key, const std::map<HLString, bool> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteFloatArrayMap(const HLString &key, const std::map<HLString, float> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteDoubleArrayMap(const HLString &key, const std::map<HLString, double> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec2ArrayMap(const HLString &key, const std::map<HLString, glm::vec2> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec3ArrayMap(const HLString &key, const std::map<HLString, glm::vec3> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec4ArrayMap(const HLString &key, const std::map<HLString, glm::vec4> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat2ArrayMap(const HLString &key, const std::map<HLString, glm::mat2> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat3ArrayMap(const HLString &key, const std::map<HLString, glm::mat3> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat4ArrayMap(const HLString &key, const std::map<HLString, glm::mat4> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteQuaternionArrayMap(const HLString &key, const std::map<HLString, glm::quat> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteStringArrayMap(const HLString &key, const std::unordered_map<HLString, HLString> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, int32> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, uint32> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, int64> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteUInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, uint64> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteBoolArrayMap(const HLString &key, const std::unordered_map<HLString, bool> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteFloatArrayMap(const HLString &key, const std::unordered_map<HLString, float> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteDoubleArrayMap(const HLString &key, const std::unordered_map<HLString, double> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec2> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec3> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteVec4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec4> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat2> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat3> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteMat4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat4> &map)
	{
		return false;
	}
	
	bool YamlWriter::WriteQuaternionArrayMap(const HLString &key, const std::unordered_map<HLString, glm::quat> &map)
	{
		return false;
	}
	
	bool YamlWriter::HasKey(const HLString &key) const
	{
		return false;
	}
	
	bool YamlWriter::WriteOut()
	{
		return false;
	}
	
	HLString YamlWriter::GetContent(bool prettify)
	{
		return HLString();
	}
	
	void YamlWriter::SetContent(const HLString &content)
	{
	}
}

