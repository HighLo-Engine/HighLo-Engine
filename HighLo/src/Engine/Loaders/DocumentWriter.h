// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-10) initial release
//

/**
 * Usage:
 *
 * // *Write into file examples*
 * Ref<DocumentWriter> writer = DocumentWriter::Create("test.json", DocumentType::Json); // valid options for DocumentType are: Json, Yaml, XML
 *
 * // Write an array into file
 * std::vector<uint32> arr;
 * arr.push_back(42);
 * arr.push_back(10);
 *
 * writer->BeginArray();
 * for (uint32 i = 0; i < arr.size(); ++i)
 * {
 *   writer->BeginObject();
 *   writer->WriteUInt32("object" + i, arr[i]);
 *   writer->EndObject();
 * }
 * writer->EndArray();
 *
 * // If the array itself should have a key name as well, you can provide it in the EndArray function as a optional parameter
 * // for example following code:
 * std::vector<uint32> arr;
 * arr.push_back(42);
 * arr.push_back(10);
 *
 * writer->BeginArray();
 * for (uint32 i = 0; i < arr.size(); ++i)
 * {
 *   writer->BeginObject();
 *   writer->WriteUInt32("object" + i, arr[i]);
 *   writer->EndObject();
 * }
 * writer->EndArray("testArray");
 *
 * // Would produce this json example:
 * {
 *   "testArray":
 *   [
 *     {
 *     "object0": 42
 *     },
 *     {
 *     "object1": 10
 *     },
 *   ]
 * }
 *
 * // Write a single object into file
 * uint32 value = 10;
 * writer->BeginObject();
 * writer->WriteUInt32("test", value);
 * writer->EndObject();
 *
 * // All code above just puts the data into the internal storage,
 * // with WriteOut you can actually write the content of the internal structure into a given file
 * writer->WriteOut();
 *
 *
 *
 * // *Read from file examples*
 * Ref<DocumentWriter> reader = DocumentWriter::Create("test.json", DocumentType::Json);
 *
 * // Reads the content from the actual file into the internal structure
 * reader->ReadContents();
 *
 * // The rest is very specific to your previous write process, but if you want to retrieve a array structure with it's keys you can do:
 * std::map<HLString, uint32> values;
 * reader->ReadUInt32ArrayMap("", values);
 *
 * // Or if you just need the values without the keys
 * std::vector<uint32> values;
 * reader->ReadUInt32Array("", values);
 *
 * // you can specify specific subobjects from which you want to retrieve the array, let's say for example we have the json file:
 * {
 *   "someKey": "someValue",
 *   "someArray": [
 *     {
 *       "test": 124
 *     },
 *     {
 *       "anotherTest": 42
 *     }
 *   ]
 * }
 *
 * // And if you now want to retrieve the array "someArray" you would want to do something like:
 * std::vector<uint32> values;
 * reader->ReadUInt32Array("someArray", values);
 * // or if you need the "test" key from inside the array as well
 * std::map<HLString, uint32> values;
 * reader->ReadUInt32ArrayMap("someArray", values);
 *
 *
 *
 * // If you just wrote a single object into the file in the previous step, you can do:
 * uint32 value;
 * reader->ReadUInt32("someKey", &value);
 *
 * // This would work for example with this json:
 * {
 *   "someKey": 42
 * }
 *
 * // value from line 100 would now then be 42.
 *
 * // If you don't wish to export into a file, but instead get the formatted value as a string you can just retrieve the string from the class as well with:
 * HLString content = reader->GetContent();
 */

#pragma once

