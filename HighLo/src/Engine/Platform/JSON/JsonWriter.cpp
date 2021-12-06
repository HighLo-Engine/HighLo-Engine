#include "HighLoPch.h"
#include "JsonWriter.h"

#include "Engine/Core/FileSystem.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#undef GetObject
#define JSON_LOG_PREFIX "JSONWriter>       "

namespace highlo
{
	JSONWriter::JSONWriter(const FileSystemPath &filePath)
	{
		m_FilePath = filePath;

		HLString content = FileSystem::Get()->ReadTextFile(filePath);
		m_Document.Parse(*content);
	}
	
	JSONWriter::~JSONWriter()
	{
	}
	
	bool JSONWriter::Write(const HLString &key, float value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for value {0} [-]", value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetFloat(value);

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, double value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for value {0} [-]", value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetDouble(value);

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, int32 value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for value {0} [-]", value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetInt(value);

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, uint32 value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for value {0} [-]", value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetUint(value);

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, int64 value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for value {0} [-]", value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetInt64(value);

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, uint64 value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for value {0} [-]", value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetUint64(value);

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}

	bool JSONWriter::Write(const HLString &key, bool value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for value {0} [-]", value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetBool(value);

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, const HLString &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for String {0} [-]", *value);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kObjectType);
		valType.SetString(*value, value.Length(), m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, std::vector<DocumentEntry> &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Array [-]");
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (auto &it = value.begin(); it != value.end(); ++it)
		{
			if (it->Type == "string")
			{
				void *val = it->Value[0];
				HLString str((char*)val, it->ElementCount);

				rapidjson::Value strVal;
				strVal.SetString(*str, str.Length(), m_Document.GetAllocator());

				valType.PushBack(strVal, m_Document.GetAllocator());
			}
			else if (it->Type == "uint64")
			{
				uint64 val = (uint64)it->Value[0];
				valType.PushBack<uint64>(val, m_Document.GetAllocator());
			}
			// TODO: Add other supported types
		}

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());

		// Cleanup 
		for (auto &it = value.begin(); it != value.end(); ++it)
		{
			delete[] it->Value;
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, const glm::vec2 &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Vector2 {0}, {1} [-]", value.x, value.y);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);
		valType.PushBack<float>(value.x, m_Document.GetAllocator());
		valType.PushBack<float>(value.y, m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, const glm::vec3 &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Vector3 {0}, {1}, {2} [-]", value.x, value.y, value.z);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);
		valType.PushBack<float>(value.x, m_Document.GetAllocator());
		valType.PushBack<float>(value.y, m_Document.GetAllocator());
		valType.PushBack<float>(value.z, m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, const glm::vec4 &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Vector4 {0}, {1}, {2}, {3} [-]", value.x, value.y, value.y, value.z);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);
		valType.PushBack<float>(value.x, m_Document.GetAllocator());
		valType.PushBack<float>(value.y, m_Document.GetAllocator());
		valType.PushBack<float>(value.z, m_Document.GetAllocator());
		valType.PushBack<float>(value.w, m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, const glm::mat2 &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Matrix2 [-]");
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);
		valType.PushBack<float>(value[0][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[0][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[1][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[1][1], m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, const glm::mat3 &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Matrix3 [-]");
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);
		valType.PushBack<float>(value[0][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[0][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[0][2], m_Document.GetAllocator());

		valType.PushBack<float>(value[1][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[1][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[1][2], m_Document.GetAllocator());

		valType.PushBack<float>(value[2][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[2][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[2][2], m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, const glm::mat4 &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Matrix4 [-]");
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);
		valType.PushBack<float>(value[0][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[0][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[0][2], m_Document.GetAllocator());
		valType.PushBack<float>(value[0][3], m_Document.GetAllocator());

		valType.PushBack<float>(value[1][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[1][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[1][2], m_Document.GetAllocator());
		valType.PushBack<float>(value[1][3], m_Document.GetAllocator());

		valType.PushBack<float>(value[2][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[2][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[2][2], m_Document.GetAllocator());
		valType.PushBack<float>(value[2][3], m_Document.GetAllocator());

		valType.PushBack<float>(value[3][0], m_Document.GetAllocator());
		valType.PushBack<float>(value[3][1], m_Document.GetAllocator());
		valType.PushBack<float>(value[3][2], m_Document.GetAllocator());
		valType.PushBack<float>(value[3][3], m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, const glm::quat &value)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key for Quaternion {0}, {1}, {2}, {3} [-]", value.w, value.x, value.y, value.z);
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);
		valType.PushBack<float>(value.w, m_Document.GetAllocator());
		valType.PushBack<float>(value.x, m_Document.GetAllocator());
		valType.PushBack<float>(value.y, m_Document.GetAllocator());
		valType.PushBack<float>(value.z, m_Document.GetAllocator());

		m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		return true;
	}
	
	bool JSONWriter::ReadFloat(const HLString &key, float *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsFloat())
		{
			*value = it->value.GetFloat();
			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadDouble(const HLString &key, double *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsDouble())
		{
			*value = it->value.GetDouble();
			return true;
		}

		return false;
	}

	bool JSONWriter::ReadInt32(const HLString &key, int32 *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsInt())
		{
			*value = it->value.GetInt();
			return true;
		}

		return false;
	}

	bool JSONWriter::ReadUInt32(const HLString &key, uint32 *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsUint())
		{
			*value = it->value.GetUint();
			return true;
		}

		return false;
	}

	bool JSONWriter::ReadInt64(const HLString &key, int64 *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsInt64())
		{
			*value = it->value.GetInt64();
			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadUInt64(const HLString &key, uint64 *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsUint64())
		{
			*value = it->value.GetUint64();
			return true;
		}

		return false;
	}

	bool JSONWriter::ReadBool(const HLString &key, bool *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsBool())
		{
			*value = it->value.GetBool();
			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadString(const HLString &key, HLString *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsString())
		{
			*value = it->value.GetString();
			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadVector2(const HLString &key, glm::vec2 *value)
		{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsArray())
		{
			rapidjson::GenericArray arr = it->value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat())
				return false;

			float x = arr[0].GetFloat();
			float y = arr[1].GetFloat();
			*value = { x, y };

			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadVector3(const HLString &key, glm::vec3 *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsArray())
		{
			rapidjson::GenericArray arr = it->value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat())
				return false;

			float x = arr[0].GetFloat();
			float y = arr[1].GetFloat();
			float z = arr[2].GetFloat();
			*value = { x, y, z };

			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadVector4(const HLString &key, glm::vec4 *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsArray())
		{
			rapidjson::GenericArray arr = it->value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat())
				return false;

			float x = arr[0].GetFloat();
			float y = arr[1].GetFloat();
			float z = arr[2].GetFloat();
			float w = arr[3].GetFloat();
			*value = { x, y, z, w };

			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadMatrix2(const HLString &key, glm::mat2 *value)
	{
		return false;
	}
	
	bool JSONWriter::ReadMatrix3(const HLString &key, glm::mat3 *value)
	{
		return false;
	}
	
	bool JSONWriter::ReadMatrix4(const HLString &key, glm::mat4 *value)
	{
		return false;
	}
	
	bool JSONWriter::ReadQuaternion(const HLString &key, glm::quat *value)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();
		HL_ASSERT(obj.HasMember(*key));

		rapidjson::Value::MemberIterator it = obj.FindMember(*key);
		if (it == obj.MemberEnd())
			return false;

		if (it->value.IsArray())
		{
			rapidjson::GenericArray arr = it->value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat())
				return false;

			float w = arr[0].GetFloat();
			float x = arr[1].GetFloat();
			float y = arr[2].GetFloat();
			float z = arr[3].GetFloat();
			*value = { w, x, y, z };

			return true;
		}

		return false;
	}
	
	bool JSONWriter::HasKey(const HLString &key) const
	{
		return m_Document.HasMember(*key);
	}
	
	bool JSONWriter::WriteOut() const
	{
		// Write content into file
		HLString content = GetContent();
		FILE *file = fopen(**m_FilePath, "w");
		if (!file)
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Could not open file {0} [-]", **m_FilePath);
			return false;
		}

		fwrite(*content, sizeof(char), strlen(content), file);
		fclose(file);
		return true;
	}
	
	HLString JSONWriter::GetContent() const
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		m_Document.Accept(writer);
		return buffer.GetString();
	}
	
	std::vector<DocumentEntry> JSONWriter::GetAll()
	{
		std::vector<DocumentEntry> result;
		if (!m_Document.IsObject())
			return result;

		rapidjson::Value obj = m_Document.GetObject();
		rapidjson::Value &root = obj;
		EnterJsonTree(root, result);

		return result;
	}
	
	void JSONWriter::EnterJsonTree(const rapidjson::Value &obj, std::vector<DocumentEntry> &entries, uint32 level)
	{
		if (obj.IsObject())
		{
			for (rapidjson::Value::ConstMemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
			{
				const rapidjson::Value &objName = obj[it->name.GetString()];
				
				DocumentEntry entry = ParseJsonToDocumentEntry(it);
				entries.push_back(entry);
				
				EnterJsonTree(objName, entries, level + 1);
			}
		}
	}
	
	DocumentEntry JSONWriter::ParseJsonToDocumentEntry(const rapidjson::Value::ConstMemberIterator &it)
	{
		DocumentEntry entry;
		entry.Key = it->name.GetString();

		if (it->value.IsUint64())
		{
			uint64 value = it->value.GetUint64();
			entry.Type = "uint64";
			entry.ElementCount = 1;
			entry.ElementSize = sizeof(uint64);
			entry.Value = new void*[1];
			entry.Value[0] = &value;
		}
		else if (it->value.IsInt64())
		{
			int64 value = it->value.GetInt64();
			entry.Type = "int64";
			entry.ElementCount = 1;
			entry.ElementSize = sizeof(int64);
			entry.Value = new void*[1];
			entry.Value[0] = &value;
		}
		else if (it->value.IsInt())
		{
			int32 value = it->value.GetInt();
			entry.Type = "int32";
			entry.ElementCount = 1;
			entry.ElementSize = sizeof(int32);
			entry.Value = new void*[1];
			entry.Value[0] = &value;
		}
		else if (it->value.IsUint())
		{
			uint32 value = it->value.GetUint();
			entry.Type = "uint32";
			entry.ElementCount = 1;
			entry.ElementSize = sizeof(uint32);
			entry.Value = new void*[1];
			entry.Value[0] = &value;
		}
		else if (it->value.IsFloat())
		{
			float value = it->value.GetFloat();
			entry.Type = "float";
			entry.ElementCount = 1;
			entry.ElementSize = sizeof(float);
			entry.Value = new void*[1];
			entry.Value[0] = &value;
		}
		else if (it->value.IsDouble())
		{
			double value = it->value.GetDouble();
			entry.Type = "double";
			entry.ElementCount = 1;
			entry.ElementSize = sizeof(double);
			entry.Value = new void*[1];
			entry.Value[0] = &value;
		}
		else if (it->value.IsBool())
		{
			bool value = it->value.GetBool();
			entry.Type = "bool";
			entry.ElementCount = 1;
			entry.ElementSize = sizeof(bool);
			entry.Value = new void*[1];
			entry.Value[0] = &value;
		}
		else if (it->value.IsNull())
		{
			entry.Type = "null";
			entry.ElementCount = 0;
			entry.ElementSize = 0;
			entry.Value = nullptr;
		}
		else if (it->value.IsString())
		{
			HLString value = it->value.GetString();
			entry.Type = "string";
			entry.ElementCount = value.Length();
			entry.ElementSize = sizeof(char);
			entry.Value = new void*[value.Length()];
			for (uint32 i = 0; i < value.Length(); ++i)
				entry.Value[i] = (void*)&value[i];
		}
		else if (it->value.IsArray())
		{
			for (rapidjson::SizeType i = 0; i < it->value.Size(); ++i)
			{
				if (it->value[i].IsString())
				{
					HLString value = it->value[i].GetString();
					entry.Type = "string";
					entry.ElementCount = value.Length();
					entry.ElementSize = sizeof(char);
					entry.Value = new void*[value.Length()];
					for (uint32 i = 0; i < value.Length(); ++i)
						entry.Value[i] = (void*)&value[i];
				}
				else if (it->value[i].IsInt64())
				{
					int64 value = it->value[i].GetInt64();
					entry.Type = "int64";
					entry.ElementCount = 1;
					entry.ElementSize = sizeof(int64);
					entry.Value = new void*[1];
					entry.Value[0] = &value;
				}
				else if (it->value[i].IsUint64())
				{
					uint64 value = it->value[i].GetInt64();
					entry.Type = "uint64";
					entry.ElementCount = 1;
					entry.ElementSize = sizeof(uint64);
					entry.Value = new void*[1];
					entry.Value[0] = &value;
				}
				else if (it->value[i].IsInt())
				{
					int32 value = it->value[i].GetInt();
					entry.Type = "int32";
					entry.ElementCount = 1;
					entry.ElementSize = sizeof(int32);
					entry.Value = new void*[1];
					entry.Value[0] = &value;
				}
				else if (it->value[i].IsUint())
				{
					uint32 value = it->value[i].GetUint();
					entry.Type = "uint32";
					entry.ElementCount = 1;
					entry.ElementSize = sizeof(uint32);
					entry.Value = new void*[1];
					entry.Value[0] = &value;
				}
				else if (it->value[i].IsFloat())
				{
					float value = it->value[i].GetFloat();
					entry.Type = "float";
					entry.ElementCount = 1;
					entry.ElementSize = sizeof(float);
					entry.Value = new void*[1];
					entry.Value[0] = &value;
				}
				else if (it->value[i].IsDouble())
				{
					double value = it->value[i].GetDouble();
					entry.Type = "double";
					entry.ElementCount = 1;
					entry.ElementSize = sizeof(double);
					entry.Value = new void*[1];
					entry.Value[0] = &value;
				}
				else if (it->value[i].IsBool())
				{
					bool value = it->value[i].GetBool();
					entry.Type = "bool";
					entry.ElementCount = 1;
					entry.ElementSize = sizeof(bool);
					entry.Value = new void*[1];
					entry.Value[0] = &value;
				}
				else if (it->value[i].IsNull())
				{
					entry.Type = "null";
					entry.ElementCount = 0;
					entry.ElementSize = 0;
					entry.Value = nullptr;
				}
				else
				{
					entry.Type = "unknown";
					entry.ElementCount = 0;
					entry.ElementSize = 0;
					entry.Value = nullptr;
				}
			}
		}
		else
		{
			entry.Type = "unknown";
			entry.ElementCount = 0;
			entry.ElementSize = 0;
			entry.Value = nullptr;
		}

		return entry;
	}
}

