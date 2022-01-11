// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "JsonWriter.h"

#include "Engine/Core/FileSystem.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#undef GetObject
#define JSON_LOG_PREFIX "JSONWriter>   "

namespace highlo
{
	JSONWriter::JSONWriter(const FileSystemPath &filePath)
	{
		m_FilePath = filePath;
	}
	
	JSONWriter::~JSONWriter()
	{
	}

	void JSONWriter::BeginArray()
	{
		m_ShouldWriteIntoArray = true;
	}

	void JSONWriter::EndArray(const HLString &key)
	{
		if (m_ShouldWriteIntoArray)
		{
			m_ShouldWriteIntoArray = false;

			if (m_ObjectWasUsed)
			{
				m_ObjectWasUsed = false;

				m_Document.SetObject();
				rapidjson::Value array(rapidjson::kArrayType);

				for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
				{
					rapidjson::Value v;
					v.SetObject();
					v.AddMember(m_TempBuffers[i].first, m_TempBuffers[i].second, m_Document.GetAllocator());
					array.PushBack(v, m_Document.GetAllocator());
				}

				rapidjson::Value arrayKeyName(rapidjson::kStringType);
				arrayKeyName.SetString(*key, m_Document.GetAllocator());

				m_Document.AddMember(arrayKeyName, array, m_Document.GetAllocator());
			}
			else
			{
				m_Document.SetArray();
				for (uint32 i = 0; i < m_TempBuffers.size(); ++i)
				{
					m_Document.PushBack(m_TempBuffers[i].second, m_Document.GetAllocator());
				}
			}
		}
	}

	void JSONWriter::BeginObject()
	{
		m_ShouldWriteIntoObject = true;
		m_ObjectWasUsed = true;
	}

