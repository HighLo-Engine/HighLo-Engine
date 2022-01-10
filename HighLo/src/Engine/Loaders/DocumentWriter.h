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
	/// Represents a single Node
	/// </summary>
	struct DocumentEntry
	{
		HLString Key;
		HLString Type;

		void **Value;
		uint32 ElementSize = 0;
		uint32 ElementCount = 0;
	};

	/// <summary>
	/// This class is used as an interface to write/read JSON, XML and YAML files
	/// </summary>
	class DocumentWriter : public IsSharedReference
	{
	public:

		HLAPI virtual ~DocumentWriter() {}

		HLAPI virtual bool Write(const HLString &key, float value) = 0;
		HLAPI virtual bool Write(const HLString &key, double value) = 0;
		HLAPI virtual bool Write(const HLString &key, int32 value) = 0;
		HLAPI virtual bool Write(const HLString &key, uint32 value) = 0;
		HLAPI virtual bool Write(const HLString &key, int64 value) = 0;
		HLAPI virtual bool Write(const HLString &key, uint64 value) = 0;
		HLAPI virtual bool Write(const HLString &key, bool value) = 0;
		HLAPI virtual bool Write(const HLString &key, const HLString &value) = 0;

		HLAPI virtual bool Write(const HLString &key, std::vector<DocumentEntry> &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::vec2 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::vec3 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::vec4 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::mat2 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::mat3 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::mat4 &value) = 0;
		HLAPI virtual bool Write(const HLString &key, const glm::quat &value) = 0;

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

		HLAPI virtual std::vector<DocumentEntry> GetAll() = 0;

		HLAPI virtual bool HasKey(const HLString &key) const = 0;
		HLAPI virtual bool WriteOut() const = 0;
		HLAPI virtual HLString GetContent() const = 0;

		HLAPI static Ref<DocumentWriter> Create(const FileSystemPath &filePath, DocumentType type = DocumentType::None);
	};
}

