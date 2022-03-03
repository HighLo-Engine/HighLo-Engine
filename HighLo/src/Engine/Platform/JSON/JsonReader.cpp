// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "JsonReader.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/LoaderUtils.h"
#include "JsonHelper.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

namespace highlo
{
	JsonReader::JsonReader(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}

	JsonReader::~JsonReader()
	{
	}

	bool JsonReader::ReadFloat(const HLString &key, float *value)
	{
		return false;
	}
	
	bool JsonReader::ReadDouble(const HLString &key, double *value)
	{
		return false;
	}
	
	bool JsonReader::ReadInt32(const HLString &key, int32 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadUInt32(const HLString &key, uint32 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadInt64(const HLString &key, int64 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadUInt64(const HLString &key, uint64 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadBool(const HLString &key, bool *value)
	{
		return false;
	}
	
	bool JsonReader::ReadString(const HLString &key, HLString *value)
	{
		return false;
	}
	
	bool JsonReader::ReadVector2(const HLString &key, glm::vec2 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadVector3(const HLString &key, glm::vec3 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadVector4(const HLString &key, glm::vec4 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadMatrix2(const HLString &key, glm::mat2 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadMatrix3(const HLString &key, glm::mat3 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadMatrix4(const HLString &key, glm::mat4 *value)
	{
		return false;
	}
	
	bool JsonReader::ReadQuaternion(const HLString &key, glm::quat *value)
	{
		return false;
	}
	
	bool JsonReader::ReadStringArray(const HLString &key, std::vector<HLString> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadInt32Array(const HLString &key, std::vector<int32> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadInt64Array(const HLString &key, std::vector<int64> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadBoolArray(const HLString &key, std::vector<bool> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadFloatArray(const HLString &key, std::vector<float> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadDoubleArray(const HLString &key, std::vector<double> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadQuatArray(const HLString &key, std::vector<glm::quat> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &value)
	{
		return false;
	}
	
	bool JsonReader::ReadContents(const FileSystemPath &filePath)
	{
		return false;
	}
	
	HLString JsonReader::GetContent(bool prettify)
	{
		return HLString();
	}
	
	void JsonReader::SetContent(const HLString &content)
	{
	}
	
	bool JsonReader::Read(const HLString &key, DocumentDataType type, const std::function<bool(rapidjson::Value &)> &insertFunc)
	{
		return false;
	}
	
	bool JsonReader::ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(rapidjson::Value &)> &insertFunc)
	{
		return false;
	}
	
	bool JsonReader::ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(HLString, rapidjson::Value &)> &insertFunc)
	{
		return false;
	}
}