	void JSONWriter::EndObject()
	{
		if (m_ShouldWriteIntoObject)
		{
			m_ShouldWriteIntoObject = false;

			if (m_ShouldWriteIntoArray)
			{
				std::pair<rapidjson::Value, rapidjson::Value> &current = m_TempBuffers.emplace_back();
				current.first = m_TempBuffer.first;
				current.second = m_TempBuffer.second;
			}
			else
			{
				m_Document.SetObject();
				m_Document.AddMember(m_TempBuffer.first, m_TempBuffer.second, m_Document.GetAllocator());
			}
		}
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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<HLString> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value str(rapidjson::kStringType);
			str.SetString(*value[i], value[i].Length(), m_Document.GetAllocator());
			valType.PushBack(str, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<int32> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType = ConvertStdArrToRapidJsonArr(value);

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<uint32> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType = ConvertStdArrToRapidJsonArr(value);

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<int64> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType = ConvertStdArrToRapidJsonArr(value);

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<uint64> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType = ConvertStdArrToRapidJsonArr(value);

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<bool> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType = ConvertStdArrToRapidJsonArr(value);

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<float> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType = ConvertStdArrToRapidJsonArr(value);

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, std::vector<double> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType = ConvertStdArrToRapidJsonArr(value);

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}
	
	bool JSONWriter::Write(const HLString &key, std::vector<glm::vec2> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[i].x, m_Document.GetAllocator());
			v.PushBack<float>(value[i].y, m_Document.GetAllocator());
			valType.PushBack(v, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<glm::vec3> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[i].x, m_Document.GetAllocator());
			v.PushBack<float>(value[i].y, m_Document.GetAllocator());
			v.PushBack<float>(value[i].z, m_Document.GetAllocator());
			valType.PushBack(v, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<glm::vec4> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[i].x, m_Document.GetAllocator());
			v.PushBack<float>(value[i].y, m_Document.GetAllocator());
			v.PushBack<float>(value[i].z, m_Document.GetAllocator());
			v.PushBack<float>(value[i].w, m_Document.GetAllocator());
			valType.PushBack(v, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<glm::mat2> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[i][0][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][0][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][1], m_Document.GetAllocator());
			valType.PushBack(v, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<glm::mat3> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[i][0][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][0][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][0][2], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][2], m_Document.GetAllocator());
			v.PushBack<float>(value[i][2][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][2][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][2][2], m_Document.GetAllocator());
			valType.PushBack(v, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<glm::mat4> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[i][0][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][0][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][0][2], m_Document.GetAllocator());
			v.PushBack<float>(value[i][0][3], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][2], m_Document.GetAllocator());
			v.PushBack<float>(value[i][1][3], m_Document.GetAllocator());
			v.PushBack<float>(value[i][2][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][2][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][2][2], m_Document.GetAllocator());
			v.PushBack<float>(value[i][2][3], m_Document.GetAllocator());
			v.PushBack<float>(value[i][3][0], m_Document.GetAllocator());
			v.PushBack<float>(value[i][3][1], m_Document.GetAllocator());
			v.PushBack<float>(value[i][3][2], m_Document.GetAllocator());
			v.PushBack<float>(value[i][3][3], m_Document.GetAllocator());
			valType.PushBack(v, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Write(const HLString &key, std::vector<glm::quat> &value)
	{
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value valType(rapidjson::kArrayType);

		for (uint32 i = 0; i < value.size(); ++i)
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[i].w, m_Document.GetAllocator());
			v.PushBack<float>(value[i].x, m_Document.GetAllocator());
			v.PushBack<float>(value[i].y, m_Document.GetAllocator());
			v.PushBack<float>(value[i].z, m_Document.GetAllocator());
			valType.PushBack(v, m_Document.GetAllocator());
		}

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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
		
		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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
		
		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}
		
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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

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

		if (!AddIntoStructure(keyType, valType))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, valType, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::ReadStringArray(const HLString &key, std::vector<HLString> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			HL_ASSERT(m_Document.IsObject());
			rapidjson::GenericObject obj = m_Document.GetObject();

			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsString())
				{
					HLString k = currentObj->name.GetString();
					HLString v = currentObj->value.GetString();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, *v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-string!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsString())
			{
				HLString k = currentObj->name.GetString();
				HLString v = currentObj->value.GetString();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, *v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-string!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadInt32Array(const HLString &key, std::vector<int32> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();

			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsInt())
				{
					HLString k = currentObj->name.GetString();
					int32 v = currentObj->value.IsInt();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-int32!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsInt())
			{
				HLString k = currentObj->name.GetString();
				int32 v = currentObj->value.GetInt();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-int32!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsUint())
				{
					HLString k = currentObj->name.GetString();
					uint32 v = currentObj->value.GetUint();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-uint32!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsUint())
			{
				HLString k = currentObj->name.GetString();
				uint32 v = currentObj->value.GetUint();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-uint32!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadInt64Array(const HLString &key, std::vector<int64> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsInt64())
				{
					HLString k = currentObj->name.GetString();
					int64 v = currentObj->value.GetInt64();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-int64!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsInt64())
			{
				HLString k = currentObj->name.GetString();
				int64 v = currentObj->value.GetInt64();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-int64!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsUint64())
				{
					HLString k = currentObj->name.GetString();
					uint64 v = currentObj->value.GetUint64();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-uint64!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsUint64())
			{
				HLString k = currentObj->name.GetString();
				uint64 v = currentObj->value.GetUint64();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-uint64!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadBoolArray(const HLString &key, std::vector<bool> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsBool())
				{
					HLString k = currentObj->name.GetString();
					bool v = currentObj->value.GetBool();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-bool!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsBool())
			{
				HLString k = currentObj->name.GetString();
				bool v = currentObj->value.GetBool();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-bool!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadFloatArray(const HLString &key, std::vector<float> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsFloat())
				{
					HLString k = currentObj->name.GetString();
					float v = currentObj->value.GetFloat();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-float!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsFloat())
			{
				HLString k = currentObj->name.GetString();
				float v = currentObj->value.GetFloat();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-float!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadDoubleArray(const HLString &key, std::vector<double> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsDouble())
				{
					HLString k = currentObj->name.GetString();
					double v = currentObj->value.GetDouble();
					value.push_back(v);
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-double!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsDouble())
			{
				HLString k = currentObj->name.GetString();
				double v = currentObj->value.GetDouble();
				value.push_back(v);
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-double!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();
					glm::vec2 v;
					v.x = values[0].GetFloat();
					v.y = values[1].GetFloat();
					value.push_back(v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-vec2!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();
				glm::vec2 v;
				v.x = values[0].GetFloat();
				v.y = values[1].GetFloat();
				value.push_back(v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-vec2!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();
					glm::vec3 v;
					v.x = values[0].GetFloat();
					v.y = values[1].GetFloat();
					v.z = values[2].GetFloat();
					value.push_back(v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-vec3!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();
				glm::vec3 v;
				v.x = values[0].GetFloat();
				v.y = values[1].GetFloat();
				v.z = values[2].GetFloat();
				value.push_back(v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-vec3!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();
					glm::vec4 v;
					v.x = values[0].GetFloat();
					v.y = values[1].GetFloat();
					v.z = values[2].GetFloat();
					v.w = values[3].GetFloat();
					value.push_back(v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-vec4!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();
				glm::vec4 v;
				v.x = values[0].GetFloat();
				v.y = values[1].GetFloat();
				v.z = values[2].GetFloat();
				v.w = values[3].GetFloat();
				value.push_back(v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-vec4!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();
					glm::mat2 m;
					m[0][0] = values[0].GetFloat();
					m[0][1] = values[1].GetFloat();
					m[1][0] = values[2].GetFloat();
					m[1][1] = values[3].GetFloat();
					value.push_back(m);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-mat2!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();
				glm::mat2 m;
				m[0][0] = values[0].GetFloat();
				m[0][1] = values[1].GetFloat();
				m[1][0] = values[3].GetFloat();
				m[1][1] = values[4].GetFloat();
				value.push_back(m);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-mat2!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::mat3 m;
					m[0][0] = values[0].GetFloat();
					m[0][1] = values[1].GetFloat();
					m[0][2] = values[2].GetFloat();

					m[1][0] = values[3].GetFloat();
					m[1][1] = values[4].GetFloat();
					m[1][2] = values[5].GetFloat();

					m[2][0] = values[6].GetFloat();
					m[2][1] = values[7].GetFloat();
					m[2][2] = values[8].GetFloat();
					value.push_back(m);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-mat3!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::mat3 m;
				m[0][0] = values[0].GetFloat();
				m[0][1] = values[1].GetFloat();
				m[0][2] = values[2].GetFloat();

				m[1][0] = values[3].GetFloat();
				m[1][1] = values[4].GetFloat();
				m[1][2] = values[5].GetFloat();

				m[2][0] = values[6].GetFloat();
				m[2][1] = values[7].GetFloat();
				m[2][2] = values[8].GetFloat();
				value.push_back(m);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-mat3!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::mat4 m;
					m[0][0] = values[0].GetFloat();
					m[0][1] = values[1].GetFloat();
					m[0][2] = values[2].GetFloat();
					m[0][3] = values[3].GetFloat();

					m[1][0] = values[4].GetFloat();
					m[1][1] = values[5].GetFloat();
					m[1][2] = values[6].GetFloat();
					m[1][3] = values[7].GetFloat();

					m[2][0] = values[8].GetFloat();
					m[2][1] = values[9].GetFloat();
					m[2][2] = values[10].GetFloat();
					m[2][3] = values[11].GetFloat();

					m[3][0] = values[12].GetFloat();
					m[3][1] = values[13].GetFloat();
					m[3][2] = values[14].GetFloat();
					m[3][3] = values[15].GetFloat();
					value.push_back(m);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-mat4!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::mat4 m;
				m[0][0] = values[0].GetFloat();
				m[0][1] = values[1].GetFloat();
				m[0][2] = values[2].GetFloat();
				m[0][3] = values[3].GetFloat();

				m[1][0] = values[4].GetFloat();
				m[1][1] = values[5].GetFloat();
				m[1][2] = values[6].GetFloat();
				m[1][3] = values[7].GetFloat();

				m[2][0] = values[8].GetFloat();
				m[2][1] = values[9].GetFloat();
				m[2][2] = values[10].GetFloat();
				m[2][3] = values[11].GetFloat();

				m[3][0] = values[12].GetFloat();
				m[3][1] = values[13].GetFloat();
				m[3][2] = values[14].GetFloat();
				m[3][3] = values[15].GetFloat();
				value.push_back(m);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-mat4!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadQuatArray(const HLString &key, std::vector<glm::quat> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();
					glm::quat q;
					q.w = values[0].GetFloat();
					q.x = values[1].GetFloat();
					q.y = values[2].GetFloat();
					q.z = values[3].GetFloat();
					value.push_back(q);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-quat!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();
				glm::quat q;
				q.w = values[0].GetFloat();
				q.x = values[1].GetFloat();
				q.y = values[2].GetFloat();
				q.z = values[3].GetFloat();
				value.push_back(q);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-quat!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsString())
				{
					HLString k = currentObj->name.GetString();
					HLString v = currentObj->value.GetString();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-string!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsString())
			{
				HLString k = currentObj->name.GetString();
				HLString v = currentObj->value.GetString();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-string!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsInt())
				{
					HLString k = currentObj->name.GetString();
					int32 v = currentObj->value.GetInt();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-int32!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsInt())
			{
				HLString k = currentObj->name.GetString();
				int32 v = currentObj->value.GetInt();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-int32!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsUint())
				{
					HLString k = currentObj->name.GetString();
					uint32 v = currentObj->value.GetUint();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-uint32!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsUint())
			{
				HLString k = currentObj->name.GetString();
				uint32 v = currentObj->value.GetUint();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-uint32!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::Readint64ArrayMap(const HLString &key, std::map<HLString, int64> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsInt64())
				{
					HLString k = currentObj->name.GetString();
					int64 v = currentObj->value.GetInt64();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-in64!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsInt64())
			{
				HLString k = currentObj->name.GetString();
				int64 v = currentObj->value.GetInt64();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-int64!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadUint64ArrayMap(const HLString &key, std::map<HLString, uint64> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsUint64())
				{
					HLString k = currentObj->name.GetString();
					uint64 v = currentObj->value.GetUint64();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-uint64!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsUint64())
			{
				HLString k = currentObj->name.GetString();
				uint64 v = currentObj->value.GetUint64();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-uint64!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsBool())
				{
					HLString k = currentObj->name.GetString();
					bool v = currentObj->value.GetBool();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-bool!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsBool())
			{
				HLString k = currentObj->name.GetString();
				bool v = currentObj->value.GetBool();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-bool!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsFloat())
				{
					HLString k = currentObj->name.GetString();
					float v = currentObj->value.GetFloat();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-float!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsFloat())
			{
				HLString k = currentObj->name.GetString();
				float v = currentObj->value.GetFloat();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-float!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsDouble())
				{
					HLString k = currentObj->name.GetString();
					double v = currentObj->value.GetDouble();

					value.insert({ k, v });
					HL_CORE_TRACE("{0}={1}", *k, v);
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-double!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsDouble())
			{
				HLString k = currentObj->name.GetString();
				double v = currentObj->value.GetDouble();

				value.insert({ k, v });
				HL_CORE_TRACE("{0}={1}", *k, v);
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-double!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &value)
	{
		if (!m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::vec2 v;
					v.x = values[0].GetFloat();
					v.y = values[1].GetFloat();
					value.insert({ k, v });
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-vec2!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::vec2 v;
				v.x = values[0].GetFloat();
				v.y = values[1].GetFloat();
				value.insert({ k, v });
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-vec2!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &value)
	{
		if (!m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::vec3 v;
					v.x = values[0].GetFloat();
					v.y = values[1].GetFloat();
					v.z = values[2].GetFloat();
					value.insert({ k, v });
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-vec3!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::vec3 v;
				v.x = values[0].GetFloat();
				v.y = values[1].GetFloat();
				v.z = values[2].GetFloat();
				value.insert({ k, v });
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-vec3!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &value)
	{
		if (m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::vec4 v;
					v.x = values[0].GetFloat();
					v.y = values[1].GetFloat();
					v.z = values[2].GetFloat();
					v.w = values[3].GetFloat();
					value.insert({ k, v });
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-vec4!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::vec4 v;
				v.x = values[0].GetFloat();
				v.y = values[1].GetFloat();
				v.z = values[2].GetFloat();
				v.w = values[3].GetFloat();
				value.insert({ k, v });
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-vec4");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &value)
	{
		if (!m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::mat2 v;
					v[0][0] = values[0].GetFloat();
					v[0][1] = values[1].GetFloat();
					v[1][0] = values[2].GetFloat();
					v[1][1] = values[3].GetFloat();
					value.insert({ k, v });
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-mat2!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::mat2 v;
				v[0][0] = values[0].GetFloat();
				v[0][1] = values[1].GetFloat();
				v[1][0] = values[2].GetFloat();
				v[1][1] = values[3].GetFloat();
				value.insert({ k, v });
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-mat2!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &value)
	{
		if (!m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::mat3 v;
					v[0][0] = values[0].GetFloat();
					v[0][1] = values[1].GetFloat();
					v[0][2] = values[2].GetFloat();

					v[1][0] = values[3].GetFloat();
					v[1][1] = values[4].GetFloat();
					v[1][2] = values[5].GetFloat();

					v[2][0] = values[6].GetFloat();
					v[2][1] = values[7].GetFloat();
					v[2][2] = values[8].GetFloat();
					value.insert({ k, v });
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-mat3!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::mat3 v;
				v[0][0] = values[0].GetFloat();
				v[0][1] = values[1].GetFloat();
				v[0][2] = values[2].GetFloat();

				v[1][0] = values[3].GetFloat();
				v[1][1] = values[4].GetFloat();
				v[1][2] = values[5].GetFloat();

				v[2][0] = values[6].GetFloat();
				v[2][1] = values[7].GetFloat();
				v[2][2] = values[8].GetFloat();
				value.insert({ k, v });
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-mat3!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &value)
	{
		if (!m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::mat4 v;
					v[0][0] = values[0].GetFloat();
					v[0][1] = values[1].GetFloat();
					v[0][2] = values[2].GetFloat();
					v[0][3] = values[3].GetFloat();

					v[1][0] = values[4].GetFloat();
					v[1][1] = values[5].GetFloat();
					v[1][2] = values[6].GetFloat();
					v[1][3] = values[7].GetFloat();

					v[2][0] = values[8].GetFloat();
					v[2][1] = values[9].GetFloat();
					v[2][2] = values[10].GetFloat();
					v[2][3] = values[11].GetFloat();

					v[3][0] = values[12].GetFloat();
					v[3][1] = values[13].GetFloat();
					v[3][2] = values[14].GetFloat();
					v[3][3] = values[15].GetFloat();
					value.insert({ k, v });
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-mat4!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::mat4 v;
				v[0][0] = values[0].GetFloat();
				v[0][1] = values[1].GetFloat();
				v[0][2] = values[2].GetFloat();
				v[0][3] = values[3].GetFloat();

				v[1][0] = values[4].GetFloat();
				v[1][1] = values[5].GetFloat();
				v[1][2] = values[6].GetFloat();
				v[1][3] = values[7].GetFloat();

				v[2][0] = values[8].GetFloat();
				v[2][1] = values[9].GetFloat();
				v[2][2] = values[10].GetFloat();
				v[2][3] = values[11].GetFloat();

				v[3][0] = values[12].GetFloat();
				v[3][1] = values[13].GetFloat();
				v[3][2] = values[14].GetFloat();
				v[3][3] = values[15].GetFloat();
				value.insert({ k, v });
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-mat4!");
				return false;
			}
		}

		return true;
	}

	bool JSONWriter::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &value)
	{
		if (!m_Document.IsNull())
			return false;

		if (!m_Document.IsArray())
		{
			rapidjson::GenericObject obj = m_Document.GetObject();
			rapidjson::GenericMemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (!it->value.IsArray())
				return false;

			rapidjson::GenericArray arr = it->value.GetArray();
			for (auto arrIt = arr.Begin(); arrIt != arr.End(); ++arrIt)
			{
				rapidjson::GenericMemberIterator currentObj = arrIt->GetObject().begin();
				if (currentObj->value.IsArray())
				{
					HLString k = currentObj->name.GetString();
					rapidjson::GenericArray values = currentObj->value.GetArray();

					glm::quat v;
					v.w = values[0].GetFloat();
					v.x = values[1].GetFloat();
					v.y = values[2].GetFloat();
					v.z = values[3].GetFloat();
					value.insert({ k, v });
				}
				else
				{
					HL_CORE_ERROR("Error: Parsed a non-quat!");
					return false;
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericMemberIterator currentObj = it->GetObject().begin();
			if (currentObj->value.IsArray())
			{
				HLString k = currentObj->name.GetString();
				rapidjson::GenericArray values = currentObj->value.GetArray();

				glm::quat v;
				v.w = values[0].GetFloat();
				v.x = values[1].GetFloat();
				v.y = values[2].GetFloat();
				v.z = values[3].GetFloat();
				value.insert({ k, v });
			}
			else
			{
				HL_CORE_ERROR("Error: Parsed a non-quat!");
				return false;
			}
		}

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
			glm::mat2 m;
			m[0][0] = arr[0].GetFloat();
			m[0][1] = arr[1].GetFloat();
			m[1][0] = arr[2].GetFloat();
			m[1][1] = arr[3].GetFloat();
			*value = m;
			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadMatrix3(const HLString &key, glm::mat3 *value)
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
			glm::mat3 m;
			m[0][0] = arr[0].GetFloat();
			m[0][1] = arr[1].GetFloat();
			m[0][2] = arr[2].GetFloat();

			m[1][0] = arr[3].GetFloat();
			m[1][1] = arr[4].GetFloat();
			m[1][2] = arr[5].GetFloat();

			m[2][0] = arr[6].GetFloat();
			m[2][1] = arr[7].GetFloat();
			m[2][2] = arr[8].GetFloat();
			*value = m;
			return true;
		}

		return false;
	}
	
	bool JSONWriter::ReadMatrix4(const HLString &key, glm::mat4 *value)
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
			glm::mat4 m;
			m[0][0] = arr[0].GetFloat();
			m[0][1] = arr[1].GetFloat();
			m[0][2] = arr[2].GetFloat();
			m[0][3] = arr[3].GetFloat();

			m[1][0] = arr[4].GetFloat();
			m[1][1] = arr[5].GetFloat();
			m[1][2] = arr[6].GetFloat();
			m[1][3] = arr[7].GetFloat();

			m[2][0] = arr[8].GetFloat();
			m[2][1] = arr[9].GetFloat();
			m[2][2] = arr[10].GetFloat();
			m[2][3] = arr[11].GetFloat();

			m[3][0] = arr[12].GetFloat();
			m[3][1] = arr[13].GetFloat();
			m[3][2] = arr[14].GetFloat();
			m[3][3] = arr[15].GetFloat();
			*value = m;
			return true;
		}

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
		HL_CORE_INFO(JSON_LOG_PREFIX "[+] Writing file {0} [+]", **m_FilePath);
		HLString content = GetContent();

		FILE *file = fopen(**m_FilePath, "w");
		if (!file)
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Could not open file {0} [-]", **m_FilePath);
			return false;
		}

		fwrite(*content, sizeof(char), content.Length(), file);
		fclose(file);
		return true;
	}

	bool JSONWriter::ReadContents(const FileSystemPath &filePath)
	{
		if (!filePath.String().IsEmpty())
			m_FilePath = filePath;

		if (FileSystem::Get()->FileExists(m_FilePath))
		{
			HL_CORE_INFO(JSON_LOG_PREFIX "[+] Loaded {0} [+]", **m_FilePath);
			HLString content = FileSystem::Get()->ReadTextFile(m_FilePath);
			if (content.IsEmpty())
				return false;

			m_Document.Parse(*content);
			return true;
		}
		else
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: File {0} not found! [-]", **m_FilePath);
		}

		return false;
	}
	
	HLString JSONWriter::GetContent() const
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		m_Document.Accept(writer);
		return buffer.GetString();
	}

	bool JSONWriter::AddIntoStructure(rapidjson::Value &keyType, rapidjson::Value &valType)
	{
		bool result = false;

		if (m_ShouldWriteIntoArray || m_ShouldWriteIntoObject)
		{
			result = true;
			m_TempBuffer.first = keyType;
			m_TempBuffer.second = valType;

			if (m_ShouldWriteIntoArray && !m_ShouldWriteIntoObject)
			{
				std::pair<rapidjson::Value, rapidjson::Value> &t = m_TempBuffers.emplace_back();
				t.first = keyType;
				t.second = valType;
			}
		}

		return result;
	}
}

