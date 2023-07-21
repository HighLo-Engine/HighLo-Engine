// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "XMLReader.h"

#include "rapidxml_ext.hpp"

#include "XMLHelper.h"
#include "Engine/Application/Application.h"
#include "Engine/Utils/LoaderUtils.h"
#include "Engine/Core/FileSystem.h"

#define XML_LOG_PREFIX "XMLReader>    "

namespace highlo
{
	XMLReader::XMLReader(const FileSystemPath &filePath)
		: m_FilePath(filePath)
	{
		m_EngineVersion = HLApplication::Get().GetApplicationSettings().Version;
	}
	
	XMLReader::~XMLReader()
	{
	}
	
	bool XMLReader::ReadFloat(const HLString &key, float *result)
	{
		return Read(key, DocumentDataType::Float, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			float value = utils::LexicalCast<float, char*>(node->value());
			*result = value;
			return true;
		});
	}
	
	bool XMLReader::ReadDouble(const HLString &key, double *result)
	{
		return Read(key, DocumentDataType::Double, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			double value = utils::LexicalCast<double, char*>(node->value());
			*result = value;
			return true;
		});
	}
	
	bool XMLReader::ReadInt32(const HLString &key, int32 *result)
	{
		return Read(key, DocumentDataType::Int32, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			int32 value = utils::LexicalCast<int32, char*>(node->value());
			*result = value;
			return true;
		});
	}
	
	bool XMLReader::ReadUInt32(const HLString &key, uint32 *result)
	{
		return Read(key, DocumentDataType::UInt32, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			uint32 value = utils::LexicalCast<uint32, char*>(node->value());
			*result = value;
			return true;
		});
	}
	
	bool XMLReader::ReadInt64(const HLString &key, int64 *result)
	{
		return Read(key, DocumentDataType::Int64, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			int64 value = utils::LexicalCast<int64, char*>(node->value());
			*result = value;
			return true;
		});
	}
	
	bool XMLReader::ReadUInt64(const HLString &key, uint64 *result)
	{
		return Read(key, DocumentDataType::UInt64, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			uint64 value = utils::LexicalCast<uint64, char*>(node->value());
			*result = value;
			return true;
		});
	}
	
	bool XMLReader::ReadBool(const HLString &key, bool *result)
	{
		return Read(key, DocumentDataType::Bool, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			bool value = utils::LexicalCast<bool, char*>(node->value());
			*result = value;
			return true;
		});
	}
	
	bool XMLReader::ReadString(const HLString &key, HLString *result)
	{
		return Read(key, DocumentDataType::String, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			*result = node->value();
			return true;
		});
	}
	
	bool XMLReader::ReadVector2(const HLString &key, glm::vec2 *result)
	{
		return Read(key, DocumentDataType::Vec2, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			glm::vec2 v;
			if (utils::XmlNodeToVec2(node, &v))
			{
				*result = v;
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadVector3(const HLString &key, glm::vec3 *result)
	{
		return Read(key, DocumentDataType::Vec3, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			glm::vec3 v;
			if (utils::XmlNodeToVec3(node, &v))
			{
				*result = v;
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadVector4(const HLString &key, glm::vec4 *result)
	{
		return Read(key, DocumentDataType::Vec4, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			glm::vec4 v;
			if (utils::XmlNodeToVec4(node, &v))
			{
				*result = v;
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMatrix2(const HLString &key, glm::mat2 *result)
	{
		return Read(key, DocumentDataType::Mat2, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			glm::mat2 v;
			if (utils::XmlNodeToMat2(node, &v))
			{
				*result = v;
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMatrix3(const HLString &key, glm::mat3 *result)
	{
		return Read(key, DocumentDataType::Mat3, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			glm::mat3 v;
			if (utils::XmlNodeToMat3(node, &v))
			{
				*result = v;
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMatrix4(const HLString &key, glm::mat4 *result)
	{
		return Read(key, DocumentDataType::Mat4, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			glm::mat4 v;
			if (utils::XmlNodeToMat4(node, &v))
			{
				*result = v;
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadQuaternion(const HLString &key, glm::quat *result)
	{
		return Read(key, DocumentDataType::Quat, [result](rapidxml::xml_node<> *node) -> bool
		{
			if (!result)
				return false;

			glm::quat v;
			if (utils::XmlNodeToQuat(node, &v))
			{
				*result = v;
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadStringArray(const HLString &key, std::vector<HLString> &result)
	{
		return ReadArray(key, DocumentDataType::String, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			result.push_back(node->value());
			return true;
		});
	}
	
	bool XMLReader::ReadInt32Array(const HLString &key, std::vector<int32> &result)
	{
		return ReadArray(key, DocumentDataType::Int32, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			int32 value = utils::LexicalCast<int32, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}
	
	bool XMLReader::ReadUInt32Array(const HLString &key, std::vector<uint32> &result)
	{
		return ReadArray(key, DocumentDataType::UInt32, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			uint32 value = utils::LexicalCast<uint32, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}
	
	bool XMLReader::ReadInt64Array(const HLString &key, std::vector<int64> &result)
	{
		return ReadArray(key, DocumentDataType::Int64, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			int64 value = utils::LexicalCast<int64, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}
	
	bool XMLReader::ReadUInt64Array(const HLString &key, std::vector<uint64> &result)
	{
		return ReadArray(key, DocumentDataType::UInt64, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			uint64 value = utils::LexicalCast<uint64, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}
	
	bool XMLReader::ReadBoolArray(const HLString &key, std::vector<bool> &result)
	{
		return ReadArray(key, DocumentDataType::Bool, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			bool value = utils::LexicalCast<bool, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}
	
	bool XMLReader::ReadFloatArray(const HLString &key, std::vector<float> &result)
	{
		return ReadArray(key, DocumentDataType::Float, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			float value = utils::LexicalCast<float, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}
	
	bool XMLReader::ReadDoubleArray(const HLString &key, std::vector<double> &result)
	{
		return ReadArray(key, DocumentDataType::Double, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			double value = utils::LexicalCast<double, char*>(node->value());
			result.push_back(value);
			return true;
		});
	}
	
	bool XMLReader::ReadVec2Array(const HLString &key, std::vector<glm::vec2> &result)
	{
		return ReadArray(key, DocumentDataType::Vec2, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			glm::vec2 v;
			if (utils::XmlNodeToVec2(node, &v))
			{
				result.push_back(v);
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadVec3Array(const HLString &key, std::vector<glm::vec3> &result)
	{
		return ReadArray(key, DocumentDataType::Vec3, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			glm::vec3 v;
			if (utils::XmlNodeToVec3(node, &v))
			{
				result.push_back(v);
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadVec4Array(const HLString &key, std::vector<glm::vec4> &result)
	{
		return ReadArray(key, DocumentDataType::Vec4, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			glm::vec4 v;
			if (utils::XmlNodeToVec4(node, &v))
			{
				result.push_back(v);
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMat2Array(const HLString &key, std::vector<glm::mat2> &result)
	{
		return ReadArray(key, DocumentDataType::Mat2, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			glm::mat2 v;
			if (utils::XmlNodeToMat2(node, &v))
			{
				result.push_back(v);
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMat3Array(const HLString &key, std::vector<glm::mat3> &result)
	{
		return ReadArray(key, DocumentDataType::Mat3, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			glm::mat3 v;
			if (utils::XmlNodeToMat3(node, &v))
			{
				result.push_back(v);
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMat4Array(const HLString &key, std::vector<glm::mat4> &result)
	{
		return ReadArray(key, DocumentDataType::Mat4, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			glm::mat4 v;
			if (utils::XmlNodeToMat4(node, &v))
			{
				result.push_back(v);
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadQuatArray(const HLString &key, std::vector<glm::quat> &result)
	{
		return ReadArray(key, DocumentDataType::Quat, [&result](rapidxml::xml_node<> *node) -> bool
		{
			if (!node->value())
				return false;

			glm::quat v;
			if (utils::XmlNodeToQuat(node, &v))
			{
				result.push_back(v);
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadStringArrayMap(const HLString &key, std::map<HLString, HLString> &result)
	{
		return ReadArrayMap(key, DocumentDataType::String, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			result.insert({ k, node->value() });
			return true;
		});
	}
	
	bool XMLReader::ReadInt32ArrayMap(const HLString &key, std::map<HLString, int32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int32, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			int32 value = utils::LexicalCast<int32, char*>(node->value());
			result.insert({ k, value });
			return true;
		});
	}
	
	bool XMLReader::ReadUInt32ArrayMap(const HLString &key, std::map<HLString, uint32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt32, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			uint32 value = utils::LexicalCast<uint32, char*>(node->value());
			result.insert({ k, value });
			return true;
		});
	}
	
	bool XMLReader::ReadInt64ArrayMap(const HLString &key, std::map<HLString, int64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Int64, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			int64 value = utils::LexicalCast<int64, char*>(node->value());
			result.insert({ k, value });
			return true;
		});
	}
	
	bool XMLReader::ReadUInt64ArrayMap(const HLString &key, std::map<HLString, uint64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::UInt64, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			uint64 value = utils::LexicalCast<uint64, char*>(node->value());
			result.insert({ k, value });
			return true;
		});
	}
	
	bool XMLReader::ReadBoolArrayMap(const HLString &key, std::map<HLString, bool> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Bool, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			bool value = utils::LexicalCast<bool, char*>(node->value());
			result.insert({ k, value });
			return true;
		});
	}
	
	bool XMLReader::ReadFloatArrayMap(const HLString &key, std::map<HLString, float> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Float, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			float value = utils::LexicalCast<float, char*>(node->value());
			result.insert({ k, value });
			return true;
		});
	}
	
	bool XMLReader::ReadDoubleArrayMap(const HLString &key, std::map<HLString, double> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Double, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			double value = utils::LexicalCast<double, char*>(node->value());
			result.insert({ k, value });
			return true;
		});
	}
	
	bool XMLReader::ReadVec2ArrayMap(const HLString &key, std::map<HLString, glm::vec2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::vec2 v;
			if (utils::XmlNodeToVec2(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadVec3ArrayMap(const HLString &key, std::map<HLString, glm::vec3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec3, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::vec3 v;
			if (utils::XmlNodeToVec3(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadVec4ArrayMap(const HLString &key, std::map<HLString, glm::vec4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec4, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::vec4 v;
			if (utils::XmlNodeToVec4(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMat2ArrayMap(const HLString &key, std::map<HLString, glm::mat2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::mat2 v;
			if (utils::XmlNodeToMat2(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMat3ArrayMap(const HLString &key, std::map<HLString, glm::mat3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat3, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::mat3 v;
			if (utils::XmlNodeToMat3(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadMat4ArrayMap(const HLString &key, std::map<HLString, glm::mat4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat4, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::mat4 v;
			if (utils::XmlNodeToMat4(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadQuatArrayMap(const HLString &key, std::map<HLString, glm::quat> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Quat, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::quat v;
			if (utils::XmlNodeToQuat(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}

	bool XMLReader::ReadStringArrayMap(const HLString &key, std::unordered_map<HLString, HLString> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			result.insert({ k, node->value() });
			return false;
		});
	}

	bool XMLReader::ReadInt32ArrayMap(const HLString &key, std::unordered_map<HLString, int32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			int32 value = utils::LexicalCast<int32, char*>(node->value());
			result.insert({ k, value });
			return false;
		});
	}

	bool XMLReader::ReadUInt32ArrayMap(const HLString &key, std::unordered_map<HLString, uint32> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			uint32 value = utils::LexicalCast<uint32, char*>(node->value());
			result.insert({ k, value });
			return false;
		});
	}

	bool XMLReader::ReadInt64ArrayMap(const HLString &key, std::unordered_map<HLString, int64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			int64 value = utils::LexicalCast<int64, char*>(node->value());
			result.insert({ k, value });
			return false;
		});
	}

	bool XMLReader::ReadUInt64ArrayMap(const HLString &key, std::unordered_map<HLString, uint64> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			uint64 value = utils::LexicalCast<uint64, char*>(node->value());
			result.insert({ k, value });
			return false;
		});
	}

	bool XMLReader::ReadBoolArrayMap(const HLString &key, std::unordered_map<HLString, bool> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			bool value = utils::LexicalCast<bool, char*>(node->value());
			result.insert({ k, value });
			return false;
		});
	}

	bool XMLReader::ReadFloatArrayMap(const HLString &key, std::unordered_map<HLString, float> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			float value = utils::LexicalCast<float, char*>(node->value());
			result.insert({ k, value });
			return false;
		});
	}

	bool XMLReader::ReadDoubleArrayMap(const HLString &key, std::unordered_map<HLString, double> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			double value = utils::LexicalCast<double, char*>(node->value());
			result.insert({ k, value });
			return false;
		});
	}

	bool XMLReader::ReadVec2ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::vec2 v;
			if (utils::XmlNodeToVec2(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}

	bool XMLReader::ReadVec3ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec3, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::vec3 v;
			if (utils::XmlNodeToVec3(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}

	bool XMLReader::ReadVec4ArrayMap(const HLString &key, std::unordered_map<HLString, glm::vec4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Vec4, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::vec4 v;
			if (utils::XmlNodeToVec4(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}

	bool XMLReader::ReadMat2ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat2> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat2, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::mat2 v;
			if (utils::XmlNodeToMat2(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}

	bool XMLReader::ReadMat3ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat3> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat3, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::mat3 v;
			if (utils::XmlNodeToMat3(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}

	bool XMLReader::ReadMat4ArrayMap(const HLString &key, std::unordered_map<HLString, glm::mat4> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Mat4, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::mat4 v;
			if (utils::XmlNodeToMat4(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}

	bool XMLReader::ReadQuatArrayMap(const HLString &key, std::unordered_map<HLString, glm::quat> &result)
	{
		return ReadArrayMap(key, DocumentDataType::Quat, [&result](HLString &k, rapidxml::xml_node<> *node) -> bool
		{
			if (!node)
				return false;

			glm::quat v;
			if (utils::XmlNodeToQuat(node, &v))
			{
				result.insert({ k, v });
				return true;
			}

			return false;
		});
	}
	
	bool XMLReader::ReadContents(const FileSystemPath &filePath)
	{
		if (!filePath.String().IsEmpty())
			m_FilePath = filePath;

		if (FileSystem::Get()->FileExists(m_FilePath))
		{
			HL_CORE_INFO(XML_LOG_PREFIX "[+] Loaded {0} [+]", **m_FilePath);
			HLString content = FileSystem::Get()->ReadTextFile(m_FilePath);
			if (content.IsEmpty())
				return false;

			char *str = utils::PrepareString(m_Document, content);
			m_Document.parse<0>(str);

			rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
			if (rootNode)
			{
				rapidxml::xml_attribute<> *versionAttr = rootNode->first_attribute("version");
				if (!versionAttr)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: XML format is wrong! Expected version attribute to be present in HighLo node! [-]");
				}
				else
				{
					HLString versionStr = versionAttr->value();
					if (versionStr != m_EngineVersion)
					{
						// TODO: if we change the XML structure in the future we can parse old formats here to keep the parser backward compatible
						HL_CORE_WARN(XML_LOG_PREFIX "[-] The config file {0} is outdated! Going to re-format the file... [-]", **m_FilePath);
					}
				}
			}

			return true;
		}
		else
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: File {0} not found! [-]", **m_FilePath);
		}

		return false;
	}
	
	HLString XMLReader::GetContent(bool prettify)
	{
		m_Document.append_node(m_RootNode);

		int32 flags = 0;
		if (!prettify)
			flags = rapidxml::print_no_indenting;

		std::string s;
		rapidxml::print(std::back_inserter(s), m_Document, flags);
		return s.c_str();
	}

	void XMLReader::SetContent(const HLString &content)
	{
		char *str = utils::PrepareString(m_Document, content);
		m_Document.parse<0>(str);

		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo"); // Our XML format should always have a root node named "HighLo"
		if (rootNode)
		{
			// Check the version string of the root node, that indicates the engine version
			rapidxml::xml_attribute<> *versionAttr = rootNode->first_attribute("version");
			if (!versionAttr)
			{
				HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: XML format is wrong! Expected version attribute to be present in HighLo node! [-]");
			}
			else
			{
				HLString versionStr = versionAttr->value();
				if (versionStr != m_EngineVersion)
				{
					HL_CORE_WARN(XML_LOG_PREFIX "[-] The config file {0} is outdated! Going to re-format the file... [-]", **m_FilePath);
				}
			}
		}
	}
	bool XMLReader::Read(const HLString &key, DocumentDataType type, const std::function<bool(rapidxml::xml_node<> *)> &insertFunc)
	{
		bool anyItemFound = false;
		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
		if (!rootNode)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error:  [-]");
			return false;
		}

		HLString typeStr = utils::DocumentDataTypeToString(type);
		for (rapidxml::xml_node<> *it = rootNode->first_node(*typeStr); it; it = it->next_sibling(*typeStr))
		{
			if (key.IsEmpty())
			{
				bool result = insertFunc(it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}

				continue;
			}

			// Here we can assume that the user needs a specifc key attribute
			rapidxml::xml_attribute<> *keyAttribute = it->first_attribute("key");
			if (!keyAttribute)
			{
				HL_CORE_ERROR("[-] Error: Expected to find any key attribute with the value {0} [-]", *key);
				return false;
			}

			HLString k = keyAttribute->value();
			if (k == key)
			{
				bool result = insertFunc(it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}
			}
		}

		return anyItemFound;
	}

	bool XMLReader::ReadArray(const HLString &key, DocumentDataType type, const std::function<bool(rapidxml::xml_node<> *)> &insertFunc)
	{
		bool anyItemFound = false;
		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
		if (!rootNode)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find a root node named HighLo! [-]");
			return false;
		}

		if (key.IsEmpty())
		{
			rapidxml::xml_node<> *firstNode = rootNode->first_node("array");
			if (!firstNode)
			{
				HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find any array node! [-]");
				return false;
			}

			// Now find any nodes of the type
			HLString typeStr = utils::DocumentDataTypeToString(type);
			for (rapidxml::xml_node<> *it = firstNode->first_node(*typeStr); it; it = it->next_sibling(*typeStr))
			{
				bool result = insertFunc(it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}
			}
		}
		else
		{
			for (rapidxml::xml_node<> *it = rootNode->first_node("array"); it; it = it->next_sibling("array"))
			{
				rapidxml::xml_attribute<> *arrayKey = it->first_attribute("key");
				if (!arrayKey)
				{
					// This array must be wrong, it has no key!
					continue;
				}

				HLString arrayKeyStr = arrayKey->value();
				if (arrayKeyStr == key)
				{
					// Now iterate over the value type and call the user defined conversion function
					HLString typeStr = utils::DocumentDataTypeToString(type);
					for (rapidxml::xml_node<> *typeIt = it->first_node(*typeStr); typeIt; typeIt = typeIt->next_sibling(*typeStr))
					{
						bool result = insertFunc(typeIt);
						if (!result)
						{
							HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
							return false;
						}
						else
						{
							anyItemFound = true;
						}
					}
				}
			}
		}

		return anyItemFound;
	}
	
	bool XMLReader::ReadArrayMap(const HLString &key, DocumentDataType type, const std::function<bool(HLString&, rapidxml::xml_node<>*)> &insertFunc)
	{
		bool anyItemFound = false;
		rapidxml::xml_node<> *rootNode = m_Document.first_node("HighLo");
		if (!rootNode)
		{
			HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find a root node named HighLo! [-]");
			return false;
		}

		if (key.IsEmpty())
		{
			rapidxml::xml_node<> *firstNode = rootNode->first_node("arraymap");
			if (!firstNode)
			{
				HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected to find any arraymap node! [-]");
				return false;
			}

			// Now find any nodes of the type
			HLString typeStr = utils::DocumentDataTypeToString(type);
			for (rapidxml::xml_node<> *it = firstNode->first_node(*typeStr); it; it = it->next_sibling(*typeStr))
			{
				rapidxml::xml_attribute<> *keyAttribute = it->first_attribute("key");
				if (!keyAttribute)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected a key attribute on node {0} [-]", it->name());
					return false;
				}

				HLString k = keyAttribute->value();
				bool result = insertFunc(k, it);
				if (!result)
				{
					HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
					return false;
				}
				else
				{
					anyItemFound = true;
				}
			}
		}
		else
		{
			for (rapidxml::xml_node<> *it = rootNode->first_node("arraymap"); it; it = it->next_sibling("arraymap"))
			{
				rapidxml::xml_attribute<> *arrayMapKey = it->first_attribute("key");
				if (!arrayMapKey)
				{
					// This arraymap must be wrong, it has no key!
					continue;
				}

				HLString arrayMapKeyStr = arrayMapKey->value();
				if (arrayMapKeyStr == key)
				{
					// Now iterate over the value type and call the user defined conversion function
					HLString typeStr = utils::DocumentDataTypeToString(type);
					for (rapidxml::xml_node<> *typeIt = it->first_node(*typeStr); typeIt; typeIt = typeIt->next_sibling(*typeStr))
					{
						rapidxml::xml_attribute<> *keyAttribute = typeIt->first_attribute("key");
						if (!keyAttribute)
						{
							HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Expected a key attribute on node {0} [-]", typeIt->name());
							return false;
						}

						HLString k = keyAttribute->value();
						bool result = insertFunc(k, typeIt);
						if (!result)
						{
							HL_CORE_ERROR(XML_LOG_PREFIX "[-] Error: Unexpected Parsing error in type specific function! [-]");
							return false;
						}
						else
						{
							anyItemFound = true;
						}
					}
				}
			}
		}

		return anyItemFound;
	}
}

