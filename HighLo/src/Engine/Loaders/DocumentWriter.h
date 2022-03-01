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
 * // you can specify a specific subobjects from which you want to retrieve the array, let's say for example we have the json file:
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

		HLAPI virtual bool WriteStringArray(const HLString &key, std::vector<HLString> &value) = 0;
		HLAPI virtual bool WriteInt32Array(const HLString &key, std::vector<int32> &value) = 0;
		HLAPI virtual bool WriteUInt32Array(const HLString &key, std::vector<uint32> &value) = 0;
		HLAPI virtual bool WriteInt64Array(const HLString &key, std::vector<int64> &value) = 0;
		HLAPI virtual bool WriteUInt64Array(const HLString &key, std::vector<uint64> &value) = 0;
		HLAPI virtual bool WriteBoolArray(const HLString &key, std::vector<bool> &value) = 0;
		HLAPI virtual bool WriteFloatArray(const HLString &key, std::vector<float> &value) = 0;
		HLAPI virtual bool WriteDoubleArray(const HLString &key, std::vector<double> &value) = 0;
		HLAPI virtual bool WriteVec2Array(const HLString &key, std::vector<glm::vec2> &value) = 0;
		HLAPI virtual bool WriteVec3Array(const HLString &key, std::vector<glm::vec3> &value) = 0;
		HLAPI virtual bool WriteVec4Array(const HLString &key, std::vector<glm::vec4> &value) = 0;
		HLAPI virtual bool WriteMat2Array(const HLString &key, std::vector<glm::mat2> &value) = 0;
		HLAPI virtual bool WriteMat3Array(const HLString &key, std::vector<glm::mat3> &value) = 0;
		HLAPI virtual bool WriteMat4Array(const HLString &key, std::vector<glm::mat4> &value) = 0;
		HLAPI virtual bool WriteQuaternionArray(const HLString &key, std::vector<glm::quat> &value) = 0;

		HLAPI virtual bool ReadStringArray(const HLString &key, std::vector<HLString> &value) = 0;
		HLAPI virtual bool ReadInt32Array(const HLString &key, std::vector<int32> &value) = 0;
		HLAPI virtual bool ReadUInt32Array(const HLString &key, std::vector<uint32> &value) = 0;
		HLAPI virtual bool ReadInt64Array(const HLString &key, std::vector<int64> &value) = 0;
		HLAPI virtual bool ReadUInt64Array(const HLString &key, std::vector<uint64> &value) = 0;
		HLAPI virtual bool ReadBoolArray(const HLString &key, std::vector<bool> &value) = 0;
		HLAPI virtual bool ReadFloatArray(const HLString &key, std::vector<float> &value) = 0;
		HLAPI virtual bool ReadDoubleArray(const HLString &key, std::vector<double> &value) = 0;
		HLAPI virtual bool ReadVec2Array(const HLString &key, std::vector<glm::vec2> &value) = 0;
		HLAPI virtual bool ReadVec3Array(const HLString &key, std::vector<glm::vec3> &value) = 0;
		HLAPI virtual bool ReadVec4Array(const HLString &key, std::vector<glm::vec4> &value) = 0;
		HLAPI virtual bool ReadMat2Array(const HLString &key, std::vector<glm::mat2> &value) = 0;
		HLAPI virtual bool ReadMat3Array(const HLString &key, std::vector<glm::mat3> &value) = 0;
		HLAPI virtual bool ReadMat4Array(const HLString &key, std::vector<glm::mat4> &value) = 0;
		HLAPI virtual bool ReadQuatArray(const HLString &key, std::vector<glm::quat> &value) = 0;

		HLAPI virtual bool ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &value) = 0;
		HLAPI virtual bool ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &value) = 0;
		HLAPI virtual bool ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &value) = 0;
		HLAPI virtual bool ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &value) = 0;
		HLAPI virtual bool ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &value) = 0;
		HLAPI virtual bool ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &value) = 0;
		HLAPI virtual bool ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &value) = 0;
		HLAPI virtual bool ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &value) = 0;
		HLAPI virtual bool ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &value) = 0;
		HLAPI virtual bool ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &value) = 0;
		HLAPI virtual bool ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &value) = 0;
		HLAPI virtual bool ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &value) = 0;
		HLAPI virtual bool ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &value) = 0;
		HLAPI virtual bool ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &value) = 0;
		HLAPI virtual bool ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &value) = 0;

		HLAPI virtual bool ReadFloat(const HLString &key, float *value) = 0;
		HLAPI virtual bool ReadDouble(const HLString &key, double *value) = 0;
		HLAPI virtual bool ReadInt32(const HLString &key, int32 *value) = 0;
		HLAPI virtual bool ReadUInt32(const HLString &key, uint32 *value) = 0;
		HLAPI virtual bool ReadInt64(const HLString &key, int64 *value) = 0;
		HLAPI virtual bool ReadUInt64(const HLString &key, uint64 *value) = 0;
		HLAPI virtual bool ReadBool(const HLString &key, bool *value) = 0;
		HLAPI virtual bool ReadString(const HLString &key, HLString *value) = 0;

		HLAPI virtual bool ReadVector2(const HLString &key, glm::vec2 *value) = 0;
		HLAPI virtual bool ReadVector3(const HLString &key, glm::vec3 *value) = 0;
		HLAPI virtual bool ReadVector4(const HLString &key, glm::vec4 *value) = 0;
		HLAPI virtual bool ReadMatrix2(const HLString &key, glm::mat2 *value) = 0;
		HLAPI virtual bool ReadMatrix3(const HLString &key, glm::mat3 *value) = 0;
		HLAPI virtual bool ReadMatrix4(const HLString &key, glm::mat4 *value) = 0;
		HLAPI virtual bool ReadQuaternion(const HLString &key, glm::quat *value) = 0;

		HLAPI virtual bool HasKey(const HLString &key) const = 0;
		HLAPI virtual bool WriteOut() = 0;
		HLAPI virtual bool ReadContents(const FileSystemPath &filePath = "") = 0;
		HLAPI virtual HLString GetContent(bool prettify = false) = 0;
		HLAPI virtual void SetContent(const HLString &content) = 0;

		HLAPI static Ref<DocumentWriter> Create(const FileSystemPath &filePath, DocumentType type = DocumentType::None);
	};
}

