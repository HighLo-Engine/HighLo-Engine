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

		virtual std::vector<DocumentEntry> GetAll() override;

		virtual bool HasKey(const HLString &key) const override;
		virtual bool WriteOut() const override;
		virtual HLString GetContent() const override;

	private:

		FileSystemPath m_FilePath;
	};
}

