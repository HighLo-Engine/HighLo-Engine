// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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

		if (HLApplication::Get().HasStarted())
		{
			m_EngineVersion = HLApplication::Get().GetApplicationSettings().Version;
			char *versionStr = utils::PrepareString(m_Document, m_EngineVersion);

			rapidxml::xml_attribute<> *attribute = m_Document.allocate_attribute("version", versionStr);
			m_RootNode->append_attribute(attribute);
		}
		else
		{
			m_EngineVersion = "1.0.0";
		}
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

			if (!key.IsEmpty())
			{
				char *keyStr = utils::PrepareString(m_Document, key);
				rapidxml::xml_attribute<> *keyAttribute = m_Document.allocate_attribute("key", keyStr);
				arrayNode->append_attribute(keyAttribute);
			}

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

	bool XMLWriter::WriteStringArray(const HLString &key, const std::vector<HLString> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "string");
				char *currentStr = utils::PrepareString(instance->m_Document, value[i]);

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteInt32Array(const HLString &key, const std::vector<int32> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "int32");
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteUInt32Array(const HLString &key, const std::vector<uint32> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "uint32");
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteInt64Array(const HLString &key, const std::vector<int64> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "int64");
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteUInt64Array(const HLString &key, const std::vector<uint64> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "uint64");
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteBoolArray(const HLString &key, const std::vector<bool> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "bool");
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteFloatArray(const HLString &key, const std::vector<float> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "float");
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteDoubleArray(const HLString &key, const std::vector<double> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = instance->m_Document.allocate_node(rapidxml::node_element, "double");
				char *currentStr = utils::PrepareString(instance->m_Document, HLString::ToString(value[i]));

				entryNode->value(currentStr);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteVec2Array(const HLString &key, const std::vector<glm::vec2> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Vec2ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteVec3Array(const HLString &key, const std::vector<glm::vec3> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Vec3ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteVec4Array(const HLString &key, const std::vector<glm::vec4> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Vec4ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteMat2Array(const HLString &key, const std::vector<glm::mat2> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Mat2ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteMat3Array(const HLString &key, const std::vector<glm::mat3> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Mat3ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteMat4Array(const HLString &key, const std::vector<glm::mat4> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::Mat4ToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteQuaternionArray(const HLString &key, const std::vector<glm::quat> &value)
	{
		Ref<XMLWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [value, instance]() mutable -> rapidxml::xml_node<>*
		{
			rapidxml::xml_node<> *arrayNode = instance->m_Document.allocate_node(rapidxml::node_element, "array");
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidxml::xml_node<> *entryNode = utils::QuatToXmlNode(instance->m_Document, value[i]);
				arrayNode->append_node(entryNode);
			}

			return arrayNode;
		});
	}

	bool XMLWriter::WriteStringArrayMap(const HLString &key, const std::map<HLString, HLString> &map)
	{
		return false;
	}

	bool XMLWriter::WriteInt32ArrayMap(const HLString &key, const std::map<HLString, int32> &map)
	{
		return false;
	}

	bool XMLWriter::WriteUInt32ArrayMap(const HLString &key, const std::map<HLString, uint32> &map)
	{
		return false;
	}

	bool XMLWriter::WriteInt64ArrayMap(const HLString &key, const std::map<HLString, int64> &map)
	{
		return false;
	}

	bool XMLWriter::WriteUInt64ArrayMap(const HLString &key, const std::map<HLString, uint64> &map)
	{
		return false;
	}

	bool XMLWriter::WriteBoolArrayMap(const HLString &key, const std::map<HLString, bool> &map)
	{
		return false;
	}

	bool XMLWriter::WriteFloatArrayMap(const HLString &key, const std::map<HLString, float> &map)
	{
		return false;
	}

	bool XMLWriter::WriteDoubleArrayMap(const HLString &key, const std::map<HLString, double> &map)
	{
		return false;
	}

	bool XMLWriter::WriteVec2ArrayMap(const HLString &key, const std::map<HLString, glm::vec2> &map)
	{
		return false;
	}

	bool XMLWriter::WriteVec3ArrayMap(const HLString &key, const std::map<HLString, glm::vec3> &map)
	{
		return false;
	}

	bool XMLWriter::WriteVec4ArrayMap(const HLString &key, const std::map<HLString, glm::vec4> &map)
	{
		return false;
	}

	bool XMLWriter::WriteMat2ArrayMap(const HLString &key, const std::map<HLString, glm::mat2> &map)
	{
		return false;
	}

	bool XMLWriter::WriteMat3ArrayMap(const HLString &key, const std::map<HLString, glm::mat3> &map)
	{
		return false;
	}

	bool XMLWriter::WriteMat4ArrayMap(const HLString &key, const std::map<HLString, glm::mat4> &map)
	{
		return false;
	}

	bool XMLWriter::WriteQuaternionArrayMap(const HLString &key, const std::map<HLString, glm::quat> &map)
	{
		return false;
	}

	bool XMLWriter::WriteStringArrayMap(const HLString &key, const std::unordered_map<HLString, HLString> &map)
	{
		return false;
	}

	bool XMLWriter::WriteInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, int32> &map)
	{
		return false;
	}

	bool XMLWriter::WriteUInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, uint32> &map)
	{
		return false;
	}

	bool XMLWriter::WriteInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, int64> &map)
	{
		return false;
	}

	bool XMLWriter::WriteUInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, uint64> &map)
	{
		return false;
	}

	bool XMLWriter::WriteBoolArrayMap(const HLString &key, const std::unordered_map<HLString, bool> &map)
	{
		return false;
	}

	bool XMLWriter::WriteFloatArrayMap(const HLString &key, const std::unordered_map<HLString, float> &map)
	{
		return false;
	}

	bool XMLWriter::WriteDoubleArrayMap(const HLString &key, const std::unordered_map<HLString, double> &map)
	{
		return false;
	}

	bool XMLWriter::WriteVec2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec2> &map)
	{
		return false;
	}

	bool XMLWriter::WriteVec3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec3> &map)
	{
		return false;
	}

	bool XMLWriter::WriteVec4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec4> &map)
	{
		return false;
	}

	bool XMLWriter::WriteMat2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat2> &map)
	{
		return false;
	}

	bool XMLWriter::WriteMat3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat3> &map)
	{
		return false;
	}

	bool XMLWriter::WriteMat4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat4> &map)
	{
		return false;
	}

	bool XMLWriter::WriteQuaternionArrayMap(const HLString &key, const std::unordered_map<HLString, glm::quat> &map)
	{
		return false;
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

	bool XMLWriter::Write(const HLString &key, DocumentDataType type, const std::function<rapidxml::xml_node<> *()> &insertFunc)
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
}
