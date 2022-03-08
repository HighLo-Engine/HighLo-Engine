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
		if (m_ShouldWriteIntoArray)
		{
			m_ShouldWriteIntoArray = false;
			m_Document.SetArray();

			if (m_ObjectsNeverUsed)
			{
				// The values should be inserted into the array structure without type safety, see example #3

				for (uint32 i = 0; i < m_ValuePairs.size(); ++i)
				{
					auto &valuePair = m_ValuePairs[i];
					m_Document.PushBack(valuePair.second, m_Document.GetAllocator());
				}
			}
			else
			{
				if (m_ObjectsPerGroup > 1)
				{
					// We have more than one object per group, so add them into their own sub-array, to keep the objects of one group together
					for (uint32 i = 0; i < m_ValuePairs.size(); i += m_ObjectsPerGroup)
					{
						auto &valuePair = m_ValuePairs[i];
						auto &typePair = m_TypePairs[i];

						rapidjson::Value arrElem(rapidjson::kArrayType);

						rapidjson::Value currentObj(rapidjson::kObjectType);
						currentObj.AddMember(valuePair.first, valuePair.second, m_Document.GetAllocator());
						currentObj.AddMember(typePair.first, typePair.second, m_Document.GetAllocator());
						arrElem.PushBack(currentObj, m_Document.GetAllocator());

						for (uint32 j = i + 1; j < i + m_ObjectsPerGroup; ++j)
						{
							auto &anotherValuePair = m_ValuePairs[j];
							auto &anotherTypePair = m_TypePairs[j];

							rapidjson::Value otherObj(rapidjson::kObjectType);
							otherObj.AddMember(anotherValuePair.first, anotherValuePair.second, m_Document.GetAllocator());
							otherObj.AddMember(anotherTypePair.first, anotherTypePair.second, m_Document.GetAllocator());
							arrElem.PushBack(otherObj, m_Document.GetAllocator());
						}

						m_Document.PushBack(arrElem, m_Document.GetAllocator());
					}
				}
				else
				{
					// We only have one object per group, so instead of a sub-array we add the objects normally to the document
					for (uint32 i = 0; i < m_ValuePairs.size(); ++i)
					{
						auto &valuePair = m_ValuePairs[i];
						auto &typePair = m_TypePairs[i];

						rapidjson::Value currentObj(rapidjson::kObjectType);
						currentObj.AddMember(valuePair.first, valuePair.second, m_Document.GetAllocator());
						currentObj.AddMember(typePair.first, typePair.second, m_Document.GetAllocator());
						m_Document.PushBack(currentObj, m_Document.GetAllocator());
					}
				}
			}
		}
	}

	void JSONWriter::BeginObject()
	{
		m_ShouldWriteIntoObject = true;
		m_ObjectsNeverUsed = false;
		m_ObjectsPerGroup = 0;
	}

	void JSONWriter::EndObject(const HLString &key)
	{
		if (m_ShouldWriteIntoObject)
		{
			m_ShouldWriteIntoObject = false;

			if (!m_ShouldWriteIntoArray)
			{
				m_Document.SetObject();
				for (uint32 i = 0; i < m_ValuePairs.size(); ++i)
				{
					auto &valuePair = m_ValuePairs[i];
					auto &typePair = m_TypePairs[i];

					rapidjson::Value currentObj(rapidjson::kObjectType);
					currentObj.AddMember("value", valuePair.second, m_Document.GetAllocator());
					currentObj.AddMember(typePair.first, typePair.second, m_Document.GetAllocator());

					m_Document.AddMember(valuePair.first, currentObj, m_Document.GetAllocator());
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
			return utils::Vec2ToJSON(value, instance->m_Document);
		});
	}

	bool JSONWriter::WriteVec3(const HLString &key, const glm::vec3 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec3, [instance, value]() mutable -> rapidjson::Value
		{
			return utils::Vec3ToJSON(value, instance->m_Document);
		});
	}

	bool JSONWriter::WriteVec4(const HLString &key, const glm::vec4 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec4, [instance, value]() mutable -> rapidjson::Value
		{
			return utils::Vec4ToJSON(value, instance->m_Document);
		});
	}

	bool JSONWriter::WriteMat2(const HLString &key, const glm::mat2 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat2, [instance, value]() mutable -> rapidjson::Value
		{
			return utils::Mat2ToJSON(value, instance->m_Document);
		});
	}

	bool JSONWriter::WriteMat3(const HLString &key, const glm::mat3 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat3, [instance, value]() mutable -> rapidjson::Value
		{
			return utils::Mat3ToJSON(value, instance->m_Document);
		});
	}

	bool JSONWriter::WriteMat4(const HLString &key, const glm::mat4 &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat4, [instance, value]() mutable -> rapidjson::Value
		{
			return utils::Mat4ToJSON(value, instance->m_Document);
		});
	}

	bool JSONWriter::WriteQuaternion(const HLString &key, const glm::quat &value)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [instance, value]() mutable -> rapidjson::Value
		{
			return utils::QuatToJSON(value, instance->m_Document);
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
				rapidjson::Value entry = utils::Vec2ToJSON(value[i], instance->m_Document);
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
				rapidjson::Value entry = utils::Vec3ToJSON(value[i], instance->m_Document);
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
				rapidjson::Value entry = utils::Vec4ToJSON(value[i], instance->m_Document);
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
				rapidjson::Value entry = utils::Mat2ToJSON(value[i], instance->m_Document);
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
				rapidjson::Value entry = utils::Mat3ToJSON(value[i], instance->m_Document);
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
				rapidjson::Value entry = utils::Mat4ToJSON(value[i], instance->m_Document);
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
				rapidjson::Value entry = utils::QuatToJSON(value[i], instance->m_Document);
				v.PushBack(entry, instance->m_Document.GetAllocator());
			}
			return v;
		});
	}

	bool JSONWriter::WriteStringArrayMap(const HLString &key, const std::map<HLString, HLString> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::String, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper(rapidjson::kStringType);
				valueWrapper.SetString(*value, value.Length(), instance->m_Document.GetAllocator());

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::String);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteInt32ArrayMap(const HLString &key, const std::map<HLString, int32> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Int32, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper;
				valueWrapper.SetInt(value);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::Int32);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteUInt32ArrayMap(const HLString &key, const std::map<HLString, uint32> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::UInt32, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper;
				valueWrapper.SetUint(value);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::UInt32);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteInt64ArrayMap(const HLString &key, const std::map<HLString, int64> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Int64, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper;
				valueWrapper.SetInt64(value);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::Int64);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteUInt64ArrayMap(const HLString &key, const std::map<HLString, uint64> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::UInt64, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);
				
				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper;
				valueWrapper.SetUint64(value);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::UInt64);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteBoolArrayMap(const HLString &key, const std::map<HLString, bool> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Bool, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper;
				valueWrapper.SetBool(value);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::Bool);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteFloatArrayMap(const HLString &key, const std::map<HLString, float> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Float, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper;
				valueWrapper.SetFloat(value);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::Float);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteDoubleArrayMap(const HLString &key, const std::map<HLString, double> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Double, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper;
				valueWrapper.SetDouble(value);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());

				if (instance->m_UseTypeSafety)
				{
					auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(instance->m_Document, DocumentDataType::Double);
					obj.AddMember(typeKey, typeValue, instance->m_Document.GetAllocator());
				}

				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteVec2ArrayMap(const HLString &key, const std::map<HLString, glm::vec2> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec2, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper = utils::Vec2ToJSON(value, instance->m_Document);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());
				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteVec3ArrayMap(const HLString &key, const std::map<HLString, glm::vec3> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec3, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper = utils::Vec3ToJSON(value, instance->m_Document);
				
				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());
				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteVec4ArrayMap(const HLString &key, const std::map<HLString, glm::vec4> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Vec4, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper = utils::Vec4ToJSON(value, instance->m_Document);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());
				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteMat2ArrayMap(const HLString &key, const std::map<HLString, glm::mat2> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat2, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper = utils::Mat2ToJSON(value, instance->m_Document);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());
				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteMat3ArrayMap(const HLString &key, const std::map<HLString, glm::mat3> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat3, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper = utils::Mat3ToJSON(value, instance->m_Document);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());
				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteMat4ArrayMap(const HLString &key, const std::map<HLString, glm::mat4> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Mat4, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper = utils::Mat4ToJSON(value, instance->m_Document);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());
				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
	}

	bool JSONWriter::WriteQuaternionArrayMap(const HLString &key, const std::map<HLString, glm::quat> &map)
	{
		Ref<JSONWriter> instance = this;
		return Write(key, DocumentDataType::Quat, [instance, map]() mutable -> rapidjson::Value
		{
			rapidjson::Value entries(rapidjson::kArrayType);
			for (auto &[str, value] : map)
			{
				rapidjson::Value obj(rapidjson::kObjectType);

				rapidjson::Value strWrapper(rapidjson::kStringType);
				strWrapper.SetString(*str, str.Length(), instance->m_Document.GetAllocator());

				rapidjson::Value valueWrapper = utils::QuatToJSON(value, instance->m_Document);

				obj.AddMember(strWrapper, valueWrapper, instance->m_Document.GetAllocator());
				entries.PushBack(obj, instance->m_Document.GetAllocator());
			}

			return entries;
		});
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
	
	bool JSONWriter::Write(const HLString &key, DocumentDataType type, const std::function<rapidjson::Value()> &insertFunc)
	{
		++m_ObjectsPerGroup;
		rapidjson::Value val = insertFunc();
		
		rapidjson::Value keyVal(rapidjson::kStringType);
		if (!key.IsEmpty())
			keyVal.SetString(*key, key.Length(), m_Document.GetAllocator());

		if (!m_ShouldWriteIntoArray && !m_ShouldWriteIntoObject)
		{
			static bool firstPushIntoDocument = true;
			if (firstPushIntoDocument)
			{
				firstPushIntoDocument = false;
				m_Document.SetObject();
			}

			m_Document.AddMember(keyVal, val, m_Document.GetAllocator());
		}
		else
		{
			auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(m_Document, type);

			auto &valuePair = m_ValuePairs.emplace_back();
			valuePair.first = keyVal;
			valuePair.second = val;

			auto &typePair = m_TypePairs.emplace_back();
			typePair.first = typeKey;
			typePair.second = typeValue;
		}

		return true;
	}
}

