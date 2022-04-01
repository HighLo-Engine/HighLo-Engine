// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-08) initial release
//

/**
 * =========================================================================================================
 *													API usage
 * =========================================================================================================
 *
 * Example #1:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->WriteString("Hello", "World");
 * writer->WriteUint64("number", 64);
 * writer->WriteOut();
 *
 * -> should result in XML:
 * <?xml version="1.0.0">
 * <HighLo version="1.0.0">
 *   <string key="Hello">World</string>
 *   <uint64 key="number">64</uint64>
 * </HighLo>
 *
 * Example #2:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->BeginObject();
 * writer->WriteString("Hello", "World");
 * writer->WriteUInt64("number", 64);
 * writer->EndObject();
 * writer->WriterOut();
 *
 * -> should result in XML:
 * <?xml version="1.0.0">
 * <HighLo version="1.0.0">
 *   <string key="Hello">World</string>
 *   <uint64 key="number">64</uint64>
 * </HighLo>
 *
 * Example #3:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->BeginArray();
 * writer->WriteString("Hello", "World");
 * writer->WriteUInt64("number", 64);
 * writer->EndArray();
 * writer->WriteOut();
 *
 * -> should result in XML:
 * <?xml version="1.0.0">
 * <HighLo version="1.0.0">
 *   <array>
 *     <string>World</string>
 *     <uint64>64</uint64>
 *   </array>
 * </HighLo>
 *
 * Example #4:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->BeginArray();
 * for ()
 *   writer->BeginObject();
 *   writer->WriteString("Hello", "World");
 *   writer->WriteUInt64("number", 64);
 *   writer->EndObject();
 * endfor
 * writer->EndArray();
 * writer->WriteOut();
 *
 * -> should result in XML:
 * <?xml version="1.0.0">
 * <HighLo version="1.0.0">
 *   <arraymap>
 *     <string key="Hello">World</string>
 *     <uint64 key="number">64</uint64>
 *   </arraymap>
 * </HighLo>
 */

#pragma once

#include "Engine/Loaders/DocumentWriter.h"

#include <rapidxml/rapidxml.hpp>

namespace highlo
{
	class XMLWriter : public DocumentWriter
	{
	public:

		XMLWriter(const FileSystemPath &filePath);
		virtual ~XMLWriter();

		virtual void BeginArray() override;
		virtual void EndArray(const HLString &key = "") override;

		virtual void BeginObject() override;
		virtual void EndObject(const HLString &key = "") override;

		virtual void EnableTypeSafety() override { m_UseTypeSafety = true; }
		virtual void DisableTypeSafety() override { HL_ASSERT(m_UseTypeSafety, "Type safety is already disabled, did you forget to call EnableTypeSafety()?");  m_UseTypeSafety = false; }

		virtual bool WriteFloat(const HLString &key, float value) override;
		virtual bool WriteDouble(const HLString &key, double value) override;
		virtual bool WriteInt32(const HLString &key, int32 value) override;
		virtual bool WriteUInt32(const HLString &key, uint32 value) override;
		virtual bool WriteInt64(const HLString &key, int64 value) override;
		virtual bool WriteUInt64(const HLString &key, uint64 value) override;
		virtual bool WriteBool(const HLString &key, bool value) override;
		virtual bool WriteString(const HLString &key, const HLString &value) override;

		virtual bool WriteVec2(const HLString &key, const glm::vec2 &value) override;
		virtual bool WriteVec3(const HLString &key, const glm::vec3 &value) override;
		virtual bool WriteVec4(const HLString &key, const glm::vec4 &value) override;
		virtual bool WriteMat2(const HLString &key, const glm::mat2 &value) override;
		virtual bool WriteMat3(const HLString &key, const glm::mat3 &value) override;
		virtual bool WriteMat4(const HLString &key, const glm::mat4 &value) override;
		virtual bool WriteQuaternion(const HLString &key, const glm::quat &value) override;

		virtual bool WriteStringArray(const HLString &key, const std::vector<HLString> &value) override;
		virtual bool WriteInt32Array(const HLString &key, const std::vector<int32> &value) override;
		virtual bool WriteUInt32Array(const HLString &key, const std::vector<uint32> &value) override;
		virtual bool WriteInt64Array(const HLString &key, const std::vector<int64> &value) override;
		virtual bool WriteUInt64Array(const HLString &key, const std::vector<uint64> &value) override;
		virtual bool WriteBoolArray(const HLString &key, const std::vector<bool> &value) override;
		virtual bool WriteFloatArray(const HLString &key, const std::vector<float> &value) override;
		virtual bool WriteDoubleArray(const HLString &key, const std::vector<double> &value) override;
		virtual bool WriteVec2Array(const HLString &key, const std::vector<glm::vec2> &value) override;
		virtual bool WriteVec3Array(const HLString &key, const std::vector<glm::vec3> &value) override;
		virtual bool WriteVec4Array(const HLString &key, const std::vector<glm::vec4> &value) override;
		virtual bool WriteMat2Array(const HLString &key, const std::vector<glm::mat2> &value) override;
		virtual bool WriteMat3Array(const HLString &key, const std::vector<glm::mat3> &value) override;
		virtual bool WriteMat4Array(const HLString &key, const std::vector<glm::mat4> &value) override;
		virtual bool WriteQuaternionArray(const HLString &key, const std::vector<glm::quat> &value) override;

		virtual bool WriteStringArrayMap(const HLString &key, const std::map<HLString, HLString> &map) override;
		virtual bool WriteInt32ArrayMap(const HLString &key, const std::map<HLString, int32> &map) override;
		virtual bool WriteUInt32ArrayMap(const HLString &key, const std::map<HLString, uint32> &map) override;
		virtual bool WriteInt64ArrayMap(const HLString &key, const std::map<HLString, int64> &map) override;
		virtual bool WriteUInt64ArrayMap(const HLString &key, const std::map<HLString, uint64> &map) override;
		virtual bool WriteBoolArrayMap(const HLString &key, const std::map<HLString, bool> &map) override;
		virtual bool WriteFloatArrayMap(const HLString &key, const std::map<HLString, float> &map) override;
		virtual bool WriteDoubleArrayMap(const HLString &key, const std::map<HLString, double> &map) override;
		virtual bool WriteVec2ArrayMap(const HLString &key, const std::map<HLString, glm::vec2> &map) override;
		virtual bool WriteVec3ArrayMap(const HLString &key, const std::map<HLString, glm::vec3> &map) override;
		virtual bool WriteVec4ArrayMap(const HLString &key, const std::map<HLString, glm::vec4> &map) override;
		virtual bool WriteMat2ArrayMap(const HLString &key, const std::map<HLString, glm::mat2> &map) override;
		virtual bool WriteMat3ArrayMap(const HLString &key, const std::map<HLString, glm::mat3> &map) override;
		virtual bool WriteMat4ArrayMap(const HLString &key, const std::map<HLString, glm::mat4> &map) override;
		virtual bool WriteQuaternionArrayMap(const HLString &key, const std::map<HLString, glm::quat> &map) override;

		virtual bool WriteOut() override;
		virtual HLString GetContent(bool prettify = false) override;
		virtual void SetContent(const HLString &content) override;

	private:

		FileSystemPath m_FilePath;
		rapidxml::xml_document<> m_Document;
		rapidxml::xml_node<> *m_RootNode = nullptr;

		bool m_ShouldWriteIntoArray = false;
		bool m_ShouldWriteIntoObject = false;
		bool m_UseTypeSafety = true;
	};
}

