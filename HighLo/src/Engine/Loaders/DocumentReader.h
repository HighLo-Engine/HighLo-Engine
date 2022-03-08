// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-03) initial release
//

#pragma once

#include "Engine/Core/FileSystemPath.h"
#include "DocumentType.h"

namespace highlo
{
	class DocumentReader : public IsSharedReference
	{
	public:

		HLAPI virtual ~DocumentReader() {}

		HLAPI virtual bool ReadFloat(const HLString &key, float *result) = 0;
		HLAPI virtual bool ReadDouble(const HLString &key, double *result) = 0;
		HLAPI virtual bool ReadInt32(const HLString &key, int32 *result) = 0;
		HLAPI virtual bool ReadUInt32(const HLString &key, uint32 *result) = 0;
		HLAPI virtual bool ReadInt64(const HLString &key, int64 *result) = 0;
		HLAPI virtual bool ReadUInt64(const HLString &key, uint64 *result) = 0;
		HLAPI virtual bool ReadBool(const HLString &key, bool *result) = 0;
		HLAPI virtual bool ReadString(const HLString &key, HLString *result) = 0;

		HLAPI virtual bool ReadVector2(const HLString &key, glm::vec2 *result) = 0;
		HLAPI virtual bool ReadVector3(const HLString &key, glm::vec3 *result) = 0;
		HLAPI virtual bool ReadVector4(const HLString &key, glm::vec4 *result) = 0;
		HLAPI virtual bool ReadMatrix2(const HLString &key, glm::mat2 *result) = 0;
		HLAPI virtual bool ReadMatrix3(const HLString &key, glm::mat3 *result) = 0;
		HLAPI virtual bool ReadMatrix4(const HLString &key, glm::mat4 *result) = 0;
		HLAPI virtual bool ReadQuaternion(const HLString &key, glm::quat *result) = 0;

		HLAPI virtual bool ReadStringArray(const HLString &key, std::vector<HLString> &result) = 0;
		HLAPI virtual bool ReadInt32Array(const HLString &key, std::vector<int32> &result) = 0;
		HLAPI virtual bool ReadUInt32Array(const HLString &key, std::vector<uint32> &result) = 0;
		HLAPI virtual bool ReadInt64Array(const HLString &key, std::vector<int64> &result) = 0;
		HLAPI virtual bool ReadUInt64Array(const HLString &key, std::vector<uint64> &result) = 0;
		HLAPI virtual bool ReadBoolArray(const HLString &key, std::vector<bool> &result) = 0;
		HLAPI virtual bool ReadFloatArray(const HLString &key, std::vector<float> &result) = 0;
		HLAPI virtual bool ReadDoubleArray(const HLString &key, std::vector<double> &result) = 0;
		HLAPI virtual bool ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result) = 0;
		HLAPI virtual bool ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result) = 0;
		HLAPI virtual bool ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result) = 0;
		HLAPI virtual bool ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result) = 0;
		HLAPI virtual bool ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result) = 0;
		HLAPI virtual bool ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result) = 0;
		HLAPI virtual bool ReadQuatArray(const HLString &key, std::vector<glm::quat> &result) = 0;

		HLAPI virtual bool ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result) = 0;
		HLAPI virtual bool ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result) = 0;
		HLAPI virtual bool ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result) = 0;
		HLAPI virtual bool ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result) = 0;
		HLAPI virtual bool ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result) = 0;
		HLAPI virtual bool ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result) = 0;
		HLAPI virtual bool ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result) = 0;
		HLAPI virtual bool ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result) = 0;
		HLAPI virtual bool ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result) = 0;
		HLAPI virtual bool ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result) = 0;
		HLAPI virtual bool ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result) = 0;
		HLAPI virtual bool ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result) = 0;
		HLAPI virtual bool ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result) = 0;
		HLAPI virtual bool ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result) = 0;
		HLAPI virtual bool ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result) = 0;

		HLAPI virtual bool ReadContents(const FileSystemPath &filePath = "") = 0;
		HLAPI virtual HLString GetContent(bool prettify = false) = 0;
		HLAPI virtual void SetContent(const HLString &content) = 0;

		HLAPI static Ref<DocumentReader> Create(const FileSystemPath &filePath, DocumentType type = DocumentType::None);
	};
}

