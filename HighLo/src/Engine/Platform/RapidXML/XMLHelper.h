// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-08) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Utils/StringUtils.h"
#include "Engine/Utils/LoaderUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

#include <rapidxml/rapidxml.hpp>

#ifdef HL_DEBUG
#define XML_HELPER_VERBOSE_LOGGING 1
#else
#define XML_HELPER_VERBOSE_LOGGING 0
#endif

namespace highlo::utils
{
	static char *PrepareString(rapidxml::xml_document<> &document, const HLString &str)
	{
		char *result = document.allocate_string(str.C_Str(), str.Length() + 1);
		result[str.Length()] = '\0';
		return result;
	}

	static rapidxml::xml_node<> *Vec2ToXMLNode(rapidxml::xml_document<> &document, const glm::vec2 &v)
	{
		char *x = PrepareString(document, HLString::ToString(v.x));
		char *y = PrepareString(document, HLString::ToString(v.y));

		rapidxml::xml_node<> *xNode = document.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *yNode = document.allocate_node(rapidxml::node_element, "y", y);

		rapidxml::xml_node<> *node = document.allocate_node(rapidxml::node_element, utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str());
		node->append_node(xNode);
		node->append_node(yNode);
		return node;
	}

	static rapidxml::xml_node<> *Vec3ToXMLNode(rapidxml::xml_document<> &document, const glm::vec3 &v)
	{
		char *x = PrepareString(document, HLString::ToString(v.x));
		char *y = PrepareString(document, HLString::ToString(v.y));
		char *z = PrepareString(document, HLString::ToString(v.z));

		rapidxml::xml_node<> *xNode = document.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *yNode = document.allocate_node(rapidxml::node_element, "y", y);
		rapidxml::xml_node<> *zNode = document.allocate_node(rapidxml::node_element, "z", z);

		rapidxml::xml_node<> *node = document.allocate_node(rapidxml::node_element, utils::DocumentDataTypeToString(DocumentDataType::Vec3).C_Str());
		node->append_node(xNode);
		node->append_node(yNode);
		node->append_node(zNode);
		return node;
	}

	static rapidxml::xml_node<> *Vec4ToXMLNode(rapidxml::xml_document<> &document, const glm::vec4 &v)
	{
		char *x = PrepareString(document, HLString::ToString(v.x));
		char *y = PrepareString(document, HLString::ToString(v.y));
		char *z = PrepareString(document, HLString::ToString(v.z));
		char *w = PrepareString(document, HLString::ToString(v.w));

		rapidxml::xml_node<> *xNode = document.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *yNode = document.allocate_node(rapidxml::node_element, "y", y);
		rapidxml::xml_node<> *zNode = document.allocate_node(rapidxml::node_element, "z", z);
		rapidxml::xml_node<> *wNode = document.allocate_node(rapidxml::node_element, "w", w);

		rapidxml::xml_node<> *node = document.allocate_node(rapidxml::node_element, utils::DocumentDataTypeToString(DocumentDataType::Vec4).C_Str());
		node->append_node(xNode);
		node->append_node(yNode);
		node->append_node(zNode);
		node->append_node(wNode);
		return node;
	}

	static rapidxml::xml_node<> *Mat2ToXMLNode(rapidxml::xml_document<> &document, const glm::mat2 &m)
	{
		char *m00 = PrepareString(document, HLString::ToString(m[0][0]));
		char *m01 = PrepareString(document, HLString::ToString(m[0][1]));
		char *m10 = PrepareString(document, HLString::ToString(m[1][0]));
		char *m11 = PrepareString(document, HLString::ToString(m[1][1]));

		rapidxml::xml_node<> *m00Node = document.allocate_node(rapidxml::node_element, "m00", m00);
		rapidxml::xml_node<> *m01Node = document.allocate_node(rapidxml::node_element, "m01", m01);
		rapidxml::xml_node<> *m10Node = document.allocate_node(rapidxml::node_element, "m10", m10);
		rapidxml::xml_node<> *m11Node = document.allocate_node(rapidxml::node_element, "m11", m11);

		rapidxml::xml_node<> *node = document.allocate_node(rapidxml::node_element, utils::DocumentDataTypeToString(DocumentDataType::Mat2).C_Str());
		node->append_node(m00Node);
		node->append_node(m01Node);
		node->append_node(m10Node);
		node->append_node(m11Node);
		return node;
	}

