// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "JsonReader.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/LoaderUtils.h"
#include "JsonHelper.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#undef GetObject
#define JSON_LOG_PREFIX "JSONWriter>   "

namespace highlo
{
	JsonReader::JsonReader(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}

	JsonReader::~JsonReader()
	{
	}

	bool JsonReader::ReadFloat(const HLString &key, float *result)
	{
		return Read(key, DocumentDataType::Float, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsFloat())
			{
				*result = value.GetFloat();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadDouble(const HLString &key, double *result)
	{
		return Read(key, DocumentDataType::Double, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsDouble())
			{
				*result = value.GetDouble();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadInt32(const HLString &key, int32 *result)
	{
		return Read(key, DocumentDataType::Int32, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsInt())
			{
				*result = value.GetInt();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadUInt32(const HLString &key, uint32 *result)
	{
		return Read(key, DocumentDataType::UInt32, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsUint())
			{
				*result = value.GetUint();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadInt64(const HLString &key, int64 *result)
	{
		return Read(key, DocumentDataType::Int64, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsInt64())
			{
				*result = value.GetInt64();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadUInt64(const HLString &key, uint64 *result)
	{
		return Read(key, DocumentDataType::UInt64, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsUint64())
			{
				*result = value.GetUint64();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadBool(const HLString &key, bool *result)
	{
		return Read(key, DocumentDataType::Bool, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsBool())
			{
				*result = value.GetBool();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadString(const HLString &key, HLString *result)
	{
		return Read(key, DocumentDataType::String, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			if (value.IsString())
			{
				*result = value.GetString();
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return Read(key, DocumentDataType::Vec2, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			return utils::JSONToVec2(value, result);
		});
	}
	
	bool JsonReader::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return Read(key, DocumentDataType::Vec3, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			return utils::JSONToVec3(value, result);
		});
	}
	
	bool JsonReader::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return Read(key, DocumentDataType::Vec4, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			return utils::JSONToVec4(value, result);
		});
	}
	
	bool JsonReader::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return Read(key, DocumentDataType::Mat2, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			return utils::JSONToMat2(value, result);
		});
	}
	
	bool JsonReader::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return Read(key, DocumentDataType::Mat3, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			return utils::JSONToMat3(value, result);
		});
	}
	
	bool JsonReader::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return Read(key, DocumentDataType::Mat4, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			return utils::JSONToMat4(value, result);
		});
	}
	
	bool JsonReader::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return Read(key, DocumentDataType::Quat, [result](rapidjson::Value &value) -> bool
		{
			if (!result)
				return false;

			return utils::JSONToQuat(value, result);
		});
	}
	
	bool JsonReader::ReadStringArray(const HLString &key, std::vector<HLString> &result)
	{
		return ReadArray(key, DocumentDataType::String, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsString())
			{
				result.push_back(value.GetString());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return ReadArray(key, DocumentDataType::Int32, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsInt())
			{
				result.push_back(value.GetInt());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return ReadArray(key, DocumentDataType::UInt32, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsUint())
			{
				result.push_back(value.GetUint());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return ReadArray(key, DocumentDataType::Int64, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsInt64())
			{
				result.push_back(value.GetInt64());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return ReadArray(key, DocumentDataType::UInt64, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsUint64())
			{
				result.push_back(value.GetUint64());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return ReadArray(key, DocumentDataType::Bool, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsBool())
			{
				result.push_back(value.GetBool());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return ReadArray(key, DocumentDataType::Float, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsFloat())
			{
				result.push_back(value.GetFloat());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return ReadArray(key, DocumentDataType::Double, [&result](const rapidjson::Value &value) -> bool
		{
			if (value.IsDouble())
			{
				result.push_back(value.GetDouble());
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return ReadArray(key, DocumentDataType::Vec2, [&result](const rapidjson::Value &value) -> bool
		{
			glm::vec2 v;
			if (!utils::JSONToVec2(value, &v))
				return false;

			result.push_back(v);
			return true;
		});
	}
	
	bool JsonReader::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return ReadArray(key, DocumentDataType::Vec3, [&result](const rapidjson::Value &value) -> bool
		{
			glm::vec3 v;
			if (!utils::JSONToVec3(value, &v))
				return false;

			result.push_back(v);
			return true;
		});
	}
	
	bool JsonReader::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return ReadArray(key, DocumentDataType::Vec4, [&result](const rapidjson::Value &value) -> bool
		{
			glm::vec4 v;
			if (!utils::JSONToVec4(value, &v))
				return false;

			result.push_back(v);
			return true;
		});
	}
	
	bool JsonReader::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return ReadArray(key, DocumentDataType::Mat2, [&result](const rapidjson::Value &value) -> bool
		{
			glm::mat2 m;
			if (!utils::JSONToMat2(value, &m))
				return false;

			result.push_back(m);
			return true;
		});
	}
	
	bool JsonReader::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return ReadArray(key, DocumentDataType::Mat3, [&result](const rapidjson::Value &value) -> bool
		{
			glm::mat3 m;
			if (!utils::JSONToMat3(value, &m))
				return false;

			result.push_back(m);
			return true;
		});
	}
	
	bool JsonReader::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return ReadArray(key, DocumentDataType::Mat4, [&result](const rapidjson::Value &value) -> bool
		{
			glm::mat4 m;
			if (!utils::JSONToMat4(value, &m))
				return false;

			result.push_back(m);
			return true;
		});
	}
	
	bool JsonReader::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return ReadArray(key, DocumentDataType::Quat, [&result](const rapidjson::Value &value) -> bool
		{
			glm::quat q;
			if (!utils::JSONToQuat(value, &q))
				return false;

			result.push_back(q);
			return true;
		});
	}
	
	bool JsonReader::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return ReadArrayMap(key, DocumentDataType::String, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsString())
			{
				result.insert({ key, value.GetString() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int32, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsInt())
			{
				result.insert({ key, value.GetInt() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt32, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsUint())
			{
				result.insert({ key, value.GetUint() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int64, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsInt64())
			{
				result.insert({ key, value.GetInt64() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt64, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsUint64())
			{
				result.insert({ key, value.GetUint64() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Bool, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsBool())
			{
				result.insert({ key, value.GetBool() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Float, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsFloat())
			{
				result.insert({ key, value.GetFloat() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Double, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			if (value.IsDouble())
			{
				result.insert({ key, value.GetDouble() });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			glm::vec2 v;
			if (utils::JSONToVec2(value, &v))
			{
				result.insert({ key, v });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec3, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			glm::vec3 v;
			if (utils::JSONToVec3(value, &v))
			{
				result.insert({ key, v });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec4, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			glm::vec4 v;
			if (utils::JSONToVec4(value, &v))
			{
				result.insert({ key, v });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat2, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			glm::mat2 m;
			if (utils::JSONToMat2(value, &m))
			{
				result.insert({ key, m });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat3, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			glm::mat3 m;
			if (utils::JSONToMat3(value, &m))
			{
				result.insert({ key, m });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat4, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			glm::mat4 m;
			if (utils::JSONToMat4(value, &m))
			{
				result.insert({ key, m });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Quat, [&result](const HLString &key, const rapidjson::Value &value) -> bool
		{
			glm::quat q;
			if (utils::JSONToQuat(value, &q))
			{
				result.insert({ key, q });
				return true;
			}

			return false;
		});
	}
	
	bool JsonReader::ReadContents(const FileSystemPath &filePath)
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
	
	HLString JsonReader::GetContent(bool prettify)
	{
		rapidjson::StringBuffer buffer;

		if (prettify)
		{
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			writer.SetMaxDecimalPlaces(3);
			m_Document.Accept(writer);
		}
		else
		{
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			writer.SetMaxDecimalPlaces(3);
			m_Document.Accept(writer);
		}

		return buffer.GetString();
	}
	
	void JsonReader::SetContent(const HLString &content)
	{
		m_Document.Parse(*content);
	}
	
	bool JsonReader::Read(const HLString &key, DocumentDataType type, const std::function<bool(rapidjson::Value&)> &insertFunc)
	{
		if (m_Document.IsNull())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Document Root was null! [-]");
			return false;
		}

		if (!m_Document.IsObject())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Document is not a object! [-]");
			return false;
		}

		rapidjson::GenericObject obj = m_Document.GetObject();

		rapidjson::GenericMemberIterator typeIterator = obj.FindMember("type");
		if (typeIterator == obj.MemberEnd())
		{
			HL_ASSERT(obj.HasMember(*key));

			rapidjson::GenericMemberIterator valueIterator = obj.FindMember(*key);
			if (valueIterator == obj.MemberEnd())
			{
				HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Did not find your specified key! [-]");
				return false;
			}

			if (key == valueIterator->name.GetString())
				return insertFunc(valueIterator->value);

			return false;
		}

		HL_ASSERT(obj.HasMember("type"));
		HL_ASSERT(obj.HasMember("value"));

		HLString correctType = utils::DocumentDataTypeToString(type);
		HLString typeStr = typeIterator->value.GetString();
		if (correctType != typeStr)
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: The type did not match with the expected type! Expected {0}, but got {1} [-]", *correctType, *typeStr);
			return false;
		}

		rapidjson::GenericMemberIterator valueWrapperIterator = obj.FindMember("value");
		if (valueWrapperIterator == obj.MemberEnd())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Did not find the value in the value wrapper! [-]");
			return false;
		}

		rapidjson::GenericMemberIterator valueIterator = valueWrapperIterator->value.GetObject().MemberBegin();
		if (valueIterator == valueWrapperIterator->value.MemberEnd())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Did not find any value! [-]");
			return false;
		}

		if (key == valueIterator->name.GetString())
			return insertFunc(valueIterator->value);

		return false;
	}
	
	bool JsonReader::ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(const rapidjson::Value&)> &insertFunc)
	{
		if (m_Document.IsNull())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Document Root was null! [-]");
			return false;
		}

		if (!m_Document.IsArray())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Expected JSON array format, object format given. [-]");
			return false;
		}

		rapidjson::GenericArray elements = m_Document.GetArray();
		ParseJSONArray(key, elements, [=](const rapidjson::Value &arrayElement)
		{


			return true;
		});

		return true;
	}

	bool JsonReader::ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(const HLString&, const rapidjson::Value&)> &insertFunc)
	{
		if (m_Document.IsNull())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Document Root was null! [-]");
			return false;
		}

		if (!m_Document.IsArray())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Expected JSON array format, object format given. [-]");
			return false;
		}
		
		rapidjson::GenericArray elements = m_Document.GetArray();
		ParseJSONArray(key, elements, [=](const rapidjson::Value &arrayElement)
		{
			if (!arrayElement.IsObject())
			{
				HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Expected JSON object, array object given! [-]");
				return false;
			}

			rapidjson::GenericObject obj = arrayElement.GetObject();

			// First check, if we got a type declaration
			auto &typeIterator = obj.FindMember("type");
			if (typeIterator == obj.MemberEnd())
			{
				HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Parsing Error: Did not find type declaration! [-]");
				return false;
			}

			// then check the actual type
			HLString expectedType = utils::DocumentDataTypeToString(type);
			HLString actualType = (*typeIterator).value.GetString();
			if (expectedType != actualType)
			{
				HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: expected type {0}, but got {1} [-]", *expectedType, *actualType);
				return false;
			}

			// Try to find the value definition
			auto &valueIterator = obj.FindMember("value");
			if (valueIterator == obj.MemberEnd())
			{
				HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Parsing Error: Expected to find value declaration! [-]");
				return false;
			}

			// Now get the actual value
			rapidjson::GenericObject valueWrapper = valueIterator->value.GetObject();
			for (rapidjson::GenericMemberIterator currentValue = valueWrapper.begin(); currentValue != valueWrapper.end(); ++currentValue)
			{
				HLString k = currentValue->name.GetString();

				bool inserted = insertFunc(k, currentValue->value);
				if (!inserted)
				{
					HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Error: Type specific function failed! [-]");
					return false;
				}
			}

			return true;
		});

		return true;
	}
	
	void JsonReader::ParseJSONArray(const HLString &key, const rapidjson::GenericArray<false, rapidjson::Value> &arrayElements, const std::function<void(const rapidjson::Value&)> &elementFunc)
	{
		bool checkForKey = !key.IsEmpty();

		for (uint32 i = 0; i < arrayElements.Size(); ++i)
		{
			if (arrayElements[i].IsArray())
			{
				// recursively call the parseArray function, to retrieve the inner array
				ParseJSONArray(key, arrayElements[i].GetArray(), elementFunc);
			}
			else
			{
				if (checkForKey)
				{
					if (arrayElements[i].IsObject())
					{
						rapidjson::GenericObject obj = arrayElements[i].GetObject();
						for (rapidjson::GenericMemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
						{
							HLString currentKey = it->name.GetString();
							if (currentKey == key)
								elementFunc(arrayElements[i]);
						}
					}
				}
				else
				{
					elementFunc(arrayElements[i]);
				}
			}
		}
	}
}

