// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-03) initial release
//

#pragma once

#include "Engine/Loaders/DocumentType.h"
#include "Engine/Utils/LoaderUtils.h"

#include <rapidjson/document.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

#undef GetObject

namespace highlo::utils
{
	static std::pair<rapidjson::Value, rapidjson::Value> ConvertDocumentTypeToRenderableFormat(rapidjson::Document &document, DocumentDataType type)
	{
		std::pair<rapidjson::Value, rapidjson::Value> result;
		HLString typeStr = utils::DocumentDataTypeToString(type);

		rapidjson::Value key(rapidjson::kStringType);
		key.SetString("type", document.GetAllocator());

		rapidjson::Value value(rapidjson::kStringType);
		value.SetString(*typeStr, typeStr.Length(), document.GetAllocator());

		result.first = key;
		result.second = value;
		return result;
	}

	template<typename T>
	static rapidjson::Value ConvertStdArrToRapidJsonArr(const std::vector<T> &arr, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		for (uint32 i = 0; i < arr.size(); ++i)
			result.PushBack(arr[i], document.GetAllocator());

		return result;
	}

	template<typename T>
	static rapidjson::Value ConvertStdMapToRapidJsonArr(const std::map<HLString, T> &map, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		for (auto &[key, value] : map)
		{
			rapidjson::Value obj(rapidjson::kObjectType);
			
			rapidjson::Value k(rapidjson::kStringType);
			k.SetString(*key, key.Length(), document.GetAllocator());

			rapidjson::Value v;
			v.Set<T>(value);

			obj.AddMember(k, v, document.GetAllocator());
			result.PushBack(obj, document.GetAllocator());
		}

		return result;
	}

	static rapidjson::Value Vec2ToJSON(const glm::vec2 &v, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		result.PushBack<float>(v.x, document.GetAllocator());
		result.PushBack<float>(v.y, document.GetAllocator());
		return result;
	}

	static rapidjson::Value Vec3ToJSON(const glm::vec3 &v, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		result.PushBack<float>(v.x, document.GetAllocator());
		result.PushBack<float>(v.y, document.GetAllocator());
		result.PushBack<float>(v.z, document.GetAllocator());
		return result;
	}

	static rapidjson::Value Vec4ToJSON(const glm::vec4 &v, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		result.PushBack<float>(v.x, document.GetAllocator());
		result.PushBack<float>(v.y, document.GetAllocator());
		result.PushBack<float>(v.z, document.GetAllocator());
		result.PushBack<float>(v.w, document.GetAllocator());
		return result;
	}

	static rapidjson::Value Mat2ToJSON(const glm::mat2 &mat, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		result.PushBack<float>(mat[0][0], document.GetAllocator());
		result.PushBack<float>(mat[0][1], document.GetAllocator());
		result.PushBack<float>(mat[1][0], document.GetAllocator());
		result.PushBack<float>(mat[1][1], document.GetAllocator());
		return result;
	}

	static rapidjson::Value Mat3ToJSON(const glm::mat3 &mat, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		result.PushBack<float>(mat[0][0], document.GetAllocator());
		result.PushBack<float>(mat[0][1], document.GetAllocator());
		result.PushBack<float>(mat[0][2], document.GetAllocator());

		result.PushBack<float>(mat[1][0], document.GetAllocator());
		result.PushBack<float>(mat[1][1], document.GetAllocator());
		result.PushBack<float>(mat[1][2], document.GetAllocator());

		result.PushBack<float>(mat[2][0], document.GetAllocator());
		result.PushBack<float>(mat[2][1], document.GetAllocator());
		result.PushBack<float>(mat[2][2], document.GetAllocator());
		return result;
	}

	static rapidjson::Value Mat4ToJSON(const glm::mat4 &mat, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		result.PushBack<float>(mat[0][0], document.GetAllocator());
		result.PushBack<float>(mat[0][1], document.GetAllocator());
		result.PushBack<float>(mat[0][2], document.GetAllocator());
		result.PushBack<float>(mat[0][3], document.GetAllocator());

		result.PushBack<float>(mat[1][0], document.GetAllocator());
		result.PushBack<float>(mat[1][1], document.GetAllocator());
		result.PushBack<float>(mat[1][2], document.GetAllocator());
		result.PushBack<float>(mat[1][3], document.GetAllocator());

		result.PushBack<float>(mat[2][0], document.GetAllocator());
		result.PushBack<float>(mat[2][1], document.GetAllocator());
		result.PushBack<float>(mat[2][2], document.GetAllocator());
		result.PushBack<float>(mat[2][3], document.GetAllocator());

		result.PushBack<float>(mat[3][0], document.GetAllocator());
		result.PushBack<float>(mat[3][1], document.GetAllocator());
		result.PushBack<float>(mat[3][2], document.GetAllocator());
		result.PushBack<float>(mat[3][3], document.GetAllocator());
		return result;
	}

