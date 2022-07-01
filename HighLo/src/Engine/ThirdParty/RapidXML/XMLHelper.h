// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-13) initial release
//

#pragma once

#include "Engine/Utils/StringUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

#include <rapidxml/rapidxml.hpp>

namespace highlo::utils
{
	static char *PrepareString(rapidxml::xml_document<> &doc, const HLString &str)
	{
		char *result = doc.allocate_string(str.C_Str(), str.Length() + 1);
		result[str.Length()] = '\0';
		return result;
	}

	static rapidxml::xml_node<> *Vec2ToXmlNode(rapidxml::xml_document<> &doc, const glm::vec2 &v)
	{
		HLString valueXStr = HLString::ToString(v.x);
		HLString valueYStr = HLString::ToString(v.y);

		char *x = PrepareString(doc, valueXStr);
		char *y = PrepareString(doc, valueYStr);

		rapidxml::xml_node<> *val1 = doc.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *val2 = doc.allocate_node(rapidxml::node_element, "y", y);

		rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "vec2");
		node->append_node(val1);
		node->append_node(val2);
		return node;
	}

	static rapidxml::xml_node<> *Vec3ToXmlNode(rapidxml::xml_document<> &doc, const glm::vec3 &v)
	{
		HLString valueXStr = HLString::ToString(v.x);
		HLString valueYStr = HLString::ToString(v.y);
		HLString valueZStr = HLString::ToString(v.z);

		char *x = PrepareString(doc, valueXStr);
		char *y = PrepareString(doc, valueYStr);
		char *z = PrepareString(doc, valueZStr);

		rapidxml::xml_node<> *valueX = doc.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *valueY = doc.allocate_node(rapidxml::node_element, "y", y);
		rapidxml::xml_node<> *valueZ = doc.allocate_node(rapidxml::node_element, "z", z);

		rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "vec3");
		node->append_node(valueX);
		node->append_node(valueY);
		node->append_node(valueZ);
		return node;
	}

	static rapidxml::xml_node<> *Vec4ToXmlNode(rapidxml::xml_document<> &doc, const glm::vec4 &v)
	{
		HLString valueXStr = HLString::ToString(v.x);
		HLString valueYStr = HLString::ToString(v.y);
		HLString valueZStr = HLString::ToString(v.z);
		HLString valueWStr = HLString::ToString(v.w);

		char *x = PrepareString(doc, valueXStr);
		char *y = PrepareString(doc, valueYStr);
		char *z = PrepareString(doc, valueZStr);
		char *w = PrepareString(doc, valueWStr);

		rapidxml::xml_node<> *valueX = doc.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *valueY = doc.allocate_node(rapidxml::node_element, "y", y);
		rapidxml::xml_node<> *valueZ = doc.allocate_node(rapidxml::node_element, "z", z);
		rapidxml::xml_node<> *valueW = doc.allocate_node(rapidxml::node_element, "w", w);

		rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "vec4");
		node->append_node(valueX);
		node->append_node(valueY);
		node->append_node(valueZ);
		node->append_node(valueW);
		return node;
	}

	static rapidxml::xml_node<> *QuatToXmlNode(rapidxml::xml_document<> &doc, const glm::quat &q)
	{
		HLString valueXStr = HLString::ToString(q.x);
		HLString valueYStr = HLString::ToString(q.y);
		HLString valueZStr = HLString::ToString(q.z);
		HLString valueWStr = HLString::ToString(q.w);

		char *x = PrepareString(doc, valueXStr);
		char *y = PrepareString(doc, valueYStr);
		char *z = PrepareString(doc, valueZStr);
		char *w = PrepareString(doc, valueWStr);

		rapidxml::xml_node<> *valueX = doc.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *valueY = doc.allocate_node(rapidxml::node_element, "y", y);
		rapidxml::xml_node<> *valueZ = doc.allocate_node(rapidxml::node_element, "z", z);
		rapidxml::xml_node<> *valueW = doc.allocate_node(rapidxml::node_element, "w", w);

		rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "quat");
		node->append_node(valueW);
		node->append_node(valueX);
		node->append_node(valueY);
		node->append_node(valueZ);
		return node;
	}

	static rapidxml::xml_node<> *Mat2ToXmlNode(rapidxml::xml_document<> &doc, const glm::mat2 &m)
	{
		HLString valueM00Str = HLString::ToString(m[0][0]);
		HLString valueM01Str = HLString::ToString(m[0][1]);
		HLString valueM10Str = HLString::ToString(m[1][0]);
		HLString valueM11Str = HLString::ToString(m[1][1]);

		char *m00 = PrepareString(doc, valueM00Str);
		char *m01 = PrepareString(doc, valueM01Str);
		char *m10 = PrepareString(doc, valueM10Str);
		char *m11 = PrepareString(doc, valueM11Str);

		rapidxml::xml_node<> *valueM00 = doc.allocate_node(rapidxml::node_element, "m00", m00);
		rapidxml::xml_node<> *valueM01 = doc.allocate_node(rapidxml::node_element, "m01", m01);
		rapidxml::xml_node<> *valueM10 = doc.allocate_node(rapidxml::node_element, "m10", m10);
		rapidxml::xml_node<> *valueM11 = doc.allocate_node(rapidxml::node_element, "m11", m11);

		rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "mat2");
		node->append_node(valueM00);
		node->append_node(valueM01);
		node->append_node(valueM10);
		node->append_node(valueM11);
		return node;
	}

	static rapidxml::xml_node<> *Mat3ToXmlNode(rapidxml::xml_document<> &doc, const glm::mat3 &m, const HLString &nodeName = "")
	{
		HLString valueM00Str = HLString::ToString(m[0][0]);
		HLString valueM01Str = HLString::ToString(m[0][1]);
		HLString valueM02Str = HLString::ToString(m[0][2]);
		HLString valueM10Str = HLString::ToString(m[1][0]);
		HLString valueM11Str = HLString::ToString(m[1][1]);
		HLString valueM12Str = HLString::ToString(m[1][2]);
		HLString valueM20Str = HLString::ToString(m[2][0]);
		HLString valueM21Str = HLString::ToString(m[2][1]);
		HLString valueM22Str = HLString::ToString(m[2][2]);

		char *m00 = PrepareString(doc, valueM00Str);
		char *m01 = PrepareString(doc, valueM01Str);
		char *m02 = PrepareString(doc, valueM02Str);

		char *m10 = PrepareString(doc, valueM10Str);
		char *m11 = PrepareString(doc, valueM11Str);
		char *m12 = PrepareString(doc, valueM12Str);

		char *m20 = PrepareString(doc, valueM20Str);
		char *m21 = PrepareString(doc, valueM21Str);
		char *m22 = PrepareString(doc, valueM22Str);

		rapidxml::xml_node<> *valueM00 = doc.allocate_node(rapidxml::node_element, "m00", m00);
		rapidxml::xml_node<> *valueM01 = doc.allocate_node(rapidxml::node_element, "m01", m01);
		rapidxml::xml_node<> *valueM02 = doc.allocate_node(rapidxml::node_element, "m02", m02);
		rapidxml::xml_node<> *valueM10 = doc.allocate_node(rapidxml::node_element, "m10", m10);
		rapidxml::xml_node<> *valueM11 = doc.allocate_node(rapidxml::node_element, "m11", m11);
		rapidxml::xml_node<> *valueM12 = doc.allocate_node(rapidxml::node_element, "m12", m12);
		rapidxml::xml_node<> *valueM20 = doc.allocate_node(rapidxml::node_element, "m20", m20);
		rapidxml::xml_node<> *valueM21 = doc.allocate_node(rapidxml::node_element, "m21", m21);
		rapidxml::xml_node<> *valueM22 = doc.allocate_node(rapidxml::node_element, "m22", m22);

		rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "mat3");
		node->append_node(valueM00);
		node->append_node(valueM01);
		node->append_node(valueM02);
		node->append_node(valueM10);
		node->append_node(valueM11);
		node->append_node(valueM12);
		node->append_node(valueM20);
		node->append_node(valueM21);
		node->append_node(valueM22);
		return node;
	}

	static rapidxml::xml_node<> *Mat4ToXmlNode(rapidxml::xml_document<> &doc, const glm::mat4 &m, const HLString &nodeName = "")
	{
		HLString valueM00Str = HLString::ToString(m[0][0]);
		HLString valueM01Str = HLString::ToString(m[0][1]);
		HLString valueM02Str = HLString::ToString(m[0][2]);
		HLString valueM03Str = HLString::ToString(m[0][3]);

		HLString valueM10Str = HLString::ToString(m[1][0]);
		HLString valueM11Str = HLString::ToString(m[1][1]);
		HLString valueM12Str = HLString::ToString(m[1][2]);
		HLString valueM13Str = HLString::ToString(m[1][3]);

		HLString valueM20Str = HLString::ToString(m[2][0]);
		HLString valueM21Str = HLString::ToString(m[2][1]);
		HLString valueM22Str = HLString::ToString(m[2][2]);
		HLString valueM23Str = HLString::ToString(m[2][3]);

		HLString valueM30Str = HLString::ToString(m[3][0]);
		HLString valueM31Str = HLString::ToString(m[3][1]);
		HLString valueM32Str = HLString::ToString(m[3][2]);
		HLString valueM33Str = HLString::ToString(m[3][3]);

		char *m00 = PrepareString(doc, valueM00Str);
		char *m01 = PrepareString(doc, valueM01Str);
		char *m02 = PrepareString(doc, valueM02Str);
		char *m03 = PrepareString(doc, valueM03Str);

		char *m10 = PrepareString(doc, valueM10Str);
		char *m11 = PrepareString(doc, valueM11Str);
		char *m12 = PrepareString(doc, valueM12Str);
		char *m13 = PrepareString(doc, valueM13Str);

		char *m20 = PrepareString(doc, valueM20Str);
		char *m21 = PrepareString(doc, valueM21Str);
		char *m22 = PrepareString(doc, valueM22Str);
		char *m23 = PrepareString(doc, valueM23Str);

		char *m30 = PrepareString(doc, valueM30Str);
		char *m31 = PrepareString(doc, valueM31Str);
		char *m32 = PrepareString(doc, valueM32Str);
		char *m33 = PrepareString(doc, valueM33Str);

		rapidxml::xml_node<> *valueM00 = doc.allocate_node(rapidxml::node_element, "m00", m00);
		rapidxml::xml_node<> *valueM01 = doc.allocate_node(rapidxml::node_element, "m01", m01);
		rapidxml::xml_node<> *valueM02 = doc.allocate_node(rapidxml::node_element, "m02", m02);
		rapidxml::xml_node<> *valueM03 = doc.allocate_node(rapidxml::node_element, "m03", m03);

		rapidxml::xml_node<> *valueM10 = doc.allocate_node(rapidxml::node_element, "m10", m10);
		rapidxml::xml_node<> *valueM11 = doc.allocate_node(rapidxml::node_element, "m11", m11);
		rapidxml::xml_node<> *valueM12 = doc.allocate_node(rapidxml::node_element, "m12", m12);
		rapidxml::xml_node<> *valueM13 = doc.allocate_node(rapidxml::node_element, "m13", m13);

		rapidxml::xml_node<> *valueM20 = doc.allocate_node(rapidxml::node_element, "m20", m20);
		rapidxml::xml_node<> *valueM21 = doc.allocate_node(rapidxml::node_element, "m21", m21);
		rapidxml::xml_node<> *valueM22 = doc.allocate_node(rapidxml::node_element, "m22", m22);
		rapidxml::xml_node<> *valueM23 = doc.allocate_node(rapidxml::node_element, "m23", m23);

		rapidxml::xml_node<> *valueM30 = doc.allocate_node(rapidxml::node_element, "m30", m30);
		rapidxml::xml_node<> *valueM31 = doc.allocate_node(rapidxml::node_element, "m31", m31);
		rapidxml::xml_node<> *valueM32 = doc.allocate_node(rapidxml::node_element, "m32", m32);
		rapidxml::xml_node<> *valueM33 = doc.allocate_node(rapidxml::node_element, "m33", m33);

		rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "mat4");
		node->append_node(valueM00);
		node->append_node(valueM01);
		node->append_node(valueM02);
		node->append_node(valueM03);
		node->append_node(valueM10);
		node->append_node(valueM11);
		node->append_node(valueM12);
		node->append_node(valueM13);
		node->append_node(valueM20);
		node->append_node(valueM21);
		node->append_node(valueM22);
		node->append_node(valueM23);
		node->append_node(valueM30);
		node->append_node(valueM31);
		node->append_node(valueM32);
		node->append_node(valueM33);
		return node;
	}

	static bool XmlNodeToVec2(const rapidxml::xml_node<> *node, glm::vec2 *outVector)
	{
		if (strcmp(node->name(), "vec2") != 0)
		{
			HL_CORE_ERROR("Error: wrong input node! expected input node to be named vec2, but got {0}", node->name());
			return false;
		}

		if (!outVector)
		{
			HL_CORE_ERROR("Error: Result pointer is nullptr!");
			return false;
		}

		rapidxml::xml_node<> *xNode = node->first_node();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0)
			|| (strcmp(yNode->name(), "y") != 0))
		{
			HL_CORE_ERROR("Error: Children nodes are not named x and y!");
			return false;
		}

		float x = utils::LexicalCast<float, char *>(xNode->value());
		float y = utils::LexicalCast<float, char *>(yNode->value());
		*outVector = { x, y };
		return true;
	}

	static bool XmlNodeToVec3(const rapidxml::xml_node<> *node, glm::vec3 *outVector)
	{
		if (strcmp(node->name(), "vec3") != 0)
			return false;

		if (!outVector)
			return false;

		rapidxml::xml_node<> *xNode = node->first_node();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();
		rapidxml::xml_node<> *zNode = yNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0)
			|| (strcmp(yNode->name(), "y") != 0)
			|| (strcmp(zNode->name(), "z") != 0))
			return false;

		float x = utils::LexicalCast<float, char *>(xNode->value());
		float y = utils::LexicalCast<float, char *>(yNode->value());
		float z = utils::LexicalCast<float, char *>(zNode->value());
		*outVector = { x, y, z };
		return true;
	}

	static bool XmlNodeToVec4(const rapidxml::xml_node<> *node, glm::vec4 *outVector)
	{
		if (strcmp(node->name(), "vec4") != 0)
			return false;

		if (!outVector)
			return false;

		rapidxml::xml_node<> *xNode = node->first_node();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();
		rapidxml::xml_node<> *zNode = yNode->next_sibling();
		rapidxml::xml_node<> *wNode = zNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0)
			|| (strcmp(yNode->name(), "y") != 0)
			|| (strcmp(zNode->name(), "z") != 0)
			|| (strcmp(wNode->name(), "w") != 0))
			return false;

		float x = utils::LexicalCast<float, char *>(xNode->value());
		float y = utils::LexicalCast<float, char *>(yNode->value());
		float z = utils::LexicalCast<float, char *>(zNode->value());
		float w = utils::LexicalCast<float, char *>(wNode->value());
		*outVector = { x, y, z, w };
		return true;
	}

	static bool XmlNodeToQuat(const rapidxml::xml_node<> *node, glm::quat *outQuat)
	{
		if (strcmp(node->name(), "quat") != 0)
			return false;

		if (!outQuat)
			return false;

		rapidxml::xml_node<> *wNode = node->first_node();
		rapidxml::xml_node<> *xNode = wNode->next_sibling();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();
		rapidxml::xml_node<> *zNode = yNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0)
			|| (strcmp(yNode->name(), "y") != 0)
			|| (strcmp(zNode->name(), "z") != 0)
			|| (strcmp(wNode->name(), "w") != 0))
			return false;

		float x = utils::LexicalCast<float, char *>(xNode->value());
		float y = utils::LexicalCast<float, char *>(yNode->value());
		float z = utils::LexicalCast<float, char *>(zNode->value());
		float w = utils::LexicalCast<float, char *>(wNode->value());
		*outQuat = { w, x, y, z };
		return true;
	}

	static bool XmlNodeToMat2(const rapidxml::xml_node<> *node, glm::mat2 *outMat)
	{
		if (strcmp(node->name(), "mat2") != 0)
			return false;

		if (!outMat)
			return false;

		rapidxml::xml_node<> *m00Node = node->first_node();
		rapidxml::xml_node<> *m01Node = m00Node->next_sibling();
		rapidxml::xml_node<> *m10Node = m01Node->next_sibling();
		rapidxml::xml_node<> *m11Node = m10Node->next_sibling();

		if ((strcmp(m00Node->name(), "m00") != 0)
			|| (strcmp(m01Node->name(), "m01") != 0)
			|| (strcmp(m10Node->name(), "m10") != 0)
			|| (strcmp(m11Node->name(), "m11") != 0))
			return false;

		float m00 = utils::LexicalCast<float, char *>(m00Node->value());
		float m01 = utils::LexicalCast<float, char *>(m01Node->value());
		float m10 = utils::LexicalCast<float, char *>(m10Node->value());
		float m11 = utils::LexicalCast<float, char *>(m11Node->value());

		glm::mat2 result;
		result[0][0] = m00;
		result[0][1] = m01;
		result[1][0] = m10;
		result[1][1] = m11;
		*outMat = result;
		return true;
	}

	static bool XmlNodeToMat3(const rapidxml::xml_node<> *node, glm::mat3 *outMat)
	{
		if (strcmp(node->name(), "mat3") != 0)
			return false;

		if (!outMat)
			return false;

		rapidxml::xml_node<> *m00Node = node->first_node();
		rapidxml::xml_node<> *m01Node = m00Node->next_sibling();
		rapidxml::xml_node<> *m02Node = m01Node->next_sibling();
		rapidxml::xml_node<> *m10Node = m02Node->next_sibling();
		rapidxml::xml_node<> *m11Node = m10Node->next_sibling();
		rapidxml::xml_node<> *m12Node = m11Node->next_sibling();
		rapidxml::xml_node<> *m20Node = m12Node->next_sibling();
		rapidxml::xml_node<> *m21Node = m20Node->next_sibling();
		rapidxml::xml_node<> *m22Node = m21Node->next_sibling();

		if ((strcmp(m00Node->name(), "m00") != 0)
			|| (strcmp(m01Node->name(), "m01") != 0)
			|| (strcmp(m02Node->name(), "m02") != 0)
			|| (strcmp(m10Node->name(), "m10") != 0)
			|| (strcmp(m11Node->name(), "m11") != 0)
			|| (strcmp(m12Node->name(), "m12") != 0)
			|| (strcmp(m20Node->name(), "m20") != 0)
			|| (strcmp(m21Node->name(), "m21") != 0)
			|| (strcmp(m22Node->name(), "m22") != 0))
			return false;

		float m00 = utils::LexicalCast<float, char *>(m00Node->value());
		float m01 = utils::LexicalCast<float, char *>(m01Node->value());
		float m02 = utils::LexicalCast<float, char *>(m02Node->value());
		float m10 = utils::LexicalCast<float, char *>(m10Node->value());
		float m11 = utils::LexicalCast<float, char *>(m11Node->value());
		float m12 = utils::LexicalCast<float, char *>(m12Node->value());
		float m20 = utils::LexicalCast<float, char *>(m20Node->value());
		float m21 = utils::LexicalCast<float, char *>(m21Node->value());
		float m22 = utils::LexicalCast<float, char *>(m22Node->value());

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
		*outMat = result;
		return true;
	}

	static bool XmlNodeToMat4(const rapidxml::xml_node<> *node, glm::mat4 *outMat)
	{
		if (strcmp(node->name(), "mat4") != 0)
			return false;

		if (!outMat)
			return false;

		rapidxml::xml_node<> *m00Node = node->first_node();
		rapidxml::xml_node<> *m01Node = m00Node->next_sibling();
		rapidxml::xml_node<> *m02Node = m01Node->next_sibling();
		rapidxml::xml_node<> *m03Node = m02Node->next_sibling();
		rapidxml::xml_node<> *m10Node = m03Node->next_sibling();
		rapidxml::xml_node<> *m11Node = m10Node->next_sibling();
		rapidxml::xml_node<> *m12Node = m11Node->next_sibling();
		rapidxml::xml_node<> *m13Node = m12Node->next_sibling();
		rapidxml::xml_node<> *m20Node = m13Node->next_sibling();
		rapidxml::xml_node<> *m21Node = m20Node->next_sibling();
		rapidxml::xml_node<> *m22Node = m21Node->next_sibling();
		rapidxml::xml_node<> *m23Node = m22Node->next_sibling();
		rapidxml::xml_node<> *m30Node = m23Node->next_sibling();
		rapidxml::xml_node<> *m31Node = m30Node->next_sibling();
		rapidxml::xml_node<> *m32Node = m31Node->next_sibling();
		rapidxml::xml_node<> *m33Node = m32Node->next_sibling();

		if ((strcmp(m00Node->name(), "m00") != 0)
			|| (strcmp(m01Node->name(), "m01") != 0)
			|| (strcmp(m02Node->name(), "m02") != 0)
			|| (strcmp(m03Node->name(), "m03") != 0)
			|| (strcmp(m10Node->name(), "m10") != 0)
			|| (strcmp(m11Node->name(), "m11") != 0)
			|| (strcmp(m12Node->name(), "m12") != 0)
			|| (strcmp(m13Node->name(), "m13") != 0)
			|| (strcmp(m20Node->name(), "m20") != 0)
			|| (strcmp(m21Node->name(), "m21") != 0)
			|| (strcmp(m22Node->name(), "m22") != 0)
			|| (strcmp(m23Node->name(), "m23") != 0)
			|| (strcmp(m30Node->name(), "m30") != 0)
			|| (strcmp(m31Node->name(), "m31") != 0)
			|| (strcmp(m32Node->name(), "m32") != 0)
			|| (strcmp(m33Node->name(), "m33") != 0))
			return false;

		float m00 = utils::LexicalCast<float, char *>(m00Node->value());
		float m01 = utils::LexicalCast<float, char *>(m01Node->value());
		float m02 = utils::LexicalCast<float, char *>(m02Node->value());
		float m03 = utils::LexicalCast<float, char *>(m03Node->value());
		float m10 = utils::LexicalCast<float, char *>(m10Node->value());
		float m11 = utils::LexicalCast<float, char *>(m11Node->value());
		float m12 = utils::LexicalCast<float, char *>(m12Node->value());
		float m13 = utils::LexicalCast<float, char *>(m13Node->value());
		float m20 = utils::LexicalCast<float, char *>(m20Node->value());
		float m21 = utils::LexicalCast<float, char *>(m21Node->value());
		float m22 = utils::LexicalCast<float, char *>(m22Node->value());
		float m23 = utils::LexicalCast<float, char *>(m23Node->value());
		float m30 = utils::LexicalCast<float, char *>(m30Node->value());
		float m31 = utils::LexicalCast<float, char *>(m31Node->value());
		float m32 = utils::LexicalCast<float, char *>(m32Node->value());
		float m33 = utils::LexicalCast<float, char *>(m33Node->value());

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
		*outMat = result;
		return true;
	}
}