	static rapidxml::xml_node<> *Mat3ToXMLNode(rapidxml::xml_document<> &document, const glm::mat3 &m)
	{
		char *m00 = PrepareString(document, HLString::ToString(m[0][0]));
		char *m01 = PrepareString(document, HLString::ToString(m[0][1]));
		char *m02 = PrepareString(document, HLString::ToString(m[0][2]));

		char *m10 = PrepareString(document, HLString::ToString(m[1][0]));
		char *m11 = PrepareString(document, HLString::ToString(m[1][1]));
		char *m12 = PrepareString(document, HLString::ToString(m[1][2]));

		char *m20 = PrepareString(document, HLString::ToString(m[2][0]));
		char *m21 = PrepareString(document, HLString::ToString(m[2][1]));
		char *m22 = PrepareString(document, HLString::ToString(m[2][2]));

		rapidxml::xml_node<> *m00Node = document.allocate_node(rapidxml::node_element, "m00", m00);
		rapidxml::xml_node<> *m01Node = document.allocate_node(rapidxml::node_element, "m01", m01);
		rapidxml::xml_node<> *m02Node = document.allocate_node(rapidxml::node_element, "m02", m02);

		rapidxml::xml_node<> *m10Node = document.allocate_node(rapidxml::node_element, "m10", m10);
		rapidxml::xml_node<> *m11Node = document.allocate_node(rapidxml::node_element, "m11", m11);
		rapidxml::xml_node<> *m12Node = document.allocate_node(rapidxml::node_element, "m12", m12);

		rapidxml::xml_node<> *m20Node = document.allocate_node(rapidxml::node_element, "m20", m20);
		rapidxml::xml_node<> *m21Node = document.allocate_node(rapidxml::node_element, "m21", m21);
		rapidxml::xml_node<> *m22Node = document.allocate_node(rapidxml::node_element, "m22", m22);

		rapidxml::xml_node<> *node = document.allocate_node(rapidxml::node_element, utils::DocumentDataTypeToString(DocumentDataType::Mat3).C_Str());
		node->append_node(m00Node);
		node->append_node(m01Node);
		node->append_node(m02Node);

		node->append_node(m10Node);
		node->append_node(m11Node);
		node->append_node(m12Node);

		node->append_node(m20Node);
		node->append_node(m21Node);
		node->append_node(m22Node);
		return node;
	}

