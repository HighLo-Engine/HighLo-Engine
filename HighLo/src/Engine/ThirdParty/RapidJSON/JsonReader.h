// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-03) initial release
//

#pragma once

#include "Engine/Loaders/DocumentReader.h"
#include "Engine/Core/DataTypes/Optional.h"

#include <rapidjson/document.h>

namespace highlo
{
	class JsonReader : public DocumentReader
	{
	public:

		JsonReader(const FileSystemPath &filePath);
		virtual ~JsonReader();

		virtual bool ReadFloat(const HLString &key, float *result) override;
		virtual bool ReadDouble(const HLString &key, double *result) override;
		virtual bool ReadInt32(const HLString &key, int32 *result) override;
		virtual bool ReadUInt32(const HLString &key, uint32 *result) override;
		virtual bool ReadInt64(const HLString &key, int64 *result) override;
		virtual bool ReadUInt64(const HLString &key, uint64 *result) override;
		virtual bool ReadBool(const HLString &key, bool *result) override;
		virtual bool ReadString(const HLString &key, HLString *result) override;

		virtual bool ReadVector2(const HLString &key, glm::vec2 *result) override;
		virtual bool ReadVector3(const HLString &key, glm::vec3 *result) override;
		virtual bool ReadVector4(const HLString &key, glm::vec4 *result) override;
		virtual bool ReadMatrix2(const HLString &key, glm::mat2 *result) override;
		virtual bool ReadMatrix3(const HLString &key, glm::mat3 *result) override;
		virtual bool ReadMatrix4(const HLString &key, glm::mat4 *result) override;
		virtual bool ReadQuaternion(const HLString &key, glm::quat *result) override;

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
		virtual bool ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result) override;
		virtual bool ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result) override;
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

		virtual bool ReadStringArrayMap(const HLString &key, std::unordered_map<HLString, HLString> &result) override;
		virtual bool ReadInt32ArrayMap(const HLString &key, std::unordered_map<HLString, int32> &result) override;
		virtual bool ReadUInt32ArrayMap(const HLString &key, std::unordered_map<HLString, uint32> &result) override;
		virtual bool ReadInt64ArrayMap(const HLString &key, std::unordered_map<HLString, int64> &result) override;
		virtual bool ReadUInt64ArrayMap(const HLString &key, std::unordered_map<HLString, uint64> &result) override;
		virtual bool ReadBoolArrayMap(const HLString &key, std::unordered_map<HLString, bool> &result) override;
		virtual bool ReadFloatArrayMap(const HLString &key, std::unordered_map<HLString, float> &result) override;
		virtual bool ReadDoubleArrayMap(const HLString &key, std::unordered_map<HLString, double> &result) override;
		virtual bool ReadVec2ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec2> &result) override;
		virtual bool ReadVec3ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec3> &result) override;
		virtual bool ReadVec4ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec4> &result) override;
		virtual bool ReadMat2ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat2> &result) override;
		virtual bool ReadMat3ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat3> &result) override;
		virtual bool ReadMat4ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat4> &result) override;
		virtual bool ReadQuatArrayMap(const HLString &key, std::unordered_map<HLString, glm::quat> &result) override;

		virtual bool ReadContents(const FileSystemPath &filePath = "") override;
		virtual HLString GetContent(bool prettify = false) override;
		virtual void SetContent(const HLString &content) override;

	private:

		bool Read(const HLString &key, DocumentDataType type, const std::function<bool(rapidjson::Value&)> &insertFunc);
		bool ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(const rapidjson::Value&)> &insertFunc);
		bool ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(const HLString&, const rapidjson::Value&)> &insertFunc);

		void ParseJSONArray(const rapidjson::GenericArray<false, rapidjson::Value> &arrayElements, const std::function<void(const rapidjson::Value&)> &elementFunc);
		Optional<rapidjson::GenericArray<false, rapidjson::Value>> GetOrFindArrayFormat(rapidjson::Document &doc, const HLString &key);

	private:

		rapidjson::Document m_Document;
		FileSystemPath m_FilePath;
	};
}

