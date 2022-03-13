// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "XMLWriter.h"

#include "XMLHelper.h"

#include <rapidxml/rapidxml_print.hpp>
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
	
	void XMLWriter::EndArray(const HLString &key)
	{
		if (m_ShouldWriteIntoArray)
		{
			m_ShouldWriteIntoArray = false;


		}
	}
	
	void XMLWriter::BeginObject()
	{
		m_ShouldWriteIntoObject = true;
	}
	
	void XMLWriter::EndObject(const HLString &key)
	{
		if (m_ShouldWriteIntoObject)
		{
			m_ShouldWriteIntoObject = false;


		}
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
	
	bool XMLWriter::WriteStringArray(const HLString &key, const std::vector<HLString> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteInt32Array(const HLString &key, const std::vector<int32> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteUInt32Array(const HLString &key, const std::vector<uint32> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteInt64Array(const HLString &key, const std::vector<int64> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteUInt64Array(const HLString &key, const std::vector<uint64> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteBoolArray(const HLString &key, const std::vector<bool> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteFloatArray(const HLString &key, const std::vector<float> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteDoubleArray(const HLString &key, const std::vector<double> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteVec2Array(const HLString &key, const std::vector<glm::vec2> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteVec3Array(const HLString &key, const std::vector<glm::vec3> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteVec4Array(const HLString &key, const std::vector<glm::vec4> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteMat2Array(const HLString &key, const std::vector<glm::mat2> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteMat3Array(const HLString &key, const std::vector<glm::mat3> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteMat4Array(const HLString &key, const std::vector<glm::mat4> &value)
	{
		return false;
	}
	
	bool XMLWriter::WriteQuaternionArray(const HLString &key, const std::vector<glm::quat> &value)
	{
		return false;
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
	
	bool XMLWriter::WriteOut()
	{
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

		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo"); // Our XML format should always have a root named "HighLo"
		if (rootNode)
		{
			rapidxml::xml_attribute<> *versionAttr = rootNode->first_attribute("version");
			if (versionAttr)
			{
				// TODO: Check if the version string is current
			}
			else
			{
				HL_CORE_WARN(XML_LOG_PREFIX "[-] Could not find any version attribute in the HighLo root node! [-]");
			}
		}
	}
}

