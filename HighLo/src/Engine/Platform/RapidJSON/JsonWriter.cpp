// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "JsonWriter.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/LoaderUtils.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#undef GetObject
#define JSON_LOG_PREFIX "JSONWriter>   "

#ifdef HL_DEBUG
#define JSON_PRINT_DEBUG_OUTPUT 0
#endif // HL_DEBUG

namespace highlo
{
	namespace utils
	{
		std::pair<rapidjson::Value, rapidjson::Value> ConvertDocumentTypeToRenderableFormat(rapidjson::Document &document, DocumentDataType type)
		{
			std::pair<rapidjson::Value, rapidjson::Value> result;
			rapidjson::Value keyType(rapidjson::kStringType);
			keyType.SetString("type", document.GetAllocator());

			rapidjson::Value valType(rapidjson::kStringType);

			HLString typeStr = utils::DocumentDataTypeToString(type);
			valType.SetString(*typeStr, typeStr.Length(), document.GetAllocator());

			result.first = keyType;
			result.second = valType;
			return result;
		}
	}

	JSONWriter::JSONWriter(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
	}

	JSONWriter::~JSONWriter()
	{
	}

	void JSONWriter::BeginArray()
	{
		m_ShouldWriteIntoArray = true;
	}

	void JSONWriter::EndArray(const HLString &key, bool rawData)
	{
		if (m_ShouldWriteIntoArray)
		{
			m_ShouldWriteIntoArray = false;

			if (key.IsEmpty())
			{
				m_Document.SetArray();
				if (!rawData)
				{
					for (uint32 i = 0; i < m_TempBufferValues.size(); ++i)
					{
						rapidjson::Value realValue(rapidjson::kObjectType);
						realValue.AddMember(m_TempBufferValues[i].first, m_TempBufferValues[i].second, m_Document.GetAllocator());

						rapidjson::Value keyValue(rapidjson::kStringType);
						keyValue.SetString("value", m_Document.GetAllocator());

						rapidjson::Value v(rapidjson::kObjectType);
						v.AddMember(m_TempBufferTypes[i].first, m_TempBufferTypes[i].second, m_Document.GetAllocator());
						v.AddMember(keyValue, realValue, m_Document.GetAllocator());
						m_Document.PushBack(v, m_Document.GetAllocator());
					}
				}
				else
				{
					for (uint32 i = 0; i < m_TempBufferValues.size(); ++i)
					{
						rapidjson::Value v(rapidjson::kObjectType);
						v.AddMember(m_TempBufferValues[i].first, m_TempBufferValues[i].second, m_Document.GetAllocator());
						m_Document.PushBack(v, m_Document.GetAllocator());
					}
				}
			}
			else
			{
				rapidjson::Value array(rapidjson::kArrayType);
				if (!rawData)
				{
					for (uint32 i = 0; i < m_TempBufferValues.size(); ++i)
					{
						rapidjson::Value realValue(rapidjson::kObjectType);
						realValue.AddMember(m_TempBufferValues[i].first, m_TempBufferValues[i].second, m_Document.GetAllocator());

						rapidjson::Value keyValue(rapidjson::kStringType);
						keyValue.SetString("value", m_Document.GetAllocator());

						rapidjson::Value v(rapidjson::kObjectType);
						v.AddMember(m_TempBufferTypes[i].first, m_TempBufferTypes[i].second, m_Document.GetAllocator());
						v.AddMember(keyValue, realValue, m_Document.GetAllocator());
						array.PushBack(v, m_Document.GetAllocator());
					}
				}
				else
				{
					for (uint32 i = 0; i < m_TempBufferValues.size(); ++i)
					{
						rapidjson::Value v(rapidjson::kObjectType);
						v.AddMember(m_TempBufferValues[i].first, m_TempBufferValues[i].second, m_Document.GetAllocator());
						array.PushBack(v, m_Document.GetAllocator());
					}
				}

				rapidjson::Value arrayKeyName(rapidjson::kStringType);
				arrayKeyName.SetString(*key, m_Document.GetAllocator());

				m_Document.SetObject();
				m_Document.AddMember(arrayKeyName, array, m_Document.GetAllocator());
			}
		}
	}

	void JSONWriter::BeginObject()
	{
		m_ShouldWriteIntoObject = true;
	}