	static rapidxml::xml_node<> *Mat4ToXMLNode(rapidxml::xml_document<> &document, const glm::mat4 &m)
	{
		char *m00 = PrepareString(document, HLString::ToString(m[0][0]));
		char *m01 = PrepareString(document, HLString::ToString(m[0][1]));
		char *m02 = PrepareString(document, HLString::ToString(m[0][2]));
		char *m03 = PrepareString(document, HLString::ToString(m[0][3]));

		char *m10 = PrepareString(document, HLString::ToString(m[1][0]));
		char *m11 = PrepareString(document, HLString::ToString(m[1][1]));
		char *m12 = PrepareString(document, HLString::ToString(m[1][2]));
		char *m13 = PrepareString(document, HLString::ToString(m[1][3]));

		char *m20 = PrepareString(document, HLString::ToString(m[2][0]));
		char *m21 = PrepareString(document, HLString::ToString(m[2][1]));
		char *m22 = PrepareString(document, HLString::ToString(m[2][2]));
		char *m23 = PrepareString(document, HLString::ToString(m[2][3]));

		char *m30 = PrepareString(document, HLString::ToString(m[3][0]));
		char *m31 = PrepareString(document, HLString::ToString(m[3][1]));
		char *m32 = PrepareString(document, HLString::ToString(m[3][2]));
		char *m33 = PrepareString(document, HLString::ToString(m[3][3]));

		rapidxml::xml_node<> *m00Node = document.allocate_node(rapidxml::node_element, "m00", m00);
		rapidxml::xml_node<> *m01Node = document.allocate_node(rapidxml::node_element, "m01", m01);
		rapidxml::xml_node<> *m02Node = document.allocate_node(rapidxml::node_element, "m02", m02);
		rapidxml::xml_node<> *m03Node = document.allocate_node(rapidxml::node_element, "m03", m03);

		rapidxml::xml_node<> *m10Node = document.allocate_node(rapidxml::node_element, "m10", m10);
		rapidxml::xml_node<> *m11Node = document.allocate_node(rapidxml::node_element, "m11", m11);
		rapidxml::xml_node<> *m12Node = document.allocate_node(rapidxml::node_element, "m12", m12);
		rapidxml::xml_node<> *m13Node = document.allocate_node(rapidxml::node_element, "m13", m13);

		rapidxml::xml_node<> *m20Node = document.allocate_node(rapidxml::node_element, "m20", m20);
		rapidxml::xml_node<> *m21Node = document.allocate_node(rapidxml::node_element, "m21", m21);
		rapidxml::xml_node<> *m22Node = document.allocate_node(rapidxml::node_element, "m22", m22);
		rapidxml::xml_node<> *m23Node = document.allocate_node(rapidxml::node_element, "m23", m23);

		rapidxml::xml_node<> *m30Node = document.allocate_node(rapidxml::node_element, "m30", m30);
		rapidxml::xml_node<> *m31Node = document.allocate_node(rapidxml::node_element, "m31", m31);
		rapidxml::xml_node<> *m32Node = document.allocate_node(rapidxml::node_element, "m32", m32);
		rapidxml::xml_node<> *m33Node = document.allocate_node(rapidxml::node_element, "m33", m33);

		rapidxml::xml_node<> *node = document.allocate_node(rapidxml::node_element, utils::DocumentDataTypeToString(DocumentDataType::Mat4).C_Str());
		node->append_node(m00Node);
		node->append_node(m01Node);
		node->append_node(m02Node);
		node->append_node(m03Node);

		node->append_node(m10Node);
		node->append_node(m11Node);
		node->append_node(m12Node);
		node->append_node(m13Node);

		node->append_node(m20Node);
		node->append_node(m21Node);
		node->append_node(m22Node);
		node->append_node(m23Node);

		node->append_node(m30Node);
		node->append_node(m31Node);
		node->append_node(m32Node);
		node->append_node(m33Node);
		return node;
	}

	static rapidxml::xml_node<> *QuatToXMLNode(rapidxml::xml_document<> &document, const glm::quat &q)
	{
		char *w = PrepareString(document, HLString::ToString(q.w));
		char *x = PrepareString(document, HLString::ToString(q.x));
		char *y = PrepareString(document, HLString::ToString(q.y));
		char *z = PrepareString(document, HLString::ToString(q.z));

		rapidxml::xml_node<> *wNode = document.allocate_node(rapidxml::node_element, "w", w);
		rapidxml::xml_node<> *xNode = document.allocate_node(rapidxml::node_element, "x", x);
		rapidxml::xml_node<> *yNode = document.allocate_node(rapidxml::node_element, "y", y);
		rapidxml::xml_node<> *zNode = document.allocate_node(rapidxml::node_element, "z", z);

		rapidxml::xml_node<> *node = document.allocate_node(rapidxml::node_element, utils::DocumentDataTypeToString(DocumentDataType::Quat).C_Str());
		node->append_node(wNode);
		node->append_node(xNode);
		node->append_node(yNode);
		node->append_node(zNode);
		return node;
	}

