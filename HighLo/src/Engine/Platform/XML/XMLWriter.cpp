// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "XMLWriter.h"

#include <rapidxml/rapidxml_print.hpp>

#include "Engine/Core/FileSystem.h"
#include "Engine/Application/Application.h"
#include "Engine/Utils/LoaderUtils.h"

#define XML_LOG_PREFIX "XMLWriter>    "

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
		m_ShouldWriteIntoArray = true;
	}

	void XMLWriter::EndArray(const HLString &key, bool rawData)
	{
		if (m_ShouldWriteIntoArray)
		{
			m_ShouldWriteIntoArray = false;

			for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
			{
				m_Document.append_node(m_TempBuffers[i]);
			}
		}
	}

	void XMLWriter::BeginObject()
	{
		m_ShouldWriteIntoObject = true;
	}

	void XMLWriter::EndObject(bool rawData)
	{
		if (m_ShouldWriteIntoObject)
		{
			m_ShouldWriteIntoObject = false;

			if (!m_ShouldWriteIntoArray)
			{
				m_Document.append_node(m_TempBuffer);
			}
			else
			{
				m_TempBuffers.push_back(m_TempBuffer);
			}
		}
	}

	bool XMLWriter::WriteFloat(const HLString &key, float value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Float, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			HLString valueStr = HLString::ToString(value);
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteDouble(const HLString &key, double value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Double, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			HLString valueStr = HLString::ToString(value);
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteInt32(const HLString &key, int32 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Int32, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			HLString valueStr = HLString::ToString(value);
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteUInt32(const HLString &key, uint32 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::UInt32, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			HLString valueStr = HLString::ToString(value);
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteInt64(const HLString &key, int64 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Int64, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			HLString valueStr = HLString::ToString(value);
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteUInt64(const HLString &key, uint64 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::UInt64, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			HLString valueStr = HLString::ToString(value);
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteBool(const HLString &key, bool value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Bool, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			HLString valueStr = HLString::ToString(value);
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteString(const HLString &key, const HLString &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::String, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			node->value(*value);
			return node;
		});
	}

	bool XMLWriter::WriteVec2(const HLString &key, const glm::vec2 &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Vec2, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			return utils::Vec2ToXmlNode(instance->m_Document, value);
		});
	}

	bool XMLWriter::WriteVec3(const HLString &key, const glm::vec3 &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Vec3, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			return utils::Vec3ToXmlNode(instance->m_Document, value);
		});
	}

	bool XMLWriter::WriteVec4(const HLString &key, const glm::vec4 &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Vec4, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			return utils::Vec4ToXmlNode(instance->m_Document, value);
		});
	}

	bool XMLWriter::WriteMat2(const HLString &key, const glm::mat2 &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Mat2, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			return utils::Mat2ToXmlNode(instance->m_Document, value);
		});
	}

	bool XMLWriter::WriteMat3(const HLString &key, const glm::mat3 &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Mat3, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			return utils::Mat3ToXmlNode(instance->m_Document, value);
		});
	}

	bool XMLWriter::WriteMat4(const HLString &key, const glm::mat4 &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Mat4, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			return utils::Mat4ToXmlNode(instance->m_Document, value);
		});
	}

	bool XMLWriter::WriteQuaternion(const HLString &key, const glm::quat &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			return utils::QuatToXmlNode(instance->m_Document, value);
		});
	}

	bool XMLWriter::WriteStringArray(const HLString &key, std::vector<HLString> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "string");
				HLString currentStr = value[i];

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteInt32Array(const HLString &key, std::vector<int32> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "int32");
				HLString currentStr = HLString::ToString(value[i]);

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}
	
	bool XMLWriter::WriteUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "uint32");
				HLString currentStr = HLString::ToString(value[i]);

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteInt64Array(const HLString &key, std::vector<int64> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "int64");
				HLString currentStr = HLString::ToString(value[i]);

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "uint64");
				HLString currentStr = HLString::ToString(value[i]);

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteBoolArray(const HLString &key, std::vector<bool> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "bool");
				HLString currentStr = HLString::ToString(value[i]);

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteFloatArray(const HLString &key, std::vector<float> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "float");
				HLString currentStr = HLString::ToString(value[i]);

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteDoubleArray(const HLString &key, std::vector<double> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data, "double");
				HLString currentStr = HLString::ToString(value[i]);

				entryNode->value(*currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Vec2ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Vec3ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Vec4ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Mat2ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Mat3ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Mat4ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteQuaternionArray(const HLString & key, std::vector<glm::quat> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_type::node_data);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::QuatToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
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

	bool XMLWriter::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return false;
	}

	bool XMLWriter::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
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
		// Write content into file
		HL_CORE_INFO(XML_LOG_PREFIX "[+] Writing file {0} [+]", **m_FilePath);
		HLString content = GetContent(true);

		FILE *file = fopen(**m_FilePath, "w");
		if (!file)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Could not open file {0} [-]", **m_FilePath);
			return false;
		}

		fwrite(*content, sizeof(char), content.Length(), file);
		fclose(file);
		return true;
	}

	bool XMLWriter::ReadContents(const FileSystemPath &filePath)
	{
		if (!filePath.String().IsEmpty())
			m_FilePath = filePath;

		if (FileSystem::Get()->FileExists(m_FilePath))
		{
			HLString content = FileSystem::Get()->ReadTextFile(m_FilePath);
			if (content.IsEmpty())
				return false;

			m_Document.parse<0>(*content);
			m_RootNode = m_Document.first_node("HighLo"); // Our XML format should always have a root node named "HighLo"

			// Check the version string of the root node, that indicates the engine version
			rapidxml::xml_attribute<> *versionAttr = m_RootNode->first_attribute("version");
			HLString versionStr = versionAttr->value();

			if (versionStr != HLApplication::Get().GetApplicationSettings().Version)
			{
				HL_CORE_WARN(XML_LOG_PREFIX "[-] The config file {0} is outdated! Going to re-format the file... [-]", **m_FilePath);
			}
		}
		else
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: File {0} not found! [-]", **m_FilePath);
		}

		return false;
	}

	HLString XMLWriter::GetContent(bool prettify) const
	{
		std::string str;
		rapidxml::print(std::back_inserter(str), m_Document);
		return HLString(str);
	}

	void XMLWriter::SetContent(const HLString &content)
	{
		char *str = (char*)content.C_Str();
		m_Document.parse<0>(str);
		m_RootNode = m_Document.first_node("HighLo"); // Our XML format should always have a root node named "HighLo"

		// Check the version string of the root node, that indicates the engine version
		rapidxml::xml_attribute<> *versionAttr = m_RootNode->first_attribute("version");
		HLString versionStr = versionAttr->value();

		if (versionStr != HLApplication::Get().GetApplicationSettings().Version)
		{
			HL_CORE_WARN(XML_LOG_PREFIX "[-] The config file {0} is outdated! Going to re-format the file... [-]", **m_FilePath);
		}
	}

	bool XMLWriter::AddIntoStructure(rapidxml::xml_node<> *value, DocumentDataType type)
	{
		bool result = false;

		if (m_ShouldWriteIntoArray || m_ShouldWriteIntoObject)
		{
			result = true;
			m_TempBuffer = value;

			if (m_ShouldWriteIntoArray && !m_ShouldWriteIntoObject)
			{
				m_TempBuffers.push_back(value);
			}
		}

		return result;
	}

	bool XMLWriter::Write(const HLString &key, DocumentDataType type, const std::function<rapidxml::xml_node<>*()> &insertFunc)
	{
		rapidxml::xml_node<> *valueNode = insertFunc();

		if (!key.IsEmpty())
		{
			rapidxml::xml_attribute<> *valueAttribute = m_Document.allocate_attribute("key", *key);
			valueNode->append_attribute(valueAttribute);
		}

		if (!AddIntoStructure(valueNode, type))
		{
			m_Document.append_node(valueNode);
		}

		return true;
	}

	rapidxml::xml_node<> *XMLWriter::GetKeyAsNode(const HLString &key)
	{
		rapidxml::xml_node<> *node = m_Document.allocate_node(rapidxml::node_type::node_data);
		node->value(*key);
		return node;
	}
	
	bool XMLWriter::Read(const HLString &key, const std::function<bool(rapidxml::xml_node<>*)> &insertFunc)
	{
		return false;
	}
	
	bool XMLWriter::ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(rapidxml::xml_node<>*)> &insertFunc)
	{
		return false;
	}
	
	bool XMLWriter::ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(HLString, rapidxml::xml_node<>*)> &insertFunc)
	{
		return false;
	}
}

