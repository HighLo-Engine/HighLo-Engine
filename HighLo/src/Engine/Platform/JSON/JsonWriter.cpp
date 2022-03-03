// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "JsonWriter.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/LoaderUtils.h"
#include "JsonHelper.h"

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

	void JSONWriter::EndArray(const HLString &key)
	{
	}

	void JSONWriter::BeginObject()
	{
		m_ShouldWriteIntoObject = true;
	}

	void JSONWriter::EndObject()
	{
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

	bool JSONWriter::WriteStringArray(const HLString &key, const std::vector<HLString> &value)
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

	bool JSONWriter::WriteInt32Array(const HLString &key, const std::vector<int32> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Int32, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteUInt32Array(const HLString &key, const std::vector<uint32> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::UInt32, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteInt64Array(const HLString &key, const std::vector<int64> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Int64, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteUInt64Array(const HLString &key, const std::vector<uint64> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::UInt64, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteBoolArray(const HLString &key, const std::vector<bool> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Bool, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}

	bool JSONWriter::WriteFloatArray(const HLString &key, const std::vector<float> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Float, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}
	
	bool JSONWriter::WriteDoubleArray(const HLString &key, const std::vector<double> &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Double, [instance, value]() mutable -> rapidjson::Value
		{
			return instance->ConvertStdArrToRapidJsonArr(value);
		});
	}
	
	bool JSONWriter::WriteVec2Array(const HLString &key, const std::vector<glm::vec2> &value)
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

	bool JSONWriter::WriteVec3Array(const HLString &key, const std::vector<glm::vec3> &value)
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

	bool JSONWriter::WriteVec4Array(const HLString &key, const std::vector<glm::vec4> &value)
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

	bool JSONWriter::WriteMat2Array(const HLString &key, const std::vector<glm::mat2> &value)
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

	bool JSONWriter::WriteMat3Array(const HLString &key, const std::vector<glm::mat3> &value)
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

	bool JSONWriter::WriteMat4Array(const HLString &key, const std::vector<glm::mat4> &value)
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

	bool JSONWriter::WriteQuaternionArray(const HLString &key, const std::vector<glm::quat> &value)
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

	bool JSONWriter::WriteStringArrayMap(const HLString &key, const std::map<HLString, HLString> &map)
	{
		return false;
	}

	bool JSONWriter::WriteInt32ArrayMap(const HLString &key, const std::map<HLString, int32> &map)
	{
		return false;
	}

	bool JSONWriter::WriteUInt32ArrayMap(const HLString &key, const std::map<HLString, uint32> &map)
	{
		return false;
	}

	bool JSONWriter::WriteInt64ArrayMap(const HLString &key, const std::map<HLString, int64> &map)
	{
		return false;
	}

	bool JSONWriter::WriteUInt64ArrayMap(const HLString &key, const std::map<HLString, uint64> &map)
	{
		return false;
	}

	bool JSONWriter::WriteBoolArrayMap(const HLString &key, const std::map<HLString, bool> &map)
	{
		return false;
	}

	bool JSONWriter::WriteFloatArrayMap(const HLString &key, const std::map<HLString, float> &map)
	{
		return false;
	}

	bool JSONWriter::WriteDoubleArrayMap(const HLString &key, const std::map<HLString, double> &map)
	{
		return false;
	}

	bool JSONWriter::WriteVec2ArrayMap(const HLString &key, const std::map<HLString, glm::vec2> &map)
	{
		return false;
	}

	bool JSONWriter::WriteVec3ArrayMap(const HLString &key, const std::map<HLString, glm::vec3> &map)
	{
		return false;
	}

	bool JSONWriter::WriteVec4ArrayMap(const HLString &key, const std::map<HLString, glm::vec4> &map)
	{
		return false;
	}

	bool JSONWriter::WriteMat2ArrayMap(const HLString &key, const std::map<HLString, glm::mat2> &map)
	{
		return false;
	}

	bool JSONWriter::WriteMat3ArrayMap(const HLString &key, const std::map<HLString, glm::mat3> &map)
	{
		return false;
	}

	bool JSONWriter::WriteMat4ArrayMap(const HLString &key, const std::map<HLString, glm::mat4> &map)
	{
		return false;
	}

	bool JSONWriter::WriteQuaternionArrayMap(const HLString &key, const std::map<HLString, glm::quat> &map)
	{
		return false;
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
		return false;
	}
	
	bool JSONWriter::Write(const HLString &key, DocumentDataType type, const std::function<rapidjson::Value()> &insertFunc)
	{
		return false;
	}
}

