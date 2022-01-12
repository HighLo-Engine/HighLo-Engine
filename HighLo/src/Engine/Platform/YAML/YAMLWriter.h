// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-10) initial release
//

#pragma once

#include "Engine/Loaders/DocumentWriter.h"

#include <yaml-cpp/yaml.h>

namespace highlo
{
	class YAMLWriter : public DocumentWriter
	{
	public:

		YAMLWriter(const FileSystemPath &filePath);
		virtual ~YAMLWriter();

		virtual void BeginArray() override;
		virtual void EndArray(const HLString &key = "") override;

		virtual void BeginObject() override;
		virtual void EndObject() override;

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

		virtual bool WriteStringArray(const HLString &key, std::vector<HLString> &value) override;
		virtual bool WriteInt32Array(const HLString &key, std::vector<int32> &value) override;
		virtual bool WriteUInt32Array(const HLString &key, std::vector<uint32> &value) override;
		virtual bool WriteInt64Array(const HLString &key, std::vector<int64> &value) override;
		virtual bool WriteUInt64Array(const HLString &key, std::vector<uint64> &value) override;
		virtual bool WriteBoolArray(const HLString &key, std::vector<bool> &value) override;
		virtual bool WriteFloatArray(const HLString &key, std::vector<float> &value) override;
		virtual bool WriteDoubleArray(const HLString &key, std::vector<double> &value) override;

		virtual bool WriteVec2Array(const HLString &key, std::vector<glm::vec2> &value) override;
		virtual bool WriteVec3Array(const HLString &key, std::vector<glm::vec3> &value) override;
		virtual bool WriteVec4Array(const HLString &key, std::vector<glm::vec4> &value) override;
		virtual bool WriteMat2Array(const HLString &key, std::vector<glm::mat2> &value) override;
		virtual bool WriteMat3Array(const HLString &key, std::vector<glm::mat3> &value) override;
		virtual bool WriteMat4Array(const HLString &key, std::vector<glm::mat4> &value) override;
		virtual bool WriteQuaternionArray(const HLString &key, std::vector<glm::quat> &value) override;

		virtual bool ReadFloat(const HLString &key, float *result) override;
		virtual bool ReadDouble(const HLString &key, double *result) override;
		virtual bool ReadInt32(const HLString &key, int32 *result) override;
		virtual bool ReadUInt32(const HLString &key, uint32 *result) override;
		virtual bool ReadInt64(const HLString &key, int64 *result) override;
		virtual bool ReadUInt64(const HLString &key, uint64 *result) override;
		virtual bool ReadBool(const HLString &key, bool *result) override;
		virtual bool ReadString(const HLString &key, HLString *result) override;

		virtual bool ReadStringArray(const HLString &key, std::vector<HLString> &result) override;
		virtual bool ReadInt32Array(const HLString &key, std::vector<int32> &result) override;
		virtual bool ReadUInt32Array(const HLString &key, std::vector<uint32> &result) override;
		virtual bool ReadInt64Array(const HLString &key, std::vector<int64> &result) override;
		virtual bool ReadUInt64Array(const HLString &key, std::vector<uint64> &result) override;
		virtual bool ReadBoolArray(const HLString &key, std::vector<bool> &result) override;
		virtual bool ReadFloatArray(const HLString &key, std::vector<float> &result) override;
		virtual bool ReadDoubleArray(const HLString &key, std::vector<double> &result) override;
		virtual bool ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result) override;
		virtual bool ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result) override;
		virtual bool ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result) override;
		virtual bool ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result) override;
		virtual bool ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result) override;
		virtual bool ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result) override;
		virtual bool ReadQuatArray(const HLString &key, std::vector<glm::quat> &result) override;

		virtual bool ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result) override;
		virtual bool ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result) override;
		virtual bool ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result) override;
		virtual bool Readint64ArrayMap(const HLString &key, std::map<HLString, int64> &result) override;
		virtual bool ReadUint64ArrayMap(const HLString &key, std::map<HLString, uint64> &result) override;
		virtual bool ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result) override;
		virtual bool ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result) override;
		virtual bool ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result) override;
		virtual bool ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result) override;
		virtual bool ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result) override;
		virtual bool ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result) override;
		virtual bool ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result) override;
		virtual bool ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result) override;
		virtual bool ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result) override;
		virtual bool ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result) override;

		virtual bool ReadVector2(const HLString &key, glm::vec2 *result) override;
		virtual bool ReadVector3(const HLString &key, glm::vec3 *result) override;
		virtual bool ReadVector4(const HLString &key, glm::vec4 *result) override;
		virtual bool ReadMatrix2(const HLString &key, glm::mat2 *result) override;
		virtual bool ReadMatrix3(const HLString &key, glm::mat3 *result) override;
		virtual bool ReadMatrix4(const HLString &key, glm::mat4 *result) override;
		virtual bool ReadQuaternion(const HLString &key, glm::quat *result) override;

		virtual bool HasKey(const HLString &key) const override;
		virtual bool WriteOut() const override;
		virtual bool ReadContents(const FileSystemPath &filePath = "") override;
		virtual HLString GetContent() const override;

	private:

		FileSystemPath m_FilePath;
		YAML::Node m_Document;
	};
}

