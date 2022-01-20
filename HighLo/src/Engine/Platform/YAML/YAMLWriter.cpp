// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "YAMLWriter.h"

#include "YAMLHelper.h"

#include "Engine/Assets/Asset.h"
#include "Engine/Core/FileSystem.h"

namespace YAML
{
	template<>
	struct convert<highlo::HLString>
	{
		static Node encode(const highlo::HLString &rhs)
		{
			return Node(std::string(rhs.C_Str()));
		}

		static bool decode(const Node &node, highlo::HLString &rhs)
		{
			if (!node.IsScalar())
				return false;

			rhs = node.Scalar();
			return true;
		}
	};
}

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
		m_ShouldWriteIntoArray = true;
	}

	void YAMLWriter::EndArray(const HLString &key, bool rawData)
	{
		if (m_ShouldWriteIntoArray)
		{
			m_ShouldWriteIntoArray = false;

			m_Emitter << YAML::BeginSeq;

			for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
			{
				YAML::Node &currentValue = m_TempBuffers[i].Node;
				DocumentDataType currentType = m_TempBuffers[i].Type;

				if (currentType == DocumentDataType::Float)
				{
					m_Emitter << YAML::FloatPrecision(3) << currentValue;
				}
				else if (currentType == DocumentDataType::Double)
				{
					m_Emitter << YAML::DoublePrecision(3) << currentValue;
				}
				else
				{
					m_Emitter << currentValue;
				}
			}

			m_Emitter << YAML::EndSeq;

			/*
			if (m_TempKeys.size() > 0)
			{
				m_Emitter << YAML::BeginMap;

				for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
				{
					HLString &currentKey = m_TempKeys[i];
					YAML::Node &currentValue = m_TempBuffers[i];
			
					m_Emitter << YAML::Value << currentValue;
				}

				m_Emitter << YAML::EndMap;
			}
			else
			{
				m_Emitter << YAML::Flow;
				m_Emitter << YAML::BeginSeq;

				for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
				{
					YAML::Node &currentValue = m_TempBuffers[i];
					m_Emitter << currentValue;
				}

				m_Emitter << YAML::EndSeq;
			}
			*/
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

			if (!m_ShouldWriteIntoArray)
			{
				if (!m_TempKey.IsEmpty())
				{
					m_Emitter << YAML::Key << m_TempKey.C_Str() << YAML::Value << m_TempBuffer.Node;
				}
				else
				{
					m_Emitter << YAML::Value << m_TempBuffer.Node;
				}
			}
			else
			{
				m_TempBuffers.push_back(m_TempBuffer);
				m_TempKeys.push_back(m_TempKey);
			}
		}
	}

	bool YAMLWriter::WriteFloat(const HLString &key, float value)
	{
		return Write(key, DocumentDataType::Float, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteDouble(const HLString &key, double value)
	{
		return Write(key, DocumentDataType::Double, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteInt32(const HLString &key, int32 value)
	{
		return Write(key, DocumentDataType::Int32, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteUInt32(const HLString &key, uint32 value)
	{
		return Write(key, DocumentDataType::UInt32, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteInt64(const HLString &key, int64 value)
	{
		return Write(key, DocumentDataType::Int64, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteUInt64(const HLString &key, uint64 value)
	{
		return Write(key, DocumentDataType::UInt64, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteBool(const HLString &key, bool value)
	{
		return Write(key, DocumentDataType::Bool, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteString(const HLString &key, const HLString &value)
	{
		return Write(key, DocumentDataType::String, [value]() -> YAML::Node
		{
			return YAML::Node(value);
		});
	}

	bool YAMLWriter::WriteVec2(const HLString &key, const glm::vec2 &value)
	{
		return Write(key, DocumentDataType::Vec2, [value]() -> YAML::Node
		{
			return utils::Vec2ToYAMLNode(value);
		});
	}

	bool YAMLWriter::WriteVec3(const HLString &key, const glm::vec3 &value)
	{
		return Write(key, DocumentDataType::Vec3, [value]() -> YAML::Node
		{
			return utils::Vec3ToYAMLNode(value);
		});
	}

	bool YAMLWriter::WriteVec4(const HLString &key, const glm::vec4 &value)
	{
		return Write(key, DocumentDataType::Vec4, [value]() -> YAML::Node
		{
			return utils::Vec4ToYAMLNode(value);
		});
	}

	bool YAMLWriter::WriteMat2(const HLString &key, const glm::mat2 &value)
	{
		return Write(key, DocumentDataType::Mat2, [value]() -> YAML::Node
		{
			return utils::Mat2ToYAMLNode(value);
		});
	}

	bool YAMLWriter::WriteMat3(const HLString &key, const glm::mat3 &value)
	{
		return Write(key, DocumentDataType::Mat3, [value]() -> YAML::Node
		{
			return utils::Mat3ToYAMLNode(value);
		});
	}

	bool YAMLWriter::WriteMat4(const HLString &key, const glm::mat4 &value)
	{
		return Write(key, DocumentDataType::Mat4, [value]() -> YAML::Node
		{
			return utils::Mat4ToYAMLNode(value);
		});
	}

	bool YAMLWriter::WriteQuaternion(const HLString &key, const glm::quat &value)
	{
		return Write(key, DocumentDataType::Quat, [value]() -> YAML::Node
		{
			return utils::QuatToYAMLNode(value);
		});
	}

	bool YAMLWriter::WriteStringArray(const HLString &key, std::vector<HLString> &value)
	{
		return Write(key, DocumentDataType::String, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i].C_Str());
			return node;
		});
	}

	bool YAMLWriter::WriteInt32Array(const HLString &key, std::vector<int32> &value)
	{
		return Write(key, DocumentDataType::Int32, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i]);
			return node;
		});
	}

	bool YAMLWriter::WriteUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		return Write(key, DocumentDataType::UInt32, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i]);
			return node;
		});
	}

	bool YAMLWriter::WriteInt64Array(const HLString &key, std::vector<int64> &value)
	{
		return Write(key, DocumentDataType::Int64, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i]);
			return node;
		});
	}

	bool YAMLWriter::WriteUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		return Write(key, DocumentDataType::UInt64, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i]);
			return node;
		});
	}

	bool YAMLWriter::WriteBoolArray(const HLString &key, std::vector<bool> &value)
	{
		return Write(key, DocumentDataType::Bool, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i]);
			return node;
		});
	}

	bool YAMLWriter::WriteFloatArray(const HLString &key, std::vector<float> &value)
	{
		return Write(key, DocumentDataType::Float, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i]);
			return node;
		});
	}

	bool YAMLWriter::WriteDoubleArray(const HLString &key, std::vector<double> &value)
	{
		return Write(key, DocumentDataType::Double, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
				node.push_back(value[i]);
			return node;
		});
	}

	bool YAMLWriter::WriteVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		return Write(key, DocumentDataType::Vec2, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				YAML::Node v = utils::Vec2ToYAMLNode(value[i]);
				node.push_back(v);
			}
			return node;
		});
	}

	bool YAMLWriter::WriteVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		return Write(key, DocumentDataType::Vec3, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				YAML::Node v = utils::Vec3ToYAMLNode(value[i]);
				node.push_back(v);
			}
			return node;
		});
	}

	bool YAMLWriter::WriteVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		return Write(key, DocumentDataType::Vec4, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				YAML::Node v = utils::Vec4ToYAMLNode(value[i]);
				node.push_back(v);
			}
			return node;
		});
	}

	bool YAMLWriter::WriteMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		return Write(key, DocumentDataType::Mat2, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				YAML::Node v = utils::Mat2ToYAMLNode(value[i]);
				node.push_back(v);
			}
			return node;
		});
	}

	bool YAMLWriter::WriteMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		return Write(key, DocumentDataType::Mat3, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				YAML::Node v = utils::Mat3ToYAMLNode(value[i]);
				node.push_back(v);
			}
			return node;
		});
	}

	bool YAMLWriter::WriteMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		return Write(key, DocumentDataType::Mat4, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				YAML::Node v = utils::Mat4ToYAMLNode(value[i]);
				node.push_back(v);
			}
			return node;
		});
	}

	bool YAMLWriter::WriteQuaternionArray(const HLString &key, std::vector<glm::quat> &value)
	{
		return Write(key, DocumentDataType::Quat, [value]() -> YAML::Node
		{
			YAML::Node node(YAML::NodeType::Sequence);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				YAML::Node v = utils::QuatToYAMLNode(value[i]);
				node.push_back(v);
			}
			return node;
		});
	}

	bool YAMLWriter::ReadFloat(const HLString &key, float *result)
	{
		return Read(key, DocumentDataType::Float, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			float value = node.as<float>();
			*result = value;
			return true;
		});
	}

	bool YAMLWriter::ReadDouble(const HLString &key, double *result)
	{
		return Read(key, DocumentDataType::Double, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			double value = node.as<double>();
			*result = value;
			return true;
		});
	}

	bool YAMLWriter::ReadInt32(const HLString &key, int32 *result)
	{
		return Read(key, DocumentDataType::Int32, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			int32 value = node.as<int32>();
			*result = value;
			return true;
		});
	}

	bool YAMLWriter::ReadUInt32(const HLString &key, uint32 *result)
	{
		return Read(key, DocumentDataType::UInt32, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			uint32 value = node.as<uint32>();
			*result = value;
			return true;
		});
	}

	bool YAMLWriter::ReadInt64(const HLString &key, int64 *result)
	{
		return Read(key, DocumentDataType::Int64, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			int64 value = node.as<int64>();
			*result = value;
			return true;
		});
	}

	bool YAMLWriter::ReadUInt64(const HLString &key, uint64 *result)
	{
		return Read(key, DocumentDataType::UInt64, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			uint64 value = node.as<uint64>();
			*result = value;
			return true;
		});
	}

	bool YAMLWriter::ReadBool(const HLString &key, bool *result)
	{
		return Read(key, DocumentDataType::Bool, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			bool value = node.as<bool>();
			*result = value;
			return true;
		});
	}

	bool YAMLWriter::ReadString(const HLString &key, HLString *result)
	{
		return Read(key, DocumentDataType::String, [result](YAML::Node &node) -> bool
		{
			if (node.Type() != YAML::NodeType::Scalar || node.IsMap() || node.IsSequence())
				return false;

			*result = node.as<HLString>();
			return true;
		});
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

	bool YAMLWriter::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return false;
	}

	bool YAMLWriter::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
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

	bool YAMLWriter::WriteOut()
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

		if (FileSystem::Get()->FileExists(m_FilePath))
		{
			HLString content = FileSystem::Get()->ReadTextFile(m_FilePath);
			if (content.IsEmpty())
				return false;

			m_Document = YAML::Load(*content);
			m_Emitter << m_Document;
		}
		else
		{
			HL_CORE_ERROR("[-] Error: File {0} not found! [-]", **m_FilePath);
		}

		return false;
	}

	HLString YAMLWriter::GetContent(bool prettify)
	{
		if (!prettify)
		{
			m_Emitter << YAML::Flow;
		}

		m_Emitter.SetIndent(4);

		HL_ASSERT(m_Emitter.good());
		return m_Emitter.c_str();
	}

	void YAMLWriter::SetContent(const HLString &content)
	{
		m_Document = YAML::Load(*content);
		m_Emitter << m_Document;
	}
	
	bool YAMLWriter::Write(const HLString &key, DocumentDataType type, const std::function<YAML::Node()> &insertFunc)
	{
		YAML::Node &node = insertFunc();
	//	if (node.IsNull() || !node.IsDefined())
	//		return false;

		if (m_ShouldWriteIntoArray || m_ShouldWriteIntoObject)
		{
			m_TempBuffer.Node = node;
			m_TempBuffer.Type = type;
			m_TempKey = key;

			if (m_ShouldWriteIntoArray && !m_ShouldWriteIntoObject)
			{
				ElementType t;
				t.Node = node;
				t.Type = type;
				m_TempBuffers.push_back(t);
				m_TempKeys.push_back(key);
			}
		}
		else
		{
			if (!key.IsEmpty())
			{
				m_Emitter << YAML::Key << key.C_Str() << YAML::Value << node;
			}
			else
			{
				m_Emitter << YAML::Value << node;
			}
		}

		return true;
	}

	bool YAMLWriter::Read(const HLString &key, DocumentDataType type, const std::function<bool(YAML::Node)> &insertFunc)
	{
		if (m_Document.IsSequence() || m_Document.IsMap() || m_Document.IsNull())
			return false;



		return true;
	}

	bool YAMLWriter::ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(YAML::Node&)> &insertFunc)
	{
		if (m_Document.IsMap() || m_Document.IsNull() || m_Document.IsScalar())
			return false;



		return true;
	}
	
	bool YAMLWriter::ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(HLString&, YAML::Node&)> &insertFunc)
	{
		if (m_Document.IsSequence() || m_Document.IsNull() || m_Document.IsScalar())
			return false;



		return true;
	}
}