	void JSONWriter::EndObject(bool rawData)
	{
		if (m_ShouldWriteIntoObject)
		{
			m_ShouldWriteIntoObject = false;

			if (m_ShouldWriteIntoArray)
			{
				std::pair<rapidjson::Value, rapidjson::Value> &v = m_TempBufferValues.emplace_back();
				v.first = m_TempBufferValue.first;
				v.second = m_TempBufferValue.second;

				std::pair<rapidjson::Value, rapidjson::Value> &t = m_TempBufferTypes.emplace_back();
				t.first = m_TempBufferType.first;
				t.second = m_TempBufferType.second;
			}
			else
			{
				m_Document.SetObject();

				if (!rawData)
				{
					rapidjson::Value realVal(rapidjson::kObjectType);
					realVal.AddMember(m_TempBufferValue.first, m_TempBufferValue.second, m_Document.GetAllocator());

					rapidjson::Value realValStr(rapidjson::kStringType);
					realValStr.SetString("value", m_Document.GetAllocator());

					m_Document.AddMember(m_TempBufferType.first, m_TempBufferType.second, m_Document.GetAllocator());
					m_Document.AddMember(realValStr, realVal, m_Document.GetAllocator());
				}
				else
				{
					m_Document.AddMember(m_TempBufferValue.first, m_TempBufferValue.second, m_Document.GetAllocator());
				}
			}
		}
	}

