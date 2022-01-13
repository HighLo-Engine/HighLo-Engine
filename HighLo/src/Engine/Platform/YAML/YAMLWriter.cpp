// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "YAMLWriter.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

#include "Engine/Assets/Asset.h"

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2 &v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			return node;
		}

		static bool decode(const Node &node, glm::vec2 &v)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3 &v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			return node;
		}

		static bool decode(const Node &node, glm::vec3 &v)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4 &v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.push_back(v.w);
			return node;
		}

		static bool decode(const Node &node, glm::vec4 &v)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			v.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat &q)
		{
			Node node;
			node.push_back(q.w);
			node.push_back(q.x);
			node.push_back(q.y);
			node.push_back(q.z);
			return node;
		}

		static bool decode(const Node &node, glm::quat &q)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			q.w = node[0].as<float>();
			q.x = node[1].as<float>();
			q.y = node[2].as<float>();
			q.z = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<highlo::AssetHandle>
	{
		static Node encode(const highlo::AssetHandle &handle)
		{
			Node node;
			node.push_back((uint64)handle);
			return node;
		}

		static bool decode(const Node &node, highlo::AssetHandle &handle)
		{
			handle = node.as<uint64>();
			return true;
		}
	};
}

namespace highlo
{
	inline YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec2 &v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 &v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 &v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter &operator<<(YAML::Emitter &out, const glm::quat &q)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << q.w << q.x << q.y << q.z << YAML::EndSeq;
		return out;
	}

	YAMLWriter::YAMLWriter(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}

	YAMLWriter::~YAMLWriter()
	{
	}

	void YAMLWriter::BeginArray()
	{
		m_ShouldWriteIntoArray = true;
	}

	void YAMLWriter::EndArray(const HLString &key, bool rawData)
	{
		if (m_ShouldWriteIntoArray)
		{
			m_ShouldWriteIntoArray = false;


		}
	}

	void YAMLWriter::BeginObject()
	{
		m_ShouldWriteIntoObject = true;
	}

	void YAMLWriter::EndObject(bool rawData)
	{
		if (m_ShouldWriteIntoObject)
		{
			m_ShouldWriteIntoObject = false;


		}
	}

	bool YAMLWriter::WriteFloat(const HLString &key, float value)
	{
		return false;
	}

	bool YAMLWriter::WriteDouble(const HLString &key, double value)
	{
		return false;
	}

	bool YAMLWriter::WriteInt32(const HLString &key, int32 value)
	{
		return false;
	}

	bool YAMLWriter::WriteUInt32(const HLString &key, uint32 value)
	{
		return false;
	}

	bool YAMLWriter::WriteInt64(const HLString &key, int64 value)
	{
		return false;
	}

	bool YAMLWriter::WriteUInt64(const HLString &key, uint64 value)
	{
		return false;
	}

	bool YAMLWriter::WriteBool(const HLString &key, bool value)
	{
		return false;
	}

	bool YAMLWriter::WriteString(const HLString &key, const HLString &value)
	{
		return false;
	}

	bool YAMLWriter::WriteVec2(const HLString &key, const glm::vec2 &value)
	{
		return false;
	}

	bool YAMLWriter::WriteVec3(const HLString &key, const glm::vec3 &value)
	{
		return false;
	}

	bool YAMLWriter::WriteVec4(const HLString &key, const glm::vec4 &value)
	{
		return false;
	}

	bool YAMLWriter::WriteMat2(const HLString &key, const glm::mat2 &value)
	{
		return false;
	}

	bool YAMLWriter::WriteMat3(const HLString &key, const glm::mat3 &value)
	{
		return false;
	}

	bool YAMLWriter::WriteMat4(const HLString &key, const glm::mat4 &value)
	{
		return false;
	}

	bool YAMLWriter::WriteQuaternion(const HLString &key, const glm::quat &value)
	{
		return false;
	}

	bool YAMLWriter::WriteStringArray(const HLString &key, std::vector<HLString> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteInt32Array(const HLString &key, std::vector<int32> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteInt64Array(const HLString &key, std::vector<int64> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteBoolArray(const HLString &key, std::vector<bool> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteFloatArray(const HLString &key, std::vector<float> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteDoubleArray(const HLString &key, std::vector<double> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		return false;
	}

	bool YAMLWriter::WriteQuaternionArray(const HLString &key, std::vector<glm::quat> &value)
	{
		return false;
	}

	bool YAMLWriter::ReadFloat(const HLString &key, float *result)
	{
		return false;
	}

	bool YAMLWriter::ReadDouble(const HLString &key, double *result)
	{
		return false;
	}

	bool YAMLWriter::ReadInt32(const HLString &key, int32 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt32(const HLString &key, uint32 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadInt64(const HLString &key, int64 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt64(const HLString &key, uint64 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadBool(const HLString &key, bool *result)
	{
		return false;
	}

	bool YAMLWriter::ReadString(const HLString &key, HLString *result)
	{
		return false;
	}

	bool YAMLWriter::ReadStringArray(const HLString &key, std::vector<HLString> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return false;
	}

	bool YAMLWriter::Readint64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadUint64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return false;
	}

	bool YAMLWriter::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return false;
	}

	bool YAMLWriter::HasKey(const HLString &key) const
	{
		return false;
	}

	bool YAMLWriter::WriteOut() const
	{
		// Write content into file
		HL_CORE_INFO("[+] Writing file {0} [+]", **m_FilePath);
		HLString content = GetContent(true);

		FILE *file = fopen(**m_FilePath, "w");
		if (!file)
		{
			HL_CORE_ERROR("[-] Could not open file {0} [-]", **m_FilePath);
			return false;
		}

		fwrite(*content, sizeof(char), content.Length(), file);
		fclose(file);
		return true;
	}

	bool YAMLWriter::ReadContents(const FileSystemPath &filePath)
	{
		if (!filePath.String().IsEmpty())
			m_FilePath = filePath;

		m_Document = YAML::LoadFile(**m_FilePath);
		return true;
	}

	HLString YAMLWriter::GetContent(bool prettify) const
	{
		return m_Emitter.c_str();
	}
}

