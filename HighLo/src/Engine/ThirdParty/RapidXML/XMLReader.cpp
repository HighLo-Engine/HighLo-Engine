// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "XMLReader.h"

#if 0

namespace highlo
{
	XMLReader::XMLReader(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}
	
	XMLReader::~XMLReader()
	{
	}
	
	bool XMLReader::ReadFloat(const HLString &key, float *result)
	{
		return false;
	}
	
	bool XMLReader::ReadDouble(const HLString &key, double *result)
	{
		return false;
	}
	
	bool XMLReader::ReadInt32(const HLString &key, int32 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadUInt32(const HLString &key, uint32 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadInt64(const HLString &key, int64 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadUInt64(const HLString &key, uint64 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadBool(const HLString &key, bool *result)
	{
		return false;
	}
	
	bool XMLReader::ReadString(const HLString &key, HLString *result)
	{
		return false;
	}
	
	bool XMLReader::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return false;
	}
	
	bool XMLReader::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return false;
	}
	
	bool XMLReader::ReadStringArray(const HLString &key, std::vector<HLString> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return false;
	}
	
	bool XMLReader::ReadContents(const FileSystemPath &filePath)
	{
		return false;
	}
	
	HLString XMLReader::GetContent(bool prettify)
	{
		return HLString();
	}

	void XMLReader::SetContent(const HLString &content)
	{
	}
}

#endif