	bool JSONWriter::WriteFloat(const HLString &key, float value)
	{
		return Write(key, DocumentDataType::Float, [value]() -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kObjectType);
			v.SetFloat(value);
			return v;
		});
	}

	bool JSONWriter::WriteDouble(const HLString &key, double value)
	{
		return Write(key, DocumentDataType::Double, [value]() -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kObjectType);
			v.SetDouble(value);
			return v;
		});
	}

	bool JSONWriter::WriteInt32(const HLString &key, int32 value)
	{
		return Write(key, DocumentDataType::Int32, [value]() -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kObjectType);
			v.SetInt(value);
			return v;
		});
	}

	bool JSONWriter::WriteUInt32(const HLString &key, uint32 value)
	{
		return Write(key, DocumentDataType::UInt32, [value]() -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kObjectType);
			v.SetUint(value);
			return v;
		});
	}

	bool JSONWriter::WriteInt64(const HLString &key, int64 value)
	{
		return Write(key, DocumentDataType::Int64, [value]() -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kObjectType);
			v.SetInt64(value);
			return v;
		});
	}

	bool JSONWriter::WriteUInt64(const HLString &key, uint64 value)
	{
		return Write(key, DocumentDataType::UInt64, [value]() -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kObjectType);
			v.SetUint64(value);
			return v;
		});
	}

	bool JSONWriter::WriteBool(const HLString &key, bool value)
	{
		return Write(key, DocumentDataType::Bool, [value]() -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kObjectType);
			v.SetBool(value);
			return v;
		});
	}

	bool JSONWriter::WriteString(const HLString &key, const HLString &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::String, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kStringType);
			v.SetString(*value, value.Length(), instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteVec2(const HLString &key, const glm::vec2 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec2, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value.x, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.y, instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteVec3(const HLString &key, const glm::vec3 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec3, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value.x, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.y, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.z, instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteVec4(const HLString &key, const glm::vec4 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec4, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value.x, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.y, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.z, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.w, instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteMat2(const HLString &key, const glm::mat2 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat2, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[0][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[0][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[1][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[1][1], instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteMat3(const HLString &key, const glm::mat3 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat3, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[0][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[0][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[0][2], instance->m_Document.GetAllocator());

			v.PushBack<float>(value[1][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[1][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[1][2], instance->m_Document.GetAllocator());

			v.PushBack<float>(value[2][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[2][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[2][2], instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteMat4(const HLString &key, const glm::mat4 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat4, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value[0][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[0][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[0][2], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[0][3], instance->m_Document.GetAllocator());

			v.PushBack<float>(value[1][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[1][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[1][2], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[1][3], instance->m_Document.GetAllocator());

			v.PushBack<float>(value[2][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[2][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[2][2], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[2][3], instance->m_Document.GetAllocator());

			v.PushBack<float>(value[3][0], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[3][1], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[3][2], instance->m_Document.GetAllocator());
			v.PushBack<float>(value[3][3], instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteQuaternion(const HLString &key, const glm::quat &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			v.PushBack<float>(value.w, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.x, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.y, instance->m_Document.GetAllocator());
			v.PushBack<float>(value.z, instance->m_Document.GetAllocator());
			return v;
		});
	}

	bool JSONWriter::WriteStringArray(const HLString &key, std::vector<HLString> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::String, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value str(rapidjson::kStringType);
				str.SetString(*value[i], value[i].Length(), instance->m_Document.GetAllocator());
				v.PushBack(str, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteInt32Array(const HLString &key, std::vector<int32> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Int32, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteUInt32Array(const HLString &key, std::vector<uint32> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::UInt32, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteInt64Array(const HLString &key, std::vector<int64> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Int64, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteUInt64Array(const HLString &key, std::vector<uint64> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::UInt64, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteBoolArray(const HLString &key, std::vector<bool> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Bool, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteFloatArray(const HLString &key, std::vector<float> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Float, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteDoubleArray(const HLString &key, std::vector<double> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Double, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteVec2Array(const HLString &key, std::vector<glm::vec2> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec2, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value entry(rapidjson::kArrayType);
				entry.PushBack<float>(value[i].x, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].y, instance->m_Document.GetAllocator());
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteVec3Array(const HLString &key, std::vector<glm::vec3> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec3, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value entry(rapidjson::kArrayType);
				entry.PushBack<float>(value[i].x, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].y, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].z, instance->m_Document.GetAllocator());
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteVec4Array(const HLString &key, std::vector<glm::vec4> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec4, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value entry(rapidjson::kArrayType);
				entry.PushBack<float>(value[i].x, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].y, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].z, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].w, instance->m_Document.GetAllocator());
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteMat2Array(const HLString &key, std::vector<glm::mat2> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat2, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value entry(rapidjson::kArrayType);
				entry.PushBack<float>(value[i][0][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][0][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][1][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][1][1], instance->m_Document.GetAllocator());
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteMat3Array(const HLString &key, std::vector<glm::mat3> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat3, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value entry(rapidjson::kArrayType);
				entry.PushBack<float>(value[i][0][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][0][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][0][2], instance->m_Document.GetAllocator());

				entry.PushBack<float>(value[i][1][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][1][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][1][2], instance->m_Document.GetAllocator());

				entry.PushBack<float>(value[i][2][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][2][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][2][2], instance->m_Document.GetAllocator());
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteMat4Array(const HLString &key, std::vector<glm::mat4> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat4, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value entry(rapidjson::kArrayType);
				entry.PushBack<float>(value[i][0][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][0][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][0][2], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][0][3], instance->m_Document.GetAllocator());

				entry.PushBack<float>(value[i][1][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][1][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][1][2], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][1][3], instance->m_Document.GetAllocator());

				entry.PushBack<float>(value[i][2][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][2][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][2][2], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][2][3], instance->m_Document.GetAllocator());

				entry.PushBack<float>(value[i][3][0], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][3][1], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][3][2], instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i][3][3], instance->m_Document.GetAllocator());
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteQuaternionArray(const HLString &key, std::vector<glm::quat> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [instance, value]() mutable -> rapidjson::Value
		{
			rapidjson::Value v(rapidjson::kArrayType);
			for (uint32 i = 0; i < value.size(); ++i)
			{
				rapidjson::Value entry(rapidjson::kArrayType);
				entry.PushBack<float>(value[i].w, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].x, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].y, instance->m_Document.GetAllocator());
				entry.PushBack<float>(value[i].z, instance->m_Document.GetAllocator());
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::ReadStringArray(const HLString &key, std::vector<HLString> &result)
	{
		return ReadArray(key, DocumentDataType::String, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsString())
				return false;

			HLString str = value.GetString();
			result.push_back(str);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", *str);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return ReadArray(key, DocumentDataType::Int32, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsInt())
				return false;

			int32 v = value.GetInt();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", v);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return ReadArray(key, DocumentDataType::UInt32, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsUint())
				return false;

			uint32 v = value.GetUint();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", v);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return ReadArray(key, DocumentDataType::Int64, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsInt64())
				return false;

			int64 v = value.GetInt64();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", v);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return ReadArray(key, DocumentDataType::UInt64, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsUint64())
				return false;

			uint64 v = value.GetUint64();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", v);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return ReadArray(key, DocumentDataType::Bool, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsBool())
				return false;

			bool v = value.GetBool();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", v);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return ReadArray(key, DocumentDataType::Float, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsFloat())
				return false;

			float v = value.GetFloat();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", v);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return ReadArray(key, DocumentDataType::Double, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsDouble())
				return false;

			double v = value.GetDouble();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}", v);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return ReadArray(key, DocumentDataType::Vec2, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::vec2 v;
			v.x = arr[0].GetFloat();
			v.y = arr[1].GetFloat();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}, {1}", v.x, v.y);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return ReadArray(key, DocumentDataType::Vec3, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::vec3 v;
			v.x = arr[0].GetFloat();
			v.y = arr[1].GetFloat();
			v.z = arr[2].GetFloat();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}, {1}, {2}", v.x, v.y, v.z);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return ReadArray(key, DocumentDataType::Vec4, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::vec4 v;
			v.x = arr[0].GetFloat();
			v.y = arr[1].GetFloat();
			v.z = arr[2].GetFloat();
			v.w = arr[3].GetFloat();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}, {1}, {2}, {3}", v.x, v.y, v.z, v.w);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return ReadArray(key, DocumentDataType::Mat2, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::mat2 v;
			v[0][0] = arr[0].GetFloat();
			v[0][1] = arr[1].GetFloat();

			v[1][0] = arr[2].GetFloat();
			v[1][1] = arr[3].GetFloat();
			result.push_back(v);

			return true;
		});
	}

	bool JSONWriter::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return ReadArray(key, DocumentDataType::Mat3, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::mat3 v;
			v[0][0] = arr[0].GetFloat();
			v[0][1] = arr[1].GetFloat();
			v[0][2] = arr[2].GetFloat();

			v[1][0] = arr[3].GetFloat();
			v[1][1] = arr[4].GetFloat();
			v[1][2] = arr[5].GetFloat();

			v[2][0] = arr[6].GetFloat();
			v[2][1] = arr[7].GetFloat();
			v[2][2] = arr[8].GetFloat();
			result.push_back(v);

			return true;
		});
	}

	bool JSONWriter::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return ReadArray(key, DocumentDataType::Mat4, [&result](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::mat4 v;
			v[0][0] = arr[0].GetFloat();
			v[0][1] = arr[1].GetFloat();
			v[0][2] = arr[2].GetFloat();
			v[0][3] = arr[3].GetFloat();

			v[1][0] = arr[4].GetFloat();
			v[1][1] = arr[5].GetFloat();
			v[1][2] = arr[6].GetFloat();
			v[1][3] = arr[7].GetFloat();

			v[2][0] = arr[8].GetFloat();
			v[2][1] = arr[9].GetFloat();
			v[2][2] = arr[10].GetFloat();
			v[2][3] = arr[11].GetFloat();

			v[3][0] = arr[12].GetFloat();
			v[3][1] = arr[13].GetFloat();
			v[3][2] = arr[14].GetFloat();
			v[3][3] = arr[15].GetFloat();
			result.push_back(v);

			return true;
		});
	}

	bool JSONWriter::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return ReadArray(key, DocumentDataType::Quat, [&result, &key](rapidjson::Value &value) mutable -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::quat v;
			v.w = arr[0].GetFloat();
			v.x = arr[1].GetFloat();
			v.y = arr[2].GetFloat();
			v.z = arr[3].GetFloat();
			result.push_back(v);

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}, {2}, {3}, {4}", *key, v.w, v.x, v.y, v.z);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return ReadArrayMap(key, DocumentDataType::String, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsString())
				return false;

			HLString value = v.GetString();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, *value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int32, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsInt())
				return false;

			int32 value = v.GetInt();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt32, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsUint())
				return false;

			uint32 value = v.GetUint();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int64, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsInt64())
				return false;

			int64 value = v.GetInt64();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt64, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsUint64())
				return false;

			uint64 value = v.GetUint64();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Bool, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsBool())
				return false;

			bool value = v.GetBool();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Float, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsFloat())
				return false;

			float value = v.GetFloat();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Double, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsDouble())
				return false;

			double value = v.GetDouble();
			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}", *keyValue, value);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsArray())
				return false;

			rapidjson::GenericArray values = v.GetArray();
			glm::vec2 value;
			value.x = values[0].GetFloat();
			value.y = values[1].GetFloat();

			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}, {2}", *keyValue, value.x, value.y);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec3, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsArray())
				return false;

			rapidjson::GenericArray values = v.GetArray();
			glm::vec3 value;
			value.x = values[0].GetFloat();
			value.y = values[1].GetFloat();
			value.z = values[2].GetFloat();

			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}, {2}, {3}", *keyValue, value.x, value.y, value.z);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec4, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsArray())
				return false;

			rapidjson::GenericArray values = v.GetArray();
			glm::vec4 value;
			value.x = values[0].GetFloat();
			value.y = values[1].GetFloat();
			value.z = values[2].GetFloat();
			value.w = values[3].GetFloat();

			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}, {2}, {3}, {4}", *keyValue, value.x, value.y, value.z, value.w);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat2, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsArray())
				return false;

			rapidjson::GenericArray values = v.GetArray();
			glm::mat2 value;
			value[0][0] = values[0].GetFloat();
			value[0][1] = values[1].GetFloat();

			value[1][0] = values[2].GetFloat();
			value[1][1] = values[3].GetFloat();

			result.insert({ keyValue, value });
			return true;
		});
	}

	bool JSONWriter::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat3, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsArray())
				return false;

			rapidjson::GenericArray values = v.GetArray();
			glm::mat3 value;
			value[0][0] = values[0].GetFloat();
			value[0][1] = values[1].GetFloat();
			value[0][2] = values[2].GetFloat();

			value[1][0] = values[3].GetFloat();
			value[1][1] = values[4].GetFloat();
			value[1][2] = values[5].GetFloat();

			value[2][0] = values[6].GetFloat();
			value[2][1] = values[7].GetFloat();
			value[2][2] = values[8].GetFloat();

			result.insert({ keyValue, value });
			return true;
		});
	}

	bool JSONWriter::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat4, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsArray())
				return false;

			rapidjson::GenericArray values = v.GetArray();
			glm::mat4 value;
			value[0][0] = values[0].GetFloat();
			value[0][1] = values[1].GetFloat();
			value[0][2] = values[2].GetFloat();
			value[0][3] = values[3].GetFloat();

			value[1][0] = values[4].GetFloat();
			value[1][1] = values[5].GetFloat();
			value[1][2] = values[6].GetFloat();
			value[1][3] = values[7].GetFloat();

			value[2][0] = values[8].GetFloat();
			value[2][1] = values[9].GetFloat();
			value[2][2] = values[10].GetFloat();
			value[2][3] = values[11].GetFloat();

			value[3][0] = values[12].GetFloat();
			value[3][1] = values[13].GetFloat();
			value[3][2] = values[14].GetFloat();
			value[3][3] = values[15].GetFloat();

			result.insert({ keyValue, value });
			return true;
		});
	}

	bool JSONWriter::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Quat, [&result](HLString &keyValue, rapidjson::Value &v) mutable -> bool
		{
			if (!v.IsArray())
				return false;

			rapidjson::GenericArray values = v.GetArray();
			glm::quat value;
			value.w = values[0].GetFloat();
			value.x = values[1].GetFloat();
			value.y = values[2].GetFloat();
			value.z = values[3].GetFloat();

			result.insert({ keyValue, value });

		#if JSON_PRINT_DEBUG_OUTPUT
			HL_CORE_TRACE("{0}={1}, {2}, {3}, {4}", *keyValue, value.w, value.x, value.y, value.z);
		#endif // JSON_PRINT_DEBUG_OUTPUT

			return true;
		});
	}

	bool JSONWriter::ReadFloat(const HLString &key, float *result)
	{
		return Read(key, DocumentDataType::Float, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsFloat())
				return false;

			*result = value.GetFloat();
			return true;
		});
	}

	bool JSONWriter::ReadDouble(const HLString &key, double *result)
	{
		return Read(key, DocumentDataType::Double, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsDouble())
				return false;

			*result = value.GetDouble();
			return true;
		});
	}

	bool JSONWriter::ReadInt32(const HLString &key, int32 *result)
	{
		return Read(key, DocumentDataType::Int32, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsInt())
				return false;

			*result = value.GetInt();
			return true;
		});
	}

	bool JSONWriter::ReadUInt32(const HLString &key, uint32 *result)
	{
		return Read(key, DocumentDataType::UInt32, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsUint())
				return false;

			*result = value.GetUint();
			return true;
		});
	}

	bool JSONWriter::ReadInt64(const HLString &key, int64 *result)
	{
		return Read(key, DocumentDataType::Int64, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsInt64())
				return false;

			*result = value.GetInt64();
			return true;
		});
	}

	bool JSONWriter::ReadUInt64(const HLString &key, uint64 *result)
	{
		return Read(key, DocumentDataType::UInt64, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsUint64())
				return false;

			*result = value.GetUint64();
			return true;
		});
	}

	bool JSONWriter::ReadBool(const HLString &key, bool *result)
	{
		return Read(key, DocumentDataType::Bool, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsBool())
				return false;

			*result = value.GetBool();
			return true;
		});
	}

	bool JSONWriter::ReadString(const HLString &key, HLString *result)
	{
		return Read(key, DocumentDataType::String, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsString())
				return false;

			*result = value.GetString();
			return true;
		});
	}

	bool JSONWriter::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return Read(key, DocumentDataType::Vec2, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat())
				return false;

			float x = arr[0].GetFloat();
			float y = arr[1].GetFloat();
			*result = { x, y };
			return true;
		});
	}

	bool JSONWriter::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return Read(key, DocumentDataType::Vec3, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat())
				return false;

			float x = arr[0].GetFloat();
			float y = arr[1].GetFloat();
			float z = arr[2].GetFloat();
			*result = { x, y, z };

			return true;
		});
	}

	bool JSONWriter::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return Read(key, DocumentDataType::Vec4, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat())
				return false;

			float x = arr[0].GetFloat();
			float y = arr[1].GetFloat();
			float z = arr[2].GetFloat();
			float w = arr[3].GetFloat();
			*result = { x, y, z, w };

			return true;
		});
	}

	bool JSONWriter::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return Read(key, DocumentDataType::Mat2, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			glm::mat2 m;
			m[0][0] = arr[0].GetFloat();
			m[0][1] = arr[1].GetFloat();

			m[1][0] = arr[2].GetFloat();
			m[1][1] = arr[3].GetFloat();
			*result = m;
			return true;
		});
	}

	bool JSONWriter::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return Read(key, DocumentDataType::Mat3, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
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
			*result = m;
			return true;
		});
	}

	bool JSONWriter::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return Read(key, DocumentDataType::Mat4, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
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
			*result = m;
			return true;
		});
	}

	bool JSONWriter::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return Read(key, DocumentDataType::Quat, [result](rapidjson::Value &value) -> bool
		{
			if (!value.IsArray())
				return false;

			rapidjson::GenericArray arr = value.GetArray();
			if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat())
				return false;

			float w = arr[0].GetFloat();
			float x = arr[1].GetFloat();
			float y = arr[2].GetFloat();
			float z = arr[3].GetFloat();
			*result = { w, x, y, z };

			return true;
		});
	}

	bool JSONWriter::HasKey(const HLString &key) const
	{
		return m_Document.HasMember(*key);
	}

	bool JSONWriter::WriteOut()
	{
		// Write content into file
		HL_CORE_INFO(JSON_LOG_PREFIX "[+] Writing file {0} [+]", **m_FilePath);
		HLString content = GetContent(true);

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

	HLString JSONWriter::GetContent(bool prettify)
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

	void JSONWriter::SetContent(const HLString &content)
	{
		m_Document.Parse(*content);
	}

	bool JSONWriter::AddIntoStructure(rapidjson::Value &keyType, rapidjson::Value &valType, DocumentDataType type)
	{
		bool result = false;

		auto &[typeKey, typeVal] = utils::ConvertDocumentTypeToRenderableFormat(m_Document, type);

		if (m_ShouldWriteIntoArray || m_ShouldWriteIntoObject)
		{
			result = true;
			m_TempBufferValue.first = keyType;
			m_TempBufferValue.second = valType;
			m_TempBufferType.first = typeKey;
			m_TempBufferType.second = typeVal;

			if (m_ShouldWriteIntoArray && !m_ShouldWriteIntoObject)
			{
				std::pair<rapidjson::Value, rapidjson::Value> &v = m_TempBufferValues.emplace_back();
				v.first = keyType;
				v.second = valType;

				std::pair<rapidjson::Value, rapidjson::Value> &t = m_TempBufferTypes.emplace_back();
				t.first = typeKey;
				t.second = typeVal;
			}
		}

		return result;
	}

	bool JSONWriter::Write(const HLString &key, DocumentDataType type, const std::function<rapidjson::Value()> &insertFunc)
	{
		if (key.IsEmpty())
		{
			HL_CORE_ERROR(JSON_LOG_PREFIX "[-] You have to specify a key! {0} [-]");
			return false;
		}

		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString(*key, key.Length(), m_Document.GetAllocator());

		rapidjson::Value value = insertFunc();
		if (!AddIntoStructure(keyType, value, type))
		{
			m_Document.SetObject();
			m_Document.AddMember(keyType, value, m_Document.GetAllocator());
		}

		return true;
	}

	bool JSONWriter::Read(const HLString &key, DocumentDataType type, const std::function<bool(rapidjson::Value &)> &insertFunc)
	{
		if (!m_Document.IsObject())
			return false;

		auto &obj = m_Document.GetObject();

		rapidjson::GenericMemberIterator typeVal = obj.FindMember("type");
		if (typeVal == obj.MemberEnd())
		{
			HL_ASSERT(obj.HasMember(*key));

			rapidjson::Value::MemberIterator it = obj.FindMember(*key);
			if (it == obj.MemberEnd())
				return false;

			if (key == it->name.GetString())
				return insertFunc(it->value);

			return false;
		}
		else
		{
			// User tries to access a value that is protected by it's type
			HL_ASSERT(obj.HasMember("type"));
			HL_ASSERT(obj.HasMember("value"));

			HLString currentType = typeVal->value.GetString();
			HLString realType = utils::DocumentDataTypeToString(type);

			if (currentType != realType)
			{
				HL_CORE_ERROR(JSON_LOG_PREFIX "[-] Parsing Error: Expected type {0} but got {1} [-]", *realType, *currentType);
				return false;
			}

			rapidjson::GenericMemberIterator value = obj.FindMember("value");
			if (value == obj.MemberEnd())
				return false;

			rapidjson::GenericMemberIterator valueIterator = value->value.GetObject().begin();
			if (valueIterator == value->value.MemberEnd())
				return false;

			if (key == valueIterator->name.GetString())
				return insertFunc(valueIterator->value);

			return false;
		}
	}

	bool JSONWriter::ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(rapidjson::Value &)> &insertFunc)
	{
		if (m_Document.IsNull())
			return false;

		HLString correctDataType = utils::DocumentDataTypeToString(type);

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
				rapidjson::GenericObject currentObj = arrIt->GetObject();

				// Get the data type
				rapidjson::GenericMemberIterator currentDataType = currentObj.FindMember("type");
				if (currentDataType == currentObj.MemberEnd())
				{
					// the user decided to parse the raw data instead of type-safe data
					rapidjson::GenericMemberIterator data = currentObj.begin();
					bool result = insertFunc(data->value);
					if (!result)
					{
						HL_CORE_ERROR("Error: User defined function returned false!");
						return false;
					}
				}
				else
				{
					if (currentDataType->value.IsString())
					{
						HLString type = currentDataType->value.GetString();
						if (type != correctDataType)
						{
							HL_CORE_ERROR("Error: Expected data type {0} but got {1}", *correctDataType, *type);
							return false;
						}
					}

					// Get the actual value
					rapidjson::GenericMemberIterator currentValue = currentObj.FindMember("value")->value.GetObject().begin();
					if (currentValue == currentObj.MemberEnd())
					{
						HL_CORE_ERROR("Error: required field value!");
						return false;
					}

					bool result = insertFunc(currentValue->value);
					if (!result)
					{
						HL_CORE_ERROR("Error: User defined function returned false!");
						return false;
					}
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericObject currentObj = it->GetObject();

			// Get the data type
			rapidjson::GenericMemberIterator currentDataType = currentObj.FindMember("type");
			if (currentDataType == currentObj.MemberEnd())
			{
				// the user decided to parse the raw data instead of type-safe data
				rapidjson::GenericMemberIterator data = currentObj.begin();
				bool result = insertFunc(data->value);
				if (!result)
				{
					HL_CORE_ERROR("Error: User defined function returned false!");
					return false;
				}
			}
			else
			{
				if (currentDataType->value.IsString())
				{
					HLString type = currentDataType->value.GetString();
					if (type != correctDataType)
					{
						HL_CORE_ERROR("Error: Expected data type {0} but got {1}", *correctDataType, *type);
						return false;
					}
				}

				// Get the actual value
				rapidjson::GenericMemberIterator currentValue = currentObj.FindMember("value")->value.GetObject().begin();
				if (currentValue == currentObj.MemberEnd())
				{
					HL_CORE_ERROR("Error: Required field value");
					return false;
				}

				bool result = insertFunc(currentValue->value);
				if (!result)
				{
					HL_CORE_ERROR("Error: User defined function returned false!");
					return false;
				}
			}
		}

		return true;
	}

	bool JSONWriter::ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(HLString, rapidjson::Value &)> &insertFunc)
	{
		if (m_Document.IsNull())
			return false;

		HLString correctDataType = utils::DocumentDataTypeToString(type);

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
				rapidjson::GenericObject currentObj = arrIt->GetObject();

				// Get the data type
				rapidjson::GenericMemberIterator currentDataType = currentObj.FindMember("type");
				if (currentDataType == currentObj.MemberEnd())
				{
					// the user decided to parse the raw data instead of type-safe data
					rapidjson::GenericMemberIterator data = currentObj.begin();
					bool result = insertFunc(data->name.GetString(), data->value);
					if (!result)
					{
						HL_CORE_ERROR("Error: User defined function returned false!");
						return false;
					}
				}
				else
				{
					if (currentDataType->value.IsString())
					{
						HLString type = currentDataType->value.GetString();
						if (type != correctDataType)
						{
							HL_CORE_ERROR("Error: Expected data type {0} but got {1}", *correctDataType, *type);
							return false;
						}
					}

					// Get the actual value
					rapidjson::GenericMemberIterator currentValue = currentObj.FindMember("value")->value.GetObject().begin();
					if (currentValue == currentObj.MemberEnd())
					{
						HL_CORE_ERROR("Error: Required field value");
						return false;
					}

					bool result = insertFunc(currentValue->name.GetString(), currentValue->value);
					if (!result)
					{
						HL_CORE_ERROR("Error: User defined function returned false!");
						return false;
					}
				}
			}

			return true;
		}

		rapidjson::GenericArray arr = m_Document.GetArray();
		for (auto it = arr.Begin(); it != arr.End(); ++it)
		{
			rapidjson::GenericObject currentObj = it->GetObject();

			// Get the data type
			rapidjson::GenericMemberIterator currentDataType = currentObj.FindMember("type");
			if (currentDataType == currentObj.MemberEnd())
			{
				// the user decided to parse the raw data instead of type-safe data
				rapidjson::GenericMemberIterator data = currentObj.begin();
				bool result = insertFunc(data->name.GetString(), data->value);
				if (!result)
				{
					HL_CORE_ERROR("Error: User defined function returned false!");
					return false;
				}
			}
			else
			{
				if (currentDataType->value.IsString())
				{
					HLString type = currentDataType->value.GetString();
					if (type != correctDataType)
					{
						HL_CORE_ERROR("Error: Expected data type {0} but got {1}", *correctDataType, *type);
						return false;
					}
				}

				// Get the actual value
				rapidjson::GenericMemberIterator currentValue = currentObj.FindMember("value")->value.GetObject().begin();
				if (currentValue == currentObj.MemberEnd())
				{
					HL_CORE_ERROR("Error: Required field value");
					return false;
				}

				bool result = insertFunc(currentValue->name.GetString(), currentValue->value);
				if (!result)
				{
					HL_CORE_ERROR("Error: User defined function returned false!");
					return false;
				}
			}
		}

		return true;
	}
}
