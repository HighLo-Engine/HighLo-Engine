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
		m_RootNode = m_Document.allocate_node(rapidxml::node_element, "HighLo");
	
		m_EngineVersion = HLApplication::Get().GetApplicationSettings().Version;
		char *versionStr = utils::PrepareString(m_Document, m_EngineVersion);

		rapidxml::xml_attribute<> *attribute = m_Document.allocate_attribute("version", versionStr);
		m_RootNode->append_attribute(attribute);
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

			// First check if the user likes to use a normal array or a arraymap
			// the difference is in the reading part, arraymaps return the key as well but arrays only return their value

			bool useArrayMap = false;
			rapidxml::xml_node<> *arrayNode = nullptr;

			for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
			{
				rapidxml::xml_attribute<> *keyAttribute = m_TempBuffers[i]->first_attribute("key");
				if (keyAttribute)
				{
					useArrayMap = true;
					break;
				}
			}

			if (useArrayMap)
				arrayNode = m_Document.allocate_node(rapidxml::node_element, "arraymap");
			else
				arrayNode = m_Document.allocate_node(rapidxml::node_element, "array");

			for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
			{
				arrayNode->append_node(m_TempBuffers[i]);
			}

			m_RootNode->append_node(arrayNode);
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
				m_RootNode->append_node(m_TempBuffer);
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
			char *valueStr = utils::PrepareString(instance->m_Document, HLString::ToString(value));

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "float");
			node->value(valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteDouble(const HLString &key, double value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Double, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			char *valueStr = utils::PrepareString(instance->m_Document, HLString::ToString(value));

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "double");
			node->value(valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteInt32(const HLString &key, int32 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Int32, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			char *valueStr = utils::PrepareString(instance->m_Document, HLString::ToString(value));

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "int32");
			node->value(valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteUInt32(const HLString &key, uint32 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::UInt32, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			char *valueStr = utils::PrepareString(instance->m_Document, HLString::ToString(value));

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "uint32");
			node->value(valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteInt64(const HLString &key, int64 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Int64, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			char *valueStr = utils::PrepareString(instance->m_Document, HLString::ToString(value));

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "int64");
			node->value(valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteUInt64(const HLString &key, uint64 value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::UInt64, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			char *valueStr = utils::PrepareString(instance->m_Document, HLString::ToString(value));

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "uint64");
			node->value(valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteBool(const HLString &key, bool value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Bool, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			char *valueStr = utils::PrepareString(instance->m_Document, HLString::ToString(value));

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "bool");
			node->value(valueStr);
			return node;
		});
	}

	bool XMLWriter::WriteString(const HLString &key, const HLString &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::String, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			char *valueStr = utils::PrepareString(instance->m_Document, value);

			rapidxml::xml_node<> *node = instance->m_Document.allocate_node(rapidxml::node_element, "string");
			node->value(valueStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, value[i]);

				entryNode->value(currentStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
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
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
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
		return ReadArray(key, DocumentDataType::String, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			HLString value = node->value();
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return ReadArray(key, DocumentDataType::Int32, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			int32 value = utils::LexicalCast<int32, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return ReadArray(key, DocumentDataType::UInt32, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			uint32 value = utils::LexicalCast<uint32, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return ReadArray(key, DocumentDataType::Int64, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			int64 value = utils::LexicalCast<int64, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return ReadArray(key, DocumentDataType::UInt64, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			uint64 value = utils::LexicalCast<uint64, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return ReadArray(key, DocumentDataType::Bool, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			bool value = utils::LexicalCast<bool, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return ReadArray(key, DocumentDataType::Float, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			float value = utils::LexicalCast<float, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return ReadArray(key, DocumentDataType::Double, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			double value = utils::LexicalCast<double, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}

	bool XMLWriter::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return ReadArray(key, DocumentDataType::Vec2, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			glm::vec2 vec;
			bool success = utils::XmlNodeToVec2(node, &vec);
			if (success)
			{
				result.push_back(vec);
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return ReadArray(key, DocumentDataType::Vec3, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			glm::vec3 vec;
			bool success = utils::XmlNodeToVec3(node, &vec);
			if (success)
			{
				result.push_back(vec);
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return ReadArray(key, DocumentDataType::Vec4, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			glm::vec4 vec;
			bool success = utils::XmlNodeToVec4(node, &vec);
			if (success)
			{
				result.push_back(vec);
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return ReadArray(key, DocumentDataType::Mat2, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			glm::mat2 mat;
			bool success = utils::XmlNodeToMat2(node, &mat);
			if (success)
			{
				result.push_back(mat);
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return ReadArray(key, DocumentDataType::Mat3, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			glm::mat3 mat;
			bool success = utils::XmlNodeToMat3(node, &mat);
			if (success)
			{
				result.push_back(mat);
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return ReadArray(key, DocumentDataType::Mat4, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			glm::mat4 mat;
			bool success = utils::XmlNodeToMat4(node, &mat);
			if (success)
			{
				result.push_back(mat);
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return ReadArray(key, DocumentDataType::Quat, [&result](rapidxml::xml_node<> *node) mutable -> bool
		{
			glm::quat q;
			bool success = utils::XmlNodeToQuat(node, &q);
			if (success)
			{
				result.push_back(q);
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return ReadArrayMap(key, DocumentDataType::String, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			HLString value = v->value();
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int32, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			int32 value = utils::LexicalCast<int32, char*>(v->value());
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt32, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			uint32 value = utils::LexicalCast<uint32, char*>(v->value());
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int64, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			int64 value = utils::LexicalCast<int64, char*>(v->value());
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt64, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			uint64 value = utils::LexicalCast<uint64, char *>(v->value());
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Bool, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			bool value = utils::LexicalCast<bool, char*>(v->value());
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Float, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			float value = utils::LexicalCast<float, char*>(v->value());
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Double, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			double value = utils::LexicalCast<double, char*>(v->value());
			result.insert({ k, value });
			return true;
		});
	}

	bool XMLWriter::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			glm::vec2 vec;
			bool success = utils::XmlNodeToVec2(v, &vec);
			if (success)
			{
				result.insert({ k, vec });
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec3, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			glm::vec3 vec;
			bool success = utils::XmlNodeToVec3(v, &vec);
			if (success)
			{
				result.insert({ k, vec });
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec4, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			glm::vec4 vec;
			bool success = utils::XmlNodeToVec4(v, &vec);
			if (success)
			{
				result.insert({ k, vec });
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat2, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			glm::mat2 mat;
			bool success = utils::XmlNodeToMat2(v, &mat);
			if (success)
			{
				result.insert({ k, mat });
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat3, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			glm::mat3 mat;
			bool success = utils::XmlNodeToMat3(v, &mat);
			if (success)
			{
				result.insert({ k, mat });
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat4, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			glm::mat4 mat;
			bool success = utils::XmlNodeToMat4(v, &mat);
			if (success)
			{
				result.insert({ k, mat });
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Quat, [&result](HLString &k, rapidxml::xml_node<> *v) mutable -> bool
		{
			glm::quat q;
			bool success = utils::XmlNodeToQuat(v, &q);
			if (success)
			{
				result.insert({ k, q });
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadFloat(const HLString &key, float *result)
	{
		return Read(key, DocumentDataType::Float, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			float value = utils::LexicalCast<float, char*>(node->value());
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadDouble(const HLString &key, double *result)
	{
		return Read(key, DocumentDataType::Double, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			double value = utils::LexicalCast<double, char*>(node->value());
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadInt32(const HLString &key, int32 *result)
	{
		return Read(key, DocumentDataType::Int32, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			int32 value = utils::LexicalCast<int32, char*>(node->value());
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadUInt32(const HLString &key, uint32 *result)
	{
		return Read(key, DocumentDataType::UInt32, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			uint32 value = utils::LexicalCast<uint32, char*>(node->value());
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadInt64(const HLString &key, int64 *result)
	{
		return Read(key, DocumentDataType::Int64, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			int64 value = utils::LexicalCast<int64>(node->value());
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadUInt64(const HLString &key, uint64 *result)
	{
		return Read(key, DocumentDataType::UInt64, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			uint64 value = utils::LexicalCast<uint64, char*>(node->value());
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadBool(const HLString &key, bool *result)
	{
		return Read(key, DocumentDataType::Bool, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			bool value = utils::LexicalCast<bool, char*>(node->value());
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadString(const HLString &key, HLString *result)
	{
		return Read(key, DocumentDataType::String, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			HLString value = node->value();
			*result = value;
			return true;
		});
	}

	bool XMLWriter::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return Read(key, DocumentDataType::Vec2, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			glm::vec2 vec;
			bool success = utils::XmlNodeToVec2(node, &vec);
			if (success)
			{
				*result = vec;
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return Read(key, DocumentDataType::Vec3, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			glm::vec3 vec;
			bool success = utils::XmlNodeToVec3(node, &vec);
			if (success)
			{
				*result = vec;
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return Read(key, DocumentDataType::Vec4, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			glm::vec4 vec;
			bool success = utils::XmlNodeToVec4(node, &vec);
			if (success)
			{
				*result = vec;
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return Read(key, DocumentDataType::Mat2, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			glm::mat2 mat;
			bool success = utils::XmlNodeToMat2(node, &mat);
			if (success)
			{
				*result = mat;
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return Read(key, DocumentDataType::Mat3, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			glm::mat3 mat;
			bool success = utils::XmlNodeToMat3(node, &mat);
			if (success)
			{
				*result = mat;
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return Read(key, DocumentDataType::Mat4, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			glm::mat4 mat;
			bool success = utils::XmlNodeToMat4(node, &mat);
			if (success)
			{
				*result = mat;
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return Read(key, DocumentDataType::Quat, [result](rapidxml::xml_node<> *node) mutable -> bool
		{
			if (!result)
				return false;

			glm::quat q;
			bool success = utils::XmlNodeToQuat(node, &q);
			if (success)
			{
				*result = q;
				return true;
			}

			return false;
		});
	}

	bool XMLWriter::HasKey(const HLString &key) const
	{
		bool anyItemFound = false;
		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
		if (!rootNode)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find a root node named HighLo! [-]");
			return false;
		}

		for (rapidxml::xml_node<> *it = rootNode->first_node(); it; it = it->next_sibling())
		{
			rapidxml::xml_attribute<> *currentKeyAttrib = it->first_attribute("key");
			if (!currentKeyAttrib)
			{
				// If a element has no key attribute check it's children
				bool anyChildHasKey = HasKeyInternal(it, key);
				if (anyChildHasKey)
					return true;

				continue;
			}

			HLString k = currentKeyAttrib->value();
			if (k == key)
			{
				anyItemFound = true;
				break;
			}
		}

		return anyItemFound;
	}

	bool XMLWriter::HasKeyInternal(rapidxml::xml_node<> *parent, const HLString &key) const
	{
		bool anyItemFound = false;

		if (!parent)
			return false;

		for (rapidxml::xml_node<> *it = parent->first_node(); it; it = it->next_sibling())
		{
			rapidxml::xml_attribute<> *currentKeyAttrib = it->first_attribute("key");
			if (!currentKeyAttrib)
			{
				return HasKeyInternal(it, key);
			}

			HLString k = currentKeyAttrib->value();
			if (k == key)
			{
				anyItemFound = true;
				break;
			}
		}

		return anyItemFound;
	}

	bool XMLWriter::WriteOut()
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

			char *str = utils::PrepareString(m_Document, content);
			m_Document.parse<0>(str);

			rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo"); // Our XML format should always have a root node named "HighLo"
			if (rootNode)
			{
				// Check the version string of the root node, that indicates the engine version
				rapidxml::xml_attribute<> *versionAttr = rootNode->first_attribute("version");
				if (!versionAttr)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX  "[-] Error: XML format is wrong! Expected version attribute to be present in HighLo node! [-]");
				}
				else
				{
					HLString versionStr = versionAttr->value();

					if (versionStr != m_EngineVersion)
					{
						// TODO: if we change the XML structure in the future we can parse old formats here to keep the parser backward compatible
						HL_CORE_WARN(XML_LOG_PREFIX "[-] The config file {0} is outdated! Going to re-format the file... [-]", **m_FilePath);
					}
				}
			}

			return true;
		}
		else
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: File {0} not found! [-]", **m_FilePath);
		}

		return false;
	}

	HLString XMLWriter::GetContent(bool prettify)
	{
		rapidxml::xml_attribute<> *versionAttr = m_Document.allocate_attribute("version", "1.0");
		rapidxml::xml_attribute<> *encodingAttr = m_Document.allocate_attribute("encoding", "UTF-8");

		rapidxml::xml_node<> *headerNode = m_Document.allocate_node(rapidxml::node_declaration);
		headerNode->append_attribute(versionAttr);
		headerNode->append_attribute(encodingAttr);

		m_Document.append_node(headerNode);
		m_Document.append_node(m_RootNode);

		int32 flags = 0;
		if (!prettify)
			flags = rapidxml::print_no_indenting;

		std::string s;
		rapidxml::print(std::back_inserter(s), m_Document, flags);
		return s.c_str();
	}

	void XMLWriter::SetContent(const HLString &content)
	{
		char *str = utils::PrepareString(m_Document, content);
		m_Document.parse<0>(str);

		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo"); // Our XML format should always have a root node named "HighLo"
		if (rootNode)
		{
			// Check the version string of the root node, that indicates the engine version
			rapidxml::xml_attribute<> *versionAttr = rootNode->first_attribute("version");
			if (!versionAttr)
			{
				HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: XML format is wrong! Expected version attribute to be present in HighLo node! [-]");
			}
			else
			{
				HLString versionStr = versionAttr->value();

				if (versionStr != m_EngineVersion)
				{
					HL_CORE_WARN(XML_LOG_PREFIX "[-] The config file {0} is outdated! Going to re-format the file... [-]", **m_FilePath);
				}
			}
		}
	}

	bool XMLWriter::Write(const HLString &key, DocumentDataType type, const std::function<rapidxml::xml_node<>*()> &insertFunc)
	{
		rapidxml::xml_node<> *node = insertFunc();
		if (!node)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Node could not be created! [-]");
			return false;
		}

		if (!key.IsEmpty())
		{
			rapidxml::xml_attribute<> *valueAttribute = m_Document.allocate_attribute("key", utils::PrepareString(m_Document, key));
			node->append_attribute(valueAttribute);
		}

		if (m_ShouldWriteIntoArray || m_ShouldWriteIntoObject)
		{
			m_TempBuffer = node;

			if (m_ShouldWriteIntoArray && !m_ShouldWriteIntoObject)
			{
				m_TempBuffers.push_back(node);
			}
		}
		else
		{
			m_RootNode->append_node(node);
		}

		return true;
	}
	
	bool XMLWriter::Read(const HLString &key, DocumentDataType type, const std::function<bool(rapidxml::xml_node<>*)> &insertFunc)
	{
		bool anyItemFound = false;
		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
		if (!rootNode)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find a root node named HighLo! [-]");
			return false;
		}

		// Now find any nodes of the type
		HLString typeStr = utils::DocumentDataTypeToString(type);
		for (rapidxml::xml_node<> *it = rootNode->first_node(*typeStr); it; it = it->next_sibling(*typeStr))
		{
			if (key.IsEmpty())
			{
				bool result = insertFunc(it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}

				continue;
			}

			// Here we can assume that the user needs a specifc key attribute
			rapidxml::xml_attribute<> *keyAttribute = it->first_attribute("key");
			if (!keyAttribute)
			{
				HL_CORE_ERROR("[-] Error: Expected to find any key attribute with the value {0} [-]", *key);
				return false;
			}

			HLString k = keyAttribute->value();
			if (k == key)
			{
				bool result = insertFunc(it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}
			}
		}

		return anyItemFound;
	}
	
	bool XMLWriter::ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(rapidxml::xml_node<>*)> &insertFunc)
	{
		bool anyItemFound = false;
		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
		if (!rootNode)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find a root node named HighLo! [-]");
			return false;
		}

		if (key.IsEmpty())
		{
			rapidxml::xml_node<> *firstNode = rootNode->first_node("array");
			if (!firstNode)
			{
				HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find any array node! [-]");
				return false;
			}

			// Now find any nodes of the type
			HLString typeStr = utils::DocumentDataTypeToString(type);
			for (rapidxml::xml_node<> *it = firstNode->first_node(*typeStr); it; it = it->next_sibling(*typeStr))
			{
				bool result = insertFunc(it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}
			}
		}
		else
		{
			for (rapidxml::xml_node<> *it = rootNode->first_node("array"); it; it = it->next_sibling("array"))
			{
				rapidxml::xml_attribute<> *arrayKey = it->first_attribute("key");
				if (!arrayKey)
				{
					// This array must be wrong, it has no key!
					continue;
				}

				HLString arrayKeyStr = arrayKey->value();
				if (arrayKeyStr == key)
				{
					// Now iterate over the value type and call the user defined conversion function
					HLString typeStr = utils::DocumentDataTypeToString(type);
					for (rapidxml::xml_node<> *typeIt = it->first_node(*typeStr); typeIt; typeIt = typeIt->next_sibling(*typeStr))
					{
						bool result = insertFunc(typeIt);
						if (!result)
						{
							HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
							return false;
						}
						else
						{
							anyItemFound = true;
						}
					}
				}
			}
		}

		return anyItemFound;
	}
	
	bool XMLWriter::ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(HLString&, rapidxml::xml_node<>*)> &insertFunc)
	{
		bool anyItemFound = false;
		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
		if (!rootNode)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find a root node named HighLo! [-]");
			return false;
		}

		if (key.IsEmpty())
		{
			rapidxml::xml_node<> *firstNode = rootNode->first_node("arraymap");
			if (!firstNode)
			{
				HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find any arraymap node! [-]");
				return false;
			}

			// Now find any nodes of the type
			HLString typeStr = utils::DocumentDataTypeToString(type);
			for (rapidxml::xml_node<> *it = firstNode->first_node(*typeStr); it; it = it->next_sibling(*typeStr))
			{
				rapidxml::xml_attribute<> *keyAttribute = it->first_attribute("key");
				if (!keyAttribute)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected a key attribute on node {0} [-]", it->name());
					return false;
				}

				HLString k = keyAttribute->value();
				bool result = insertFunc(k, it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}
			}
		}
		else
		{
			for (rapidxml::xml_node<> *it = rootNode->first_node("arraymap"); it; it = it->next_sibling("arraymap"))
			{
				rapidxml::xml_attribute<> *arrayMapKey = it->first_attribute("key");
				if (!arrayMapKey)
				{
					// This arraymap must be wrong, it has no key!
					continue;
				}

				HLString arrayMapKeyStr = arrayMapKey->value();
				if (arrayMapKeyStr == key)
				{
					// Now iterate over the value type and call the user defined conversion function
					HLString typeStr = utils::DocumentDataTypeToString(type);
					for (rapidxml::xml_node<> *typeIt = it->first_node(*typeStr); typeIt; typeIt = typeIt->next_sibling(*typeStr))
					{
						rapidxml::xml_attribute<> *keyAttribute = typeIt->first_attribute("key");
						if (!keyAttribute)
						{
							HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected a key attribute on node {0} [-]", typeIt->name());
							return false;
						}

						HLString k = keyAttribute->value();
						bool result = insertFunc(k, typeIt);
						if (!result)
						{
							HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
							return false;
						}
						else
						{
							anyItemFound = true;
						}
					}
				}
			}
		}

		return anyItemFound;
	}
}