#include "DocumentType.h"
#include "Engine/Core/FileSystemPath.h"

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace highlo
{
	/// <summary>
	/// This class is used as an interface to write/read JSON, XML and YAML files
	/// </summary>
	class DocumentWriter : public IsSharedReference
	{
	public:

		HLAPI virtual ~DocumentWriter() {}

		HLAPI virtual void BeginArray() = 0;
		HLAPI virtual void EndArray(const HLString &key = "", bool rawData = false) = 0;

		HLAPI virtual void BeginObject() = 0;
		HLAPI virtual void EndObject(bool rawData = false) = 0;

		HLAPI virtual bool WriteFloat(const HLString &key, float value) = 0;
		HLAPI virtual bool WriteDouble(const HLString &key, double value) = 0;
		HLAPI virtual bool WriteInt32(const HLString &key, int32 value) = 0;
		HLAPI virtual bool WriteUInt32(const HLString &key, uint32 value) = 0;
		HLAPI virtual bool WriteInt64(const HLString &key, int64 value) = 0;
		HLAPI virtual bool WriteUInt64(const HLString &key, uint64 value) = 0;
		HLAPI virtual bool WriteBool(const HLString &key, bool value) = 0;
		HLAPI virtual bool WriteString(const HLString &key, const HLString &value) = 0;

		HLAPI virtual bool WriteVec2(const HLString &key, const glm::vec2 &value) = 0;
		HLAPI virtual bool WriteVec3(const HLString &key, const glm::vec3 &value) = 0;
		HLAPI virtual bool WriteVec4(const HLString &key, const glm::vec4 &value) = 0;
		HLAPI virtual bool WriteMat2(const HLString &key, const glm::mat2 &value) = 0;
		HLAPI virtual bool WriteMat3(const HLString &key, const glm::mat3 &value) = 0;
		HLAPI virtual bool WriteMat4(const HLString &key, const glm::mat4 &value) = 0;
		HLAPI virtual bool WriteQuaternion(const HLString &key, const glm::quat &value) = 0;

		HLAPI virtual bool WriteStringArray(const HLString &key, const std::vector<HLString> &value) = 0;
		HLAPI virtual bool WriteInt32Array(const HLString &key, const std::vector<int32> &value) = 0;
		HLAPI virtual bool WriteUInt32Array(const HLString &key, const std::vector<uint32> &value) = 0;
		HLAPI virtual bool WriteInt64Array(const HLString &key, const std::vector<int64> &value) = 0;
		HLAPI virtual bool WriteUInt64Array(const HLString &key, const std::vector<uint64> &value) = 0;
		HLAPI virtual bool WriteBoolArray(const HLString &key, const std::vector<bool> &value) = 0;
		HLAPI virtual bool WriteFloatArray(const HLString &key, const std::vector<float> &value) = 0;
		HLAPI virtual bool WriteDoubleArray(const HLString &key, const std::vector<double> &value) = 0;
		HLAPI virtual bool WriteVec2Array(const HLString &key, const std::vector<glm::vec2> &value) = 0;
		HLAPI virtual bool WriteVec3Array(const HLString &key, const std::vector<glm::vec3> &value) = 0;
		HLAPI virtual bool WriteVec4Array(const HLString &key, const std::vector<glm::vec4> &value) = 0;
		HLAPI virtual bool WriteMat2Array(const HLString &key, const std::vector<glm::mat2> &value) = 0;
		HLAPI virtual bool WriteMat3Array(const HLString &key, const std::vector<glm::mat3> &value) = 0;
		HLAPI virtual bool WriteMat4Array(const HLString &key, const std::vector<glm::mat4> &value) = 0;
		HLAPI virtual bool WriteQuaternionArray(const HLString &key, const std::vector<glm::quat> &value) = 0;

		HLAPI virtual bool WriteStringArrayMap(const HLString &key, const std::map<HLString, HLString> &map) = 0;
		HLAPI virtual bool WriteInt32ArrayMap(const HLString &key, const std::map<HLString, int32> &map) = 0;
		HLAPI virtual bool WriteUInt32ArrayMap(const HLString &key, const std::map<HLString, uint32> &map) = 0;
		HLAPI virtual bool WriteInt64ArrayMap(const HLString &key, const std::map<HLString, int64> &map) = 0;
		HLAPI virtual bool WriteUInt64ArrayMap(const HLString &key, const std::map<HLString, uint64> &map) = 0;
		HLAPI virtual bool WriteBoolArrayMap(const HLString &key, const std::map<HLString, bool> &map) = 0;
		HLAPI virtual bool WriteFloatArrayMap(const HLString &key, const std::map<HLString, float> &map) = 0;
		HLAPI virtual bool WriteDoubleArrayMap(const HLString &key, const std::map<HLString, double> &map) = 0;

		HLAPI virtual bool WriteVec2ArrayMap(const HLString &key, const std::map<HLString, glm::vec2> &map) = 0;
		HLAPI virtual bool WriteVec3ArrayMap(const HLString &key, const std::map<HLString, glm::vec3> &map) = 0;
		HLAPI virtual bool WriteVec4ArrayMap(const HLString &key, const std::map<HLString, glm::vec4> &map) = 0;
		HLAPI virtual bool WriteMat2ArrayMap(const HLString &key, const std::map<HLString, glm::mat2> &map) = 0;
		HLAPI virtual bool WriteMat3ArrayMap(const HLString &key, const std::map<HLString, glm::mat3> &map) = 0;
		HLAPI virtual bool WriteMat4ArrayMap(const HLString &key, const std::map<HLString, glm::mat4> &map) = 0;
		HLAPI virtual bool WriteQuaternionArrayMap(const HLString &key, const std::map<HLString, glm::quat> &map) = 0;

		HLAPI virtual bool WriteStringArrayMap(const HLString &key, const std::unordered_map<HLString, HLString> &map) = 0;
		HLAPI virtual bool WriteInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, int32> &map) = 0;
		HLAPI virtual bool WriteUInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, uint32> &map) = 0;
		HLAPI virtual bool WriteInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, int64> &map) = 0;
		HLAPI virtual bool WriteUInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, uint64> &map) = 0;
		HLAPI virtual bool WriteBoolArrayMap(const HLString &key, const std::unordered_map<HLString, bool> &map) = 0;
		HLAPI virtual bool WriteFloatArrayMap(const HLString &key, const std::unordered_map<HLString, float> &map) = 0;
		HLAPI virtual bool WriteDoubleArrayMap(const HLString &key, const std::unordered_map<HLString, double> &map) = 0;

		HLAPI virtual bool WriteVec2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec2> &map) = 0;
		HLAPI virtual bool WriteVec3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec3> &map) = 0;
		HLAPI virtual bool WriteVec4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec4> &map) = 0;
		HLAPI virtual bool WriteMat2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat2> &map) = 0;
		HLAPI virtual bool WriteMat3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat3> &map) = 0;
		HLAPI virtual bool WriteMat4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat4> &map) = 0;
		HLAPI virtual bool WriteQuaternionArrayMap(const HLString &key, const std::unordered_map<HLString, glm::quat> &map) = 0;

		HLAPI virtual bool HasKey(const HLString &key) const = 0;
		HLAPI virtual bool WriteOut() = 0;
		
		HLAPI virtual HLString GetContent(bool prettify = false) = 0;
		HLAPI virtual void SetContent(const HLString &content) = 0;

		HLAPI static Ref<DocumentWriter> Create(const FileSystemPath &filePath, DocumentType type = DocumentType::None);
	};
}
