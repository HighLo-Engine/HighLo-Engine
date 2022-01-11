// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-10) initial release
//

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
		HLAPI virtual void EndArray(const HLString &key = "") = 0;

		HLAPI virtual void BeginObject() = 0;
		HLAPI virtual void EndObject() = 0;

		HLAPI virtual bool Write(const HLString &key, float value) = 0;
		HLAPI virtual bool Write(const HLString &key, double value) = 0;
		HLAPI virtual bool Write(const HLString &key, int32 value) = 0;
		HLAPI virtual bool Write(const HLString &key, uint32 value) = 0;
		HLAPI virtual bool Write(const HLString &key, int64 value) = 0;
		HLAPI virtual bool Write(const HLString &key, uint64 value) = 0;
		HLAPI virtual bool Write(const HLString &key, bool value) = 0;
		HLAPI virtual bool Write(const HLString &key, const HLString &value) = 0;

		HLAPI virtual bool Write(const HLString &key, std::vector<HLString> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<int32> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<uint32> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<int64> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<uint64> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<bool> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<float> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<double> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<glm::vec2> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<glm::vec3> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<glm::vec4> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<glm::mat2> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<glm::mat3> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<glm::mat4> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, std::vector<glm::quat> &value) = 0;

		HLAPI virtual bool Write(const HLString &key, const glm::vec2 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::vec3 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::vec4 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::mat2 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::mat3 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::mat4 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::quat &value) = 0;

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
		HLAPI virtual bool Readint64ArrayMap(const HLString &key, std::map<HLString, int64> &value) = 0;
		HLAPI virtual bool ReadUint64ArrayMap(const HLString &key, std::map<HLString, uint64> &value) = 0;
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
		HLAPI virtual bool WriteOut() const = 0;
		HLAPI virtual bool ReadContents(const FileSystemPath &filePath = "") = 0;
		HLAPI virtual HLString GetContent() const = 0;

		HLAPI static Ref<DocumentWriter> Create(const FileSystemPath &filePath, DocumentType type = DocumentType::None);
	};
}