	static bool XMLNodeToVec2(const rapidxml::xml_node<> *node, glm::vec2 *outVector, HLString *outErrorMessage = nullptr)
	{
		if (strcmp(node->name(), utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str()) != 0)
		{
			if (outErrorMessage)
				*outErrorMessage = fmt::format("Error: wrong input node! Expected input node to be named %s, but got %s", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), node->name());
			return false;
		}

		if (!outVector)
		{
			if (outErrorMessage)
				*outErrorMessage = "Error: Result pointer is nullptr!";
			return false;
		}

		rapidxml::xml_node<> *xNode = node->first_node();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0) || (strcmp(yNode->name(), "y") != 0))
		{
			if (outErrorMessage)
				*outErrorMessage = "Children nodes are not named x, y!";
			return false;
		}

		float x = utils::LexicalCast<float, char*>(xNode->value());
		float y = utils::LexicalCast<float, char*>(yNode->value());

	#if XML_HELPER_VERBOSE_LOGGING
		HL_CORE_TRACE("Reading {0} with values {1}, {2}", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), x, y);
	#endif

		*outVector = { x, y };

		return true;
	}

	static bool XMLNodeToVec3(const rapidxml::xml_node<> *node, glm::vec3 *outVector, HLString *outErrorMessage = nullptr)
	{
		if (strcmp(node->name(), utils::DocumentDataTypeToString(DocumentDataType::Vec3).C_Str()) != 0)
		{
			if (outErrorMessage)
				*outErrorMessage = fmt::format("Error: wrong input node! Expected input node to be named %s, but got %s", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), node->name());
			return false;
		}

		if (!outVector)
		{
			if (outErrorMessage)
				*outErrorMessage = "Error: Result pointer is nullptr!";
			return false;
		}

		rapidxml::xml_node<> *xNode = node->first_node();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();
		rapidxml::xml_node<> *zNode = yNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0) || (strcmp(yNode->name(), "y") != 0) || (strcmp(zNode->name(), "z") != 0))
		{
			if (outErrorMessage)
				*outErrorMessage = "Children nodes are not named x, y, z!";
			return false;
		}

		float x = utils::LexicalCast<float, char*>(xNode->value());
		float y = utils::LexicalCast<float, char*>(yNode->value());
		float z = utils::LexicalCast<float, char*>(zNode->value());

	#if XML_HELPER_VERBOSE_LOGGING
		HL_CORE_TRACE("Reading {0} with values {1}, {2}, {3}", utils::DocumentDataTypeToString(DocumentDataType::Vec3).C_Str(), x, y, z);
	#endif

		*outVector = { x, y, z };

		return true;
	}

	static bool XMLNodeToVec4(const rapidxml::xml_node<> *node, glm::vec4 *outVector, HLString *outErrorMessage = nullptr)
	{
		if (strcmp(node->name(), utils::DocumentDataTypeToString(DocumentDataType::Vec4).C_Str()) != 0)
		{
			if (outErrorMessage)
				*outErrorMessage = fmt::format("Error: wrong input node! Expected input node to be named %s, but got %s", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), node->name());
			return false;
		}

		if (!outVector)
		{
			if (outErrorMessage)
				*outErrorMessage = "Error: Result pointer is nullptr!";
			return false;
		}

		rapidxml::xml_node<> *xNode = node->first_node();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();
		rapidxml::xml_node<> *zNode = yNode->next_sibling();
		rapidxml::xml_node<> *wNode = zNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0) || (strcmp(yNode->name(), "y") != 0) || (strcmp(zNode->name(), "z") != 0) || (strcmp(wNode->name(), "w") != 0))
		{
			if (outErrorMessage)
				*outErrorMessage = "Children nodes are not named x, y, z, w!";
			return false;
		}

		float x = utils::LexicalCast<float, char*>(xNode->value());
		float y = utils::LexicalCast<float, char*>(yNode->value());
		float z = utils::LexicalCast<float, char*>(zNode->value());
		float w = utils::LexicalCast<float, char*>(wNode->value());

	#if XML_HELPER_VERBOSE_LOGGING
		HL_CORE_TRACE("Reading {0} with values {1}, {2}, {3}, {4}", utils::DocumentDataTypeToString(DocumentDataType::Vec4).C_Str(), x, y, z, w);
	#endif

		*outVector = { x, y, z, w };

		return true;
	}

	static bool XMLNodeToMat2(const rapidxml::xml_node<> *node, glm::mat2 *outMatrix, HLString *outErrorMessage = nullptr)
	{
		if (strcmp(node->name(), utils::DocumentDataTypeToString(DocumentDataType::Mat2).C_Str()) != 0)
		{
			if (outErrorMessage)
				*outErrorMessage = fmt::format("Error: wrong input node! Expected input node to be named %s, but got %s", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), node->name());
			return false;
		}

		if (!outMatrix)
		{
			if (outErrorMessage)
				*outErrorMessage = "Error: Result pointer is nullptr!";
			return false;
		}

		rapidxml::xml_node<> *m00Node = node->first_node();
		rapidxml::xml_node<> *m01Node = m00Node->next_sibling();
		rapidxml::xml_node<> *m10Node = m01Node->next_sibling();
		rapidxml::xml_node<> *m11Node = m10Node->next_sibling();

		if ((strcmp(m00Node->name(), "m00") != 0) || (strcmp(m01Node->name(), "m01") != 0) || (strcmp(m10Node->name(), "m10") != 0) || (strcmp(m11Node->name(), "m11") != 0))
		{
			if (outErrorMessage)
				*outErrorMessage = "Children nodes are not named m00, m01, m10, m11!";
			return false;
		}

		float m00 = utils::LexicalCast<float, char*>(m00Node->value());
		float m01 = utils::LexicalCast<float, char*>(m01Node->value());
		float m10 = utils::LexicalCast<float, char*>(m10Node->value());
		float m11 = utils::LexicalCast<float, char*>(m11Node->value());

	#if XML_HELPER_VERBOSE_LOGGING
		HL_CORE_TRACE("Reading {0} with values {1}, {2}, {3}, {4}", utils::DocumentDataTypeToString(DocumentDataType::Mat2).C_Str(), m00, m01, m10, m11);
	#endif

		glm::mat2 result;
		result[0][0] = m00;
		result[0][1] = m01;
		result[1][0] = m10;
		result[1][1] = m11;
		*outMatrix = result;

		return true;
	}

	static bool XMLNodeToMat3(const rapidxml::xml_node<> *node, glm::mat3 *outMatrix, HLString *outErrorMessage = nullptr)
	{
		if (strcmp(node->name(), utils::DocumentDataTypeToString(DocumentDataType::Mat3).C_Str()) != 0)
		{
			if (outErrorMessage)
				*outErrorMessage = fmt::format("Error: wrong input node! Expected input node to be named %s, but got %s", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), node->name());
			return false;
		}

		if (!outMatrix)
		{
			if (outErrorMessage)
				*outErrorMessage = "Error: Result pointer is nullptr!";
			return false;
		}

		rapidxml::xml_node<> *m00Node = node->first_node();
		rapidxml::xml_node<> *m01Node = m00Node->next_sibling();
		rapidxml::xml_node<> *m02Node = m01Node->next_sibling();

		rapidxml::xml_node<> *m10Node = m02Node->next_sibling();
		rapidxml::xml_node<> *m11Node = m10Node->next_sibling();
		rapidxml::xml_node<> *m12Node = m11Node->next_sibling();
		
		rapidxml::xml_node<> *m20Node = m12Node->next_sibling();
		rapidxml::xml_node<> *m21Node = m20Node->next_sibling();
		rapidxml::xml_node<> *m22Node = m21Node->next_sibling();

		if ((strcmp(m00Node->name(), "m00") != 0) || (strcmp(m01Node->name(), "m01") != 0) || (strcmp(m02Node->name(), "m02") != 0) 
		 || (strcmp(m10Node->name(), "m10") != 0) || (strcmp(m11Node->name(), "m11") != 0) || (strcmp(m12Node->name(), "m12") != 0)
		 || (strcmp(m20Node->name(), "m20") != 0) || (strcmp(m21Node->name(), "m21") != 0) || (strcmp(m22Node->name(), "m22") != 0))
		{
			if (outErrorMessage)
				*outErrorMessage = "Children nodes are not named m00, m01, m02, m10, m11, m12, m20, m21, m22!";
			return false;
		}

		float m00 = utils::LexicalCast<float, char*>(m00Node->value());
		float m01 = utils::LexicalCast<float, char*>(m01Node->value());
		float m02 = utils::LexicalCast<float, char*>(m02Node->value());

		float m10 = utils::LexicalCast<float, char*>(m10Node->value());
		float m11 = utils::LexicalCast<float, char*>(m11Node->value());
		float m12 = utils::LexicalCast<float, char*>(m12Node->value());
		
		float m20 = utils::LexicalCast<float, char*>(m20Node->value());
		float m21 = utils::LexicalCast<float, char*>(m21Node->value());
		float m22 = utils::LexicalCast<float, char*>(m22Node->value());

	#if XML_HELPER_VERBOSE_LOGGING
		HL_CORE_TRACE("Reading {0} with values {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}", utils::DocumentDataTypeToString(DocumentDataType::Mat3).C_Str(), m00, m01, m02, m10, m11, m12, m20, m21, m22);
	#endif

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
		*outMatrix = result;

		return true;
	}

	static bool XMLNodeToMat4(const rapidxml::xml_node<> *node, glm::mat4 *outMatrix, HLString *outErrorMessage = nullptr)
	{
		if (strcmp(node->name(), utils::DocumentDataTypeToString(DocumentDataType::Mat4).C_Str()) != 0)
		{
			if (outErrorMessage)
				*outErrorMessage = fmt::format("Error: wrong input node! Expected input node to be named %s, but got %s", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), node->name());
			return false;
		}

		if (!outMatrix)
		{
			if (outErrorMessage)
				*outErrorMessage = "Error: Result pointer is nullptr!";
			return false;
		}

		rapidxml::xml_node<> *m00Node = node->first_node();
		rapidxml::xml_node<> *m01Node = m00Node->next_sibling();
		rapidxml::xml_node<> *m02Node = m01Node->next_sibling();
		rapidxml::xml_node<> *m03Node = m02Node->next_sibling();

		rapidxml::xml_node<> *m10Node = m02Node->next_sibling();
		rapidxml::xml_node<> *m11Node = m10Node->next_sibling();
		rapidxml::xml_node<> *m12Node = m11Node->next_sibling();
		rapidxml::xml_node<> *m13Node = m12Node->next_sibling();

		rapidxml::xml_node<> *m20Node = m12Node->next_sibling();
		rapidxml::xml_node<> *m21Node = m20Node->next_sibling();
		rapidxml::xml_node<> *m22Node = m21Node->next_sibling();
		rapidxml::xml_node<> *m23Node = m22Node->next_sibling();

		rapidxml::xml_node<> *m30Node = m23Node->next_sibling();
		rapidxml::xml_node<> *m31Node = m30Node->next_sibling();
		rapidxml::xml_node<> *m32Node = m31Node->next_sibling();
		rapidxml::xml_node<> *m33Node = m32Node->next_sibling();

		if ((strcmp(m00Node->name(), "m00") != 0) || (strcmp(m01Node->name(), "m01") != 0) || (strcmp(m02Node->name(), "m02") != 0) || (strcmp(m03Node->name(), "m03") != 0)
		 || (strcmp(m10Node->name(), "m10") != 0) || (strcmp(m11Node->name(), "m11") != 0) || (strcmp(m12Node->name(), "m12") != 0) || (strcmp(m13Node->name(), "m13") != 0)
		 || (strcmp(m20Node->name(), "m20") != 0) || (strcmp(m21Node->name(), "m21") != 0) || (strcmp(m22Node->name(), "m22") != 0) || (strcmp(m23Node->name(), "m23") != 0)
		 || (strcmp(m30Node->name(), "m30") != 0) || (strcmp(m31Node->name(), "m31") != 0) || (strcmp(m32Node->name(), "m32") != 0) || (strcmp(m33Node->name(), "m33") != 0))
		{
			if (outErrorMessage)
				*outErrorMessage = "Children nodes are not named m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33!";
			return false;
		}

		float m00 = utils::LexicalCast<float, char*>(m00Node->value());
		float m01 = utils::LexicalCast<float, char*>(m01Node->value());
		float m02 = utils::LexicalCast<float, char*>(m02Node->value());
		float m03 = utils::LexicalCast<float, char*>(m03Node->value());

		float m10 = utils::LexicalCast<float, char*>(m10Node->value());
		float m11 = utils::LexicalCast<float, char*>(m11Node->value());
		float m12 = utils::LexicalCast<float, char*>(m12Node->value());
		float m13 = utils::LexicalCast<float, char*>(m13Node->value());

		float m20 = utils::LexicalCast<float, char*>(m20Node->value());
		float m21 = utils::LexicalCast<float, char*>(m21Node->value());
		float m22 = utils::LexicalCast<float, char*>(m22Node->value());
		float m23 = utils::LexicalCast<float, char*>(m23Node->value());

		float m30 = utils::LexicalCast<float, char*>(m30Node->value());
		float m31 = utils::LexicalCast<float, char*>(m31Node->value());
		float m32 = utils::LexicalCast<float, char*>(m32Node->value());
		float m33 = utils::LexicalCast<float, char*>(m33Node->value());

	#if XML_HELPER_VERBOSE_LOGGING
		HL_CORE_TRACE("Reading {0} with values {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}", utils::DocumentDataTypeToString(DocumentDataType::Mat4).C_Str(), m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
	#endif

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
		*outMatrix = result;

		return true;
	}

	static bool XMLNodeToQuat(const rapidxml::xml_node<> *node, glm::quat *outQuaternion, HLString *outErrorMessage = nullptr)
	{
		if (strcmp(node->name(), utils::DocumentDataTypeToString(DocumentDataType::Quat).C_Str()) != 0)
		{
			if (outErrorMessage)
				*outErrorMessage = fmt::format("Error: wrong input node! Expected input node to be named %s, but got %s", utils::DocumentDataTypeToString(DocumentDataType::Vec2).C_Str(), node->name());
			return false;
		}

		if (!outQuaternion)
		{
			if (outErrorMessage)
				*outErrorMessage = "Error: Result pointer is nullptr!";
			return false;
		}

		rapidxml::xml_node<> *wNode = node->first_node();
		rapidxml::xml_node<> *xNode = wNode->next_sibling();
		rapidxml::xml_node<> *yNode = xNode->next_sibling();
		rapidxml::xml_node<> *zNode = yNode->next_sibling();

		if ((strcmp(xNode->name(), "x") != 0) || (strcmp(yNode->name(), "y") != 0) || (strcmp(zNode->name(), "z") != 0) || (strcmp(wNode->name(), "w") != 0))
		{
			if (outErrorMessage)
				*outErrorMessage = "Children nodes are not named w, x, y, z!";
			return false;
		}

		float x = utils::LexicalCast<float, char*>(xNode->value());
		float y = utils::LexicalCast<float, char*>(yNode->value());
		float z = utils::LexicalCast<float, char*>(zNode->value());
		float w = utils::LexicalCast<float, char*>(wNode->value());

	#if XML_HELPER_VERBOSE_LOGGING
		HL_CORE_TRACE("Reading {0} with values {1}, {2}, {3}, {4}", utils::DocumentDataTypeToString(DocumentDataType::Quat).C_Str(), w, x, y, z);
	#endif

		*outQuaternion = { w, x, y, z };

		return true;
	}
}