	static rapidjson::Value QuatToJSON(const glm::quat &q, rapidjson::Document &document)
	{
		rapidjson::Value result(rapidjson::kArrayType);
		result.PushBack<float>(q.w, document.GetAllocator());
		result.PushBack<float>(q.x, document.GetAllocator());
		result.PushBack<float>(q.y, document.GetAllocator());
		result.PushBack<float>(q.z, document.GetAllocator());
		return result;
	}

	template<typename MapValueType>
	static rapidjson::Value ConvertMapToJsonObject(const std::map<HLString, MapValueType> &map, rapidjson::Document &doc, DocumentDataType type)
	{
		rapidjson::Value result(rapidjson::kArrayType);

		for (auto &[k, v] : map)
		{
			// NOTE: Create strings every iteration (even for valueStrDecl), because rapidjson handles value objects only by reference
			//       and if the references get added as a member they lose their attributes in the next iteration and rapidjson asserts
			auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(doc, type);

			rapidjson::Value currentObj(rapidjson::kObjectType);
			rapidjson::Value valueWrapper(rapidjson::kObjectType);

			rapidjson::Value valueStrDecl(rapidjson::kStringType);
			rapidjson::Value userKey(rapidjson::kStringType);

			rapidjson::Value userValue;
			if constexpr (std::is_same<MapValueType, HLString>::value)
			{
				if (type == DocumentDataType::String)
				{
					userValue.SetString(v.C_Str(), v.Length(), doc.GetAllocator());
				}
			}
			else if constexpr (!std::is_class<MapValueType>::value)
			{
				switch (type)
				{
					case DocumentDataType::Bool:
						userValue.SetBool(*((bool*)&v));
						break;

					case DocumentDataType::Int32:
						userValue.SetInt(*((int32*)&v));
						break;

					case DocumentDataType::UInt32:
						userValue.SetUint(*((uint32*)&v));
						break;

					case DocumentDataType::Int64:
						userValue.SetInt64(*((int64*)&v));
						break;

					case DocumentDataType::UInt64:
						userValue.SetUint64(*((uint64*)&v));
						break;

					case DocumentDataType::Float:
						userValue.SetFloat(*((float*)&v));
						break;

					case DocumentDataType::Double:
						userValue.SetDouble(*((double*)&v));
						break;
				}
			}
			else
			{
				switch (type)
				{
					case DocumentDataType::Vec2:
					{
						glm::vec2 vector = (*((glm::vec2*)&v[0]));
						userValue = utils::Vec2ToJSON(vector, doc);
						break;
					}

					case DocumentDataType::Vec3:
					{
						glm::vec3 vector = (*((glm::vec3*)&v[0]));
						userValue = utils::Vec3ToJSON(vector, doc);
						break;
					}

					case DocumentDataType::Vec4:
					{
						glm::vec4 vector = (*((glm::vec4*)&v[0]));
						userValue = utils::Vec4ToJSON(vector, doc);
						break;
					}

					case DocumentDataType::Mat2:
					{
						glm::mat2 matrix = (*((glm::mat2*)&v[0]));
						userValue = utils::Mat2ToJSON(matrix, doc);
						break;
					}

					case DocumentDataType::Mat3:
					{
						glm::mat3 matrix = (*((glm::mat3*)&v[0]));
						userValue = utils::Mat3ToJSON(matrix, doc);
						break;
					}

					case DocumentDataType::Mat4:
					{
						glm::mat4 matrix = (*((glm::mat4*)&v[0]));
						userValue = utils::Mat4ToJSON(matrix, doc);
						break;
					}

					case DocumentDataType::Quat:
					{
						glm::quat quaternion = (*((glm::quat*)&v[0]));
						userValue = utils::QuatToJSON(quaternion, doc);
						break;
					}
				}
			}

			valueStrDecl.SetString("value", doc.GetAllocator());
			userKey.SetString(k.C_Str(), k.Length(), doc.GetAllocator());

			valueWrapper.AddMember(userKey, userValue, doc.GetAllocator());

			currentObj.AddMember(typeKey, typeValue, doc.GetAllocator());
			currentObj.AddMember(valueStrDecl, valueWrapper, doc.GetAllocator());
			result.PushBack(currentObj, doc.GetAllocator());
		}

		return result;
	}

