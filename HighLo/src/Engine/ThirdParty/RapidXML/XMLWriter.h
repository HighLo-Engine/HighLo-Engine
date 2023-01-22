// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-10) initial release
//

/*
 * Here we got the following situation:
 * The XML format from the engine is in general type-safe, meaning the type is always known by the XMLWriter class.
 * This is done by writing the type name as the node name into the format, however we got one big exception:
 *
 * Matrices, Vectors and Quaternions are assumed to have always a float-datatype, because their node names are already taken by the values's location.
 * The future will show, if this assumption is true or if their will be a special case, where we would need to store a ivec4 for example to disk in a XML format...
 * Then the XML formatter would have to be re-visited.
 *
 * Current Vector format: (in this example a 2-dimensional vector)
 * <vector2 key="someKey">
 *   <x>20.2</x>
 *   <y>2242.2</y>
 * </vector2>
 *
 * The above format assumes that the nodes x, y hold a float value!
 *
 * Current matrix format: (in this example a 2x2 matrix)
 * <matrix2 key="someKey">
 *   <m00>434.5</m00>
 *   <m01>42.5</m01>
 *   <m10>53.5</m10>
 *   <m11>76.5</m11>
 * </matrix2>
 *
 * The node names are corresponding to the matrix position, m00 would be the 0th row and the 0th column, ...
 */

#pragma once

#include "Engine/Loaders/DocumentWriter.h"
#include "XMLHelper.h"

#include <rapidxml/rapidxml.hpp>

namespace highlo
{
	class XMLWriter : public DocumentWriter
	{
	public:

		XMLWriter(const FileSystemPath &filePath);
		virtual ~XMLWriter();

		virtual void BeginArray() override;
		virtual void EndArray(const HLString &key = "", bool rawData = false) override;

		virtual void BeginObject() override;
		virtual void EndObject(bool rawData = false) override;

		virtual bool WriteFloat(const HLString &key, float value) override;
		virtual bool WriteDouble(const HLString &key, double value) override;
		virtual bool WriteInt32(const HLString &key, int32 value) override;
		virtual bool WriteUInt32(const HLString &key, uint32 value) override;
		virtual bool WriteInt64(const HLString &key, int64 value) override;
		virtual bool WriteUInt64(const HLString &key, uint64 value) override;
		virtual bool WriteBool(const HLString &key, bool value) override;
		virtual bool WriteString(const HLString &key, const HLString &value) override;

		virtual bool WriteVec2(const HLString &key, const glm::vec2 &value) override;
		virtual bool WriteVec3(const HLString &key, const glm::vec3 &value) override;
		virtual bool WriteVec4(const HLString &key, const glm::vec4 &value) override;
		virtual bool WriteMat2(const HLString &key, const glm::mat2 &value) override;
		virtual bool WriteMat3(const HLString &key, const glm::mat3 &value) override;
		virtual bool WriteMat4(const HLString &key, const glm::mat4 &value) override;
		virtual bool WriteQuaternion(const HLString &key, const glm::quat &value) override;

		virtual bool WriteStringArray(const HLString &key, const std::vector<HLString> &value) override;
		virtual bool WriteInt32Array(const HLString &key, const std::vector<int32> &value) override;
		virtual bool WriteUInt32Array(const HLString &key, const std::vector<uint32> &value) override;
		virtual bool WriteInt64Array(const HLString &key, const std::vector<int64> &value) override;
		virtual bool WriteUInt64Array(const HLString &key, const std::vector<uint64> &value) override;
		virtual bool WriteBoolArray(const HLString &key, const std::vector<bool> &value) override;
		virtual bool WriteFloatArray(const HLString &key, const std::vector<float> &value) override;
		virtual bool WriteDoubleArray(const HLString &key, const std::vector<double> &value) override;

		virtual bool WriteVec2Array(const HLString &key, const std::vector<glm::vec2> &value) override;
		virtual bool WriteVec3Array(const HLString &key, const std::vector<glm::vec3> &value) override;
		virtual bool WriteVec4Array(const HLString &key, const std::vector<glm::vec4> &value) override;
		virtual bool WriteMat2Array(const HLString &key, const std::vector<glm::mat2> &value) override;
		virtual bool WriteMat3Array(const HLString &key, const std::vector<glm::mat3> &value) override;
		virtual bool WriteMat4Array(const HLString &key, const std::vector<glm::mat4> &value) override;
		virtual bool WriteQuaternionArray(const HLString &key, const std::vector<glm::quat> &value) override;

