#include "HighLoPch.h"
#include "YamlReader.h"

#include <yaml-cpp/yaml.h>
#include "YamlHelper.h"

namespace highlo
{
	YamlReader::YamlReader(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}

	YamlReader::~YamlReader()
	{
	}
	
	bool YamlReader::ReadFloat(const HLString &key, float *result)
	{
		return false;
	}
	
	bool YamlReader::ReadDouble(const HLString &key, double *result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt32(const HLString &key, int32 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt32(const HLString &key, uint32 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt64(const HLString &key, int64 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt64(const HLString &key, uint64 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadBool(const HLString &key, bool *result)
	{
		return false;
	}
	
	bool YamlReader::ReadString(const HLString &key, HLString *result)
	{
		return false;
	}
	
	bool YamlReader::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return false;
	}
	
	bool YamlReader::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return false;
	}
	
	bool YamlReader::ReadStringArray(const HLString &key, std::vector<HLString> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadStringArrayMap(const HLString &key, std::unordered_map<HLString, HLString> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt32ArrayMap(const HLString &key, std::unordered_map<HLString, int32> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt32ArrayMap(const HLString &key, std::unordered_map<HLString, uint32> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadInt64ArrayMap(const HLString &key, std::unordered_map<HLString, int64> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadUInt64ArrayMap(const HLString &key, std::unordered_map<HLString, uint64> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadBoolArrayMap(const HLString &key, std::unordered_map<HLString, bool> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadFloatArrayMap(const HLString &key, std::unordered_map<HLString, float> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadDoubleArrayMap(const HLString &key, std::unordered_map<HLString, double> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec2ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec2> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec3ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec3> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadVec4ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec4> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat2ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat2> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat3ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat3> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadMat4ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat4> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadQuatArrayMap(const HLString &key, std::unordered_map<HLString, glm::quat> &result)
	{
		return false;
	}
	
	bool YamlReader::ReadContents(const FileSystemPath &filePath)
	{
		return false;
	}
	
	HLString YamlReader::GetContent(bool prettify)
	{
		return HLString();
	}
	
	void YamlReader::SetContent(const HLString &content)
	{
	}
}