	template<typename MapValueType>
	static rapidjson::Value ConvertUnorderedMapToJsonObject(const std::unordered_map<HLString, MapValueType> &map, rapidjson::Document &doc, DocumentDataType type)
	{
		rapidjson::Value result(rapidjson::kArrayType);

		for (auto &[k, v] : map)
		{
			// NOTE: Create strings every iteration (even for valueStrDecl), because rapidjson handles value objects only by reference
			//       and if the references get added as a member they lose their attributes in the next iteration and rapidjson asserts
			auto &[typeKey, typeValue] = utils::ConvertDocumentTypeToRenderableFormat(doc, type);

			rapidjson::Value currentObj(rapidjson::kObjectType);
			rapidjson::Value valueWrapper(rapidjson::kObjectType);

			rapidjson::Value valueStrDecl(rapidjson::kStringType);
			rapidjson::Value userKey(rapidjson::kStringType);

			rapidjson::Value userValue;
			if constexpr (std::is_same<MapValueType, HLString>::value)
			{
				if (type == DocumentDataType::String)
				{
					userValue.SetString(v.C_Str(), v.Length(), doc.GetAllocator());
				}
			}
			else if constexpr (!std::is_class<MapValueType>::value)
			{
				switch (type)
				{
					case DocumentDataType::Bool:
						userValue.SetBool(*((bool*)&v));
						break;

					case DocumentDataType::Int32:
						userValue.SetInt(*((int32*)&v));
						break;

					case DocumentDataType::UInt32:
						userValue.SetUint(*((uint32*)&v));
						break;

					case DocumentDataType::Int64:
						userValue.SetInt64(*((int64*)&v));
						break;

					case DocumentDataType::UInt64:
						userValue.SetUint64(*((uint64*)&v));
						break;

					case DocumentDataType::Float:
						userValue.SetFloat(*((float*)&v));
						break;

					case DocumentDataType::Double:
						userValue.SetDouble(*((double*)&v));
						break;
				}
			}
			else
			{
				switch (type)
				{
					case DocumentDataType::Vec2:
					{
						glm::vec2 vector = (*((glm::vec2*)&v[0]));
						userValue = utils::Vec2ToJSON(vector, doc);
						break;
					}

					case DocumentDataType::Vec3:
					{
						glm::vec3 vector = (*((glm::vec3*)&v[0]));
						userValue = utils::Vec3ToJSON(vector, doc);
						break;
					}

					case DocumentDataType::Vec4:
					{
						glm::vec4 vector = (*((glm::vec4*)&v[0]));
						userValue = utils::Vec4ToJSON(vector, doc);
						break;
					}

					case DocumentDataType::Mat2:
					{
						glm::mat2 matrix = (*((glm::mat2*)&v[0]));
						userValue = utils::Mat2ToJSON(matrix, doc);
						break;
					}

					case DocumentDataType::Mat3:
					{
						glm::mat3 matrix = (*((glm::mat3*)&v[0]));
						userValue = utils::Mat3ToJSON(matrix, doc);
						break;
					}

					case DocumentDataType::Mat4:
					{
						glm::mat4 matrix = (*((glm::mat4*)&v[0]));
						userValue = utils::Mat4ToJSON(matrix, doc);
						break;
					}

					case DocumentDataType::Quat:
					{
						glm::quat quaternion = (*((glm::quat*)&v[0]));
						userValue = utils::QuatToJSON(quaternion, doc);
						break;
					}
				}
			}

			valueStrDecl.SetString("value", doc.GetAllocator());
			userKey.SetString(k.C_Str(), k.Length(), doc.GetAllocator());

			valueWrapper.AddMember(userKey, userValue, doc.GetAllocator());

			currentObj.AddMember(typeKey, typeValue, doc.GetAllocator());
			currentObj.AddMember(valueStrDecl, valueWrapper, doc.GetAllocator());
			result.PushBack(currentObj, doc.GetAllocator());
		}

		return result;
	}

	// ==================================================================================================================================================================
	// ==================================================================================================================================================================
	// ==================================================================================================================================================================

	static bool JSONToVec2(const rapidjson::Value &node, glm::vec2 *outVector)
	{
		if (!node.IsArray())
			return false;

		rapidjson::GenericArray arr = node.GetArray();
		if (!arr[0].IsFloat() || !arr[1].IsFloat())
			return false;

		float x = arr[0].GetFloat();
		float y = arr[1].GetFloat();

		if (outVector)
			*outVector = { x, y };

		return true;
	}

	static bool JSONToVec3(const rapidjson::Value &node, glm::vec3 *outVector)
	{
		if (!node.IsArray())
			return false;

		rapidjson::GenericArray arr = node.GetArray();
		if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat())
			return false;

		float x = arr[0].GetFloat();
		float y = arr[1].GetFloat();
		float z = arr[2].GetFloat();

		if (outVector)
			*outVector = { x, y, z };

