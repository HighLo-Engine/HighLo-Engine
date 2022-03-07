// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-10) initial release
//

/**
 * usability improvement:
 * ----------------------
 * 
 * we currently have duplicated usage, you can achieve the same result with either:
 * std::vector<int32> arr;
 * arr.push_back(10);
 * arr.push_back(42);
 * 
 * writer->WriteInt32Array("test", arr);
 * 
 * will result in: {"test":[10,42]}, but the following produces the same output:
 * 
 * writer->BeginObject();
 * writer->WriteInt32Array("test", arr);
 * writer->EndObject(true);
 * 
 * a better usabilty version could be if the second version would only produce a typesafe variant. The typesafe variant currently gets generated by not providing a bool in EndObject()
 * The typesafe variant would currently look like:
 * {"type":"int32","value":{"test":[10,42]}}
 */

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
 * -> should result in json:
 * {
 *    "Hello":"World",
 *    "number":64
 * }
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
 * -> should result in json:
 * {
 *     "Hello":
 *     {
 *       "value":"World",
 *       "type":"string"
 *     },
 *     "number":
 *     {
 *       "value":64,
 *       "type":"uint64"
 *     }
 * }
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
 * -> should result in json:
 * [
 *   "World",
 *   64
 * ]
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
 * -> should result in json:
 * [
 *  {
 *     "Hello":"World",
 *     "type":"string"
 *   },
 *   {
 *     "number":64,
 *     "type":"uint64"
 *   }
 * ]
 */

#pragma once

#include "Engine/Loaders/DocumentWriter.h"

#include <rapidjson/document.h>

namespace highlo
{
	enum class NodeInsertType
	{
		Direct = 0,
		Array = 1,
		Object = 2,
		ArrayObject = 3
	};

	class JSONWriter : public DocumentWriter
	{
	public:

		JSONWriter(const FileSystemPath &filePath);
		virtual ~JSONWriter();

		virtual void BeginArray() override;
		virtual void EndArray(const HLString &key = "") override;

		virtual void BeginObject() override;
		virtual void EndObject(const HLString &key = "") override;

		virtual void EnableTypeSafety() override { m_UseTypeSafety = true; }
		virtual void DisableTypeSafety() override { m_UseTypeSafety = false; }

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

		template<typename T>
		rapidjson::Value ConvertStdArrToRapidJsonArr(const std::vector<T> &arr)
		{
			rapidjson::Value result(rapidjson::kArrayType);
			for (uint32 i = 0; i < arr.size(); ++i)
				result.PushBack(arr[i], m_Document.GetAllocator());

			return result;
		}

		bool Write(const HLString &key, DocumentDataType type, const std::function<rapidjson::Value()> &insertFunc);

	private:

		rapidjson::Document m_Document;
		FileSystemPath m_FilePath;
		bool m_UseTypeSafety = true;

		bool m_ShouldWriteIntoArray = false;
		bool m_ShouldWriteIntoObject = false;
		bool m_ObjectsNeverUsed = true;

		uint32 m_ObjectsPerGroup = 0;

		std::vector<std::pair<rapidjson::Value, rapidjson::Value>> m_ValuePairs;
		std::vector<std::pair<rapidjson::Value, rapidjson::Value>> m_TypePairs;
	};
}

