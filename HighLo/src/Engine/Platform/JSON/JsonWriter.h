// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-10) initial release
//

#pragma once

#include "Engine/Loaders/DocumentWriter.h"

#include <rapidjson/document.h>

namespace highlo
{
	class JSONWriter : public DocumentWriter
	{
	public:

		JSONWriter(const FileSystemPath &filePath);
		virtual ~JSONWriter();

		virtual bool Write(const HLString &key, float value) override;
		virtual bool Write(const HLString &key, double value) override;
		virtual bool Write(const HLString &key, int32 value) override;
		virtual bool Write(const HLString &key, uint32 value) override;
		virtual bool Write(const HLString &key, int64 value) override;
		virtual bool Write(const HLString &key, uint64 value) override;
		virtual bool Write(const HLString &key, bool value) override;
		virtual bool Write(const HLString &key, const HLString &value) override;

		virtual bool Write(const HLString &key, std::vector<DocumentEntry> &value) override;
		virtual bool Write(const HLString &key, const glm::vec2 &value) override;
		virtual bool Write(const HLString &key, const glm::vec3 &value) override;
		virtual bool Write(const HLString &key, const glm::vec4 &value) override;
		virtual bool Write(const HLString &key, const glm::mat2 &value) override;
		virtual bool Write(const HLString &key, const glm::mat3 &value) override;
		virtual bool Write(const HLString &key, const glm::mat4 &value) override;
		virtual bool Write(const HLString &key, const glm::quat &value) override;

		virtual bool ReadFloat(const HLString &key, float *value) override;
		virtual bool ReadDouble(const HLString &key, double *value) override;
		virtual bool ReadInt32(const HLString &key, int32 *value) override;
		virtual bool ReadUInt32(const HLString &key, uint32 *value) override;
		virtual bool ReadInt64(const HLString &key, int64 *value) override;
		virtual bool ReadUInt64(const HLString &key, uint64 *value) override;
		virtual bool ReadBool(const HLString &key, bool *value) override;
		virtual bool ReadString(const HLString &key, HLString *value) override;

		virtual bool ReadVector2(const HLString &key, glm::vec2 *value) override;
		virtual bool ReadVector3(const HLString &key, glm::vec3 *value) override;
		virtual bool ReadVector4(const HLString &key, glm::vec4 *value) override;
		virtual bool ReadMatrix2(const HLString &key, glm::mat2 *value) override;
		virtual bool ReadMatrix3(const HLString &key, glm::mat3 *value) override;
		virtual bool ReadMatrix4(const HLString &key, glm::mat4 *value) override;
		virtual bool ReadQuaternion(const HLString &key, glm::quat *value) override;

		virtual bool HasKey(const HLString &key) const override;
		virtual bool WriteOut() const override;
		virtual HLString GetContent() const override;
		virtual std::vector<DocumentEntry> GetAll() override;

	private:

		void EnterJsonTree(const rapidjson::Value &obj, std::vector<DocumentEntry> &entries, uint32 level = 0);
		DocumentEntry ParseJsonToDocumentEntry(const rapidjson::Value::ConstMemberIterator &value);

		rapidjson::Document m_Document;
		FileSystemPath m_FilePath;
		char m_StreamData[0xFFFF];
	};
}