		return true;
	}

	static bool JSONToVec4(const rapidjson::Value &node, glm::vec4 *outVector)
	{
		if (!node.IsArray())
			return false;

		rapidjson::GenericArray arr = node.GetArray();
		if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat())
			return false;

		float x = arr[0].GetFloat();
		float y = arr[1].GetFloat();
		float z = arr[2].GetFloat();
		float w = arr[3].GetFloat();

		if (outVector)
			*outVector = { x, y, z, w };

		return true;
	}

	static bool JSONToMat2(const rapidjson::Value &node, glm::mat2 *outMatrix)
	{
		if (!node.IsArray())
			return false;

		rapidjson::GenericArray arr = node.GetArray();
		if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat())
			return false;

		float m00 = arr[0].GetFloat();
		float m01 = arr[1].GetFloat();
		float m10 = arr[2].GetFloat();
		float m11 = arr[3].GetFloat();

		glm::mat2 result;
		result[0][0] = m00;
		result[0][1] = m01;
		result[1][0] = m10;
		result[1][1] = m11;

		if (outMatrix)
			*outMatrix = result;

		return true;
	}

	static bool JSONToMat3(const rapidjson::Value &node, glm::mat3 *outMatrix)
	{
		if (!node.IsArray())
			return false;

		rapidjson::GenericArray arr = node.GetArray();
		if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat()
		 || !arr[3].IsFloat() || !arr[4].IsFloat() || !arr[5].IsFloat()
		 || !arr[6].IsFloat() || !arr[7].IsFloat() || !arr[8].IsFloat())
			return false;

		float m00 = arr[0].GetFloat();
		float m01 = arr[1].GetFloat();
		float m02 = arr[2].GetFloat();

		float m10 = arr[3].GetFloat();
		float m11 = arr[4].GetFloat();
		float m12 = arr[5].GetFloat();
		
		float m20 = arr[6].GetFloat();
		float m21 = arr[7].GetFloat();
		float m22 = arr[8].GetFloat();

		glm::mat3 result;
		result[0][0] = m00;
		result[0][1] = m01;
		result[0][2] = m02;
		
		result[1][0] = m10;
		result[1][1] = m11;
		result[1][2] = m12;

		result[2][0] = m20;
		result[2][1] = m21;
		result[2][2] = m22;

		if (outMatrix)
			*outMatrix = result;

		return true;
	}

	static bool JSONToMat4(const rapidjson::Value &node, glm::mat4 *outMatrix)
	{
		if (!node.IsArray())
			return false;

		rapidjson::GenericArray arr = node.GetArray();
		if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat()
		 || !arr[4].IsFloat() || !arr[5].IsFloat() || !arr[6].IsFloat() || !arr[7].IsFloat()
		 || !arr[8].IsFloat() || !arr[9].IsFloat() || !arr[10].IsFloat() || !arr[11].IsFloat()
		 || !arr[12].IsFloat() || !arr[13].IsFloat() || !arr[14].IsFloat() || !arr[15].IsFloat())
			return false;

		float m00 = arr[0].GetFloat();
		float m01 = arr[1].GetFloat();
		float m02 = arr[2].GetFloat();
		float m03 = arr[3].GetFloat();

		float m10 = arr[4].GetFloat();
		float m11 = arr[5].GetFloat();
		float m12 = arr[6].GetFloat();
		float m13 = arr[7].GetFloat();

		float m20 = arr[8].GetFloat();
		float m21 = arr[9].GetFloat();
		float m22 = arr[10].GetFloat();
		float m23 = arr[11].GetFloat();
		
		float m30 = arr[12].GetFloat();
		float m31 = arr[13].GetFloat();
		float m32 = arr[14].GetFloat();
		float m33 = arr[15].GetFloat();

		glm::mat4 result;
		result[0][0] = m00;
		result[0][1] = m01;
		result[0][2] = m02;
		result[0][3] = m03;

		result[1][0] = m10;
		result[1][1] = m11;
		result[1][2] = m12;
		result[1][3] = m13;

		result[2][0] = m20;
		result[2][1] = m21;
		result[2][2] = m22;
		result[2][3] = m23;

		result[3][0] = m30;
		result[3][1] = m31;
		result[3][2] = m32;
		result[3][3] = m33;

		if (outMatrix)
			*outMatrix = result;

		return true;
	}

	static bool JSONToQuat(const rapidjson::Value &node, glm::quat *outQuaternion)
	{
		if (!node.IsArray())
			return false;

		rapidjson::GenericArray arr = node.GetArray();
		if (!arr[0].IsFloat() || !arr[1].IsFloat() || !arr[2].IsFloat() || !arr[3].IsFloat())
			return false;

		float w = arr[0].GetFloat();
		float x = arr[1].GetFloat();
		float y = arr[2].GetFloat();
		float z = arr[3].GetFloat();

		if (outQuaternion)
			*outQuaternion = { w, x, y, z };

		return true;
	}
}