		virtual bool WriteStringArrayMap(const HLString &key, const std::map<HLString, HLString> &map) override;
		virtual bool WriteInt32ArrayMap(const HLString &key, const std::map<HLString, int32> &map) override;
		virtual bool WriteUInt32ArrayMap(const HLString &key, const std::map<HLString, uint32> &map) override;
		virtual bool WriteInt64ArrayMap(const HLString &key, const std::map<HLString, int64> &map) override;
		virtual bool WriteUInt64ArrayMap(const HLString &key, const std::map<HLString, uint64> &map) override;
		virtual bool WriteBoolArrayMap(const HLString &key, const std::map<HLString, bool> &map) override;
		virtual bool WriteFloatArrayMap(const HLString &key, const std::map<HLString, float> &map) override;
		virtual bool WriteDoubleArrayMap(const HLString &key, const std::map<HLString, double> &map) override;

		virtual bool WriteVec2ArrayMap(const HLString &key, const std::map<HLString, glm::vec2> &map) override;
		virtual bool WriteVec3ArrayMap(const HLString &key, const std::map<HLString, glm::vec3> &map) override;
		virtual bool WriteVec4ArrayMap(const HLString &key, const std::map<HLString, glm::vec4> &map) override;
		virtual bool WriteMat2ArrayMap(const HLString &key, const std::map<HLString, glm::mat2> &map) override;
		virtual bool WriteMat3ArrayMap(const HLString &key, const std::map<HLString, glm::mat3> &map) override;
		virtual bool WriteMat4ArrayMap(const HLString &key, const std::map<HLString, glm::mat4> &map) override;
		virtual bool WriteQuaternionArrayMap(const HLString &key, const std::map<HLString, glm::quat> &map) override;

		virtual bool WriteStringArrayMap(const HLString &key, const std::unordered_map<HLString, HLString> &map) override;
		virtual bool WriteInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, int32> &map) override;
		virtual bool WriteUInt32ArrayMap(const HLString &key, const std::unordered_map<HLString, uint32> &map) override;
		virtual bool WriteInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, int64> &map) override;
		virtual bool WriteUInt64ArrayMap(const HLString &key, const std::unordered_map<HLString, uint64> &map) override;
		virtual bool WriteBoolArrayMap(const HLString &key, const std::unordered_map<HLString, bool> &map) override;
		virtual bool WriteFloatArrayMap(const HLString &key, const std::unordered_map<HLString, float> &map) override;
		virtual bool WriteDoubleArrayMap(const HLString &key, const std::unordered_map<HLString, double> &map) override;

		virtual bool WriteVec2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec2> &map) override;
		virtual bool WriteVec3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec3> &map) override;
		virtual bool WriteVec4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::vec4> &map) override;
		virtual bool WriteMat2ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat2> &map) override;
		virtual bool WriteMat3ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat3> &map) override;
		virtual bool WriteMat4ArrayMap(const HLString &key, const std::unordered_map<HLString, glm::mat4> &map) override;
		virtual bool WriteQuaternionArrayMap(const HLString &key, const std::unordered_map<HLString, glm::quat> &map) override;

		virtual bool HasKey(const HLString &key) const override;
		virtual bool WriteOut() override;
		virtual HLString GetContent(bool prettify = false) override;
		virtual void SetContent(const HLString &content) override;

	private:

		bool HasKeyInternal(rapidxml::xml_node<> *parent, const HLString &key) const;
		bool Write(const HLString &key, DocumentDataType type, const std::function<rapidxml::xml_node<> *()> &insertFunc);

		FileSystemPath m_FilePath;
		rapidxml::xml_document<> m_Document;
		rapidxml::xml_node<> *m_RootNode = nullptr;
		HLString m_EngineVersion;

		std::vector<rapidxml::xml_node<> *> m_TempBuffers;
		rapidxml::xml_node<> *m_TempBuffer = nullptr;

		bool m_ShouldWriteIntoArray = false;
		bool m_ShouldWriteIntoObject = false;
	};
}
