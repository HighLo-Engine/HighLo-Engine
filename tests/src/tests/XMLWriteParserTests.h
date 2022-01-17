// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-16) initial release
//

#pragma once

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace highlo;

struct XMLWriteParserTests : public testing::Test
{
	Ref<DocumentWriter> Writer;

	XMLWriteParserTests()
	{
		Writer = DocumentWriter::Create("", DocumentType::XML);
	}

	virtual ~XMLWriteParserTests()
	{
	}
};


TEST_F(XMLWriteParserTests, XMLParser_Write_String)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><string key=\"test\">Hello World!</string></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteString("test", "Hello World!");
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

// Duplicate of above test, the XML Parser does not support a special TypeCheck because everything is type-safe by default
// (because the node names are always the type itself)
TEST_F(XMLWriteParserTests, XMLParser_Write_String_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><string key=\"test\">Hello World!</string></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteString("test", "Hello World!");
	Writer->EndObject();

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Int32)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><int32 key=\"test\">-10</int32></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt32("test", -10);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Int32_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><int32 key=\"test\">-10</int32></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt32("test", -10);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_UInt32)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><uint32 key=\"test\">42</uint32></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt32("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_UInt32_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><uint32 key=\"test\">42</uint32></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt32("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Int64)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><int64 key=\"test\">42</int64></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Int64_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><int64 key=\"test\">42</int64></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt64("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_UInt64)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><uint64 key=\"test\">42</uint64></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_UInt64_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><uint64 key=\"test\">42</uint64></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt64("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Bool)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><bool key=\"test\">1</bool></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteBool("test", true);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Bool_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><bool key=\"test\">1</bool></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteBool("test", true);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Float)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><float key=\"test\">20.2</float></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteFloat("test", 20.2f);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Float_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><float key=\"test\">20.2</float></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteFloat("test", 20.2f);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Double)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><double key=\"test\">20.2</double></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteDouble("test", 20.2);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Double_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><double key=\"test\">20.2</double></HighLo>";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteDouble("test", 20.2);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Vec2)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><vector2 key=\"test\"><x>20.2</x><y>10.5</y></vector2></HighLo>";
	glm::vec2 v(20.2f, 10.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec2("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec2_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><vector2 key=\"test\"><x>20.2</x><y>10.5</y></vector2></HighLo>";
	glm::vec2 v(20.2f, 10.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec2("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Vec3)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><vector3 key=\"test\"><x>20.2</x><y>10.5</y><z>23.5</z></vector3></HighLo>";
	glm::vec3 v(20.2f, 10.5f, 23.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec3("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec3_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><vector3 key=\"test\"><x>20.2</x><y>10.5</y><z>23.5</z></vector3></HighLo>";
	glm::vec3 v(20.2f, 10.5f, 23.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec3("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Vec4)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><vector4 key=\"test\"><x>20.2</x><y>10.5</y><z>23.5</z><w>11.5</w></vector4></HighLo>";
	glm::vec4 v(20.2f, 10.5f, 23.5f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec4("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec4_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><vector4 key=\"test\"><x>20.2</x><y>10.5</y><z>23.5</z><w>11.5</w></vector4></HighLo>";
	glm::vec4 v(20.2f, 10.5f, 23.5f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec4("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Mat2)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><matrix2 key=\"test\"><m00>20.2</m00><m01>10.5</m01><m10>23.5</m10><m11>11.5</m11></matrix2></HighLo>";

	glm::mat2 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[1][0] = 23.5f;
	m[1][1] = 11.5f;

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat2("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat2_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><matrix2 key=\"test\"><m00>20.2</m00><m01>10.5</m01><m10>23.5</m10><m11>11.5</m11></matrix2></HighLo>";

	glm::mat2 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[1][0] = 23.5f;
	m[1][1] = 11.5f;

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat2("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Mat3)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><matrix3 key=\"test\"><m00>20.2</m00><m01>10.5</m01><m02>23.5</m02><m10>11.5</m10><m11>534.5</m11><m12>234.5</m12><m20>12.5</m20><m21>45.7</m21><m22>42.4</m22></matrix3></HighLo>";

	glm::mat3 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[0][2] = 23.5f;

	m[1][0] = 11.5f;
	m[1][1] = 534.5f;
	m[1][2] = 234.5f;

	m[2][0] = 12.5f;
	m[2][1] = 45.7f;
	m[2][2] = 42.4f;

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat3("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat3_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><matrix3 key=\"test\"><m00>20.2</m00><m01>10.5</m01><m02>23.5</m02><m10>11.5</m10><m11>534.5</m11><m12>234.5</m12><m20>12.5</m20><m21>45.7</m21><m22>42.4</m22></matrix3></HighLo>";

	glm::mat3 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[0][2] = 23.5f;

	m[1][0] = 11.5f;
	m[1][1] = 534.5f;
	m[1][2] = 234.5f;

	m[2][0] = 12.5f;
	m[2][1] = 45.7f;
	m[2][2] = 42.4f;

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat3("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Mat4)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><matrix4 key=\"test\"><m00>20.2</m00><m01>10.5</m01><m02>23.5</m02><m03>11.5</m03><m10>534.5</m10><m11>234.5</m11><m12>12.5</m12><m13>45.7</m13><m20>42.4</m20><m21>47.2</m21><m22>55.4</m22><m23>48.4</m23><m30>32</m30><m31>15.1</m31><m32>499.1</m32><m33>22.5</m33></matrix4></HighLo>";

	glm::mat4 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[0][2] = 23.5f;
	m[0][3] = 11.5f;

	m[1][0] = 534.5f;
	m[1][1] = 234.5f;
	m[1][2] = 12.5f;
	m[1][3] = 45.7f;

	m[2][0] = 42.4f;
	m[2][1] = 47.2f;
	m[2][2] = 55.4f;
	m[2][3] = 48.4f;

	m[3][0] = 32.0f;
	m[3][1] = 15.1f;
	m[3][2] = 499.1f;
	m[3][3] = 22.5f;

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat4("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat4_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><matrix4 key=\"test\"><m00>20.2</m00><m01>10.5</m01><m02>23.5</m02><m03>11.5</m03><m10>534.5</m10><m11>234.5</m11><m12>12.5</m12><m13>45.7</m13><m20>42.4</m20><m21>47.2</m21><m22>55.4</m22><m23>48.4</m23><m30>32</m30><m31>15.1</m31><m32>499.1</m32><m33>22.5</m33></matrix4></HighLo>";

	glm::mat4 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[0][2] = 23.5f;
	m[0][3] = 11.5f;

	m[1][0] = 534.5f;
	m[1][1] = 234.5f;
	m[1][2] = 12.5f;
	m[1][3] = 45.7f;

	m[2][0] = 42.4f;
	m[2][1] = 47.2f;
	m[2][2] = 55.4f;
	m[2][3] = 48.4f;

	m[3][0] = 32.0f;
	m[3][1] = 15.1f;
	m[3][2] = 499.1f;
	m[3][3] = 22.5f;

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat4("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Quaternion)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><quaternion key=\"test\"><x>10.5</x><y>23.1</y><z>11.5</z><w>20.2</w></quaternion></HighLo>";
	glm::quat q(20.2f, 10.5f, 23.1f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteQuaternion("test", q);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Quaternion_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><quaternion key=\"test\"><x>10.5</x><y>23.1</y><z>11.5</z><w>20.2</w></quaternion></HighLo>";
	glm::quat q(20.2f, 10.5f, 23.1f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteQuaternion("test", q);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

///
/// ArrayMap
/// 

TEST_F(XMLWriteParserTests, XMLParser_Write_String_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><string key=\"object0\">Hello</string><string key=\"object1\">world</string><string key=\"object2\">!</string></arraymap></HighLo>";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_String_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKey\"><string key=\"object0\">Hello</string><string key=\"object1\">world</string><string key=\"object2\">!</string></arraymap></HighLo>";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKey", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_String_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><string key=\"object0\">Hello</string><string key=\"object1\">world</string><string key=\"object2\">!</string></arraymap></HighLo>";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Int32_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><int32 key=\"object0\">42</int32><int32 key=\"object1\">10</int32><int32 key=\"object2\">-10</int32></arraymap></HighLo>";
	std::vector<int32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(-10);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteInt32("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Int32_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><int32 key=\"object0\">42</int32><int32 key=\"object1\">10</int32><int32 key=\"object2\">-10</int32></arraymap></HighLo>";
	std::vector<int32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(-10);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteInt32("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Int32_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><int32 key=\"object0\">42</int32><int32 key=\"object1\">10</int32><int32 key=\"object2\">-10</int32></arraymap></HighLo>";
	std::vector<int32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(-10);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteInt32("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_UInt32_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><uint32 key=\"object0\">42</uint32><uint32 key=\"object1\">10</uint32><uint32 key=\"object2\">12</uint32></arraymap></HighLo>";
	std::vector<uint32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteUInt32("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_UInt32_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><uint32 key=\"object0\">42</uint32><uint32 key=\"object1\">10</uint32><uint32 key=\"object2\">12</uint32></arraymap></HighLo>";
	std::vector<uint32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteUInt32("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_UInt32_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><uint32 key=\"object0\">42</uint32><uint32 key=\"object1\">10</uint32><uint32 key=\"object2\">12</uint32></arraymap></HighLo>";
	std::vector<uint32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteUInt32("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Int64_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><int64 key=\"object0\">42</int64><int64 key=\"object1\">10</int64><int64 key=\"object2\">12</int64></arraymap></HighLo>";
	std::vector<int64> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteInt64("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Int64_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><int64 key=\"object0\">42</int64><int64 key=\"object1\">10</int64><int64 key=\"object2\">12</int64></arraymap></HighLo>";
	std::vector<int64> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteInt64("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Int64_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><int64 key=\"object0\">42</int64><int64 key=\"object1\">10</int64><int64 key=\"object2\">12</int64></arraymap></HighLo>";
	std::vector<int64> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteInt64("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_UInt64_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><uint64 key=\"object0\">42</uint64><uint64 key=\"object1\">10</uint64><uint64 key=\"object2\">12</uint64></arraymap></HighLo>";
	std::vector<uint64> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteUInt64("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_UInt64_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><uint64 key=\"object0\">42</uint64><uint64 key=\"object1\">10</uint64><uint64 key=\"object2\">12</uint64></arraymap></HighLo>";
	std::vector<uint64> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteUInt64("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_UInt64_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><uint64 key=\"object0\">42</uint64><uint64 key=\"object1\">10</uint64><uint64 key=\"object2\">12</uint64></arraymap></HighLo>";
	std::vector<uint64> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(12);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteUInt64("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Bool_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><bool key=\"object0\">1</bool><bool key=\"object1\">0</bool><bool key=\"object2\">1</bool></arraymap></HighLo>";
	std::vector<bool> arr;
	arr.push_back(true);
	arr.push_back(false);
	arr.push_back(true);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteBool("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Bool_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><bool key=\"object0\">1</bool><bool key=\"object1\">0</bool><bool key=\"object2\">1</bool></arraymap></HighLo>";
	std::vector<bool> arr;
	arr.push_back(true);
	arr.push_back(false);
	arr.push_back(true);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteBool("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Bool_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><bool key=\"object0\">1</bool><bool key=\"object1\">0</bool><bool key=\"object2\">1</bool></arraymap></HighLo>";
	std::vector<bool> arr;
	arr.push_back(true);
	arr.push_back(false);
	arr.push_back(true);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteBool("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Float_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><float key=\"object0\">2.5</float><float key=\"object1\">6.3</float><float key=\"object2\">23</float></arraymap></HighLo>";
	std::vector<float> arr;
	arr.push_back(2.5f);
	arr.push_back(6.3f);
	arr.push_back(23.0f);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteFloat("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Float_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><float key=\"object0\">2.5</float><float key=\"object1\">6.3</float><float key=\"object2\">23</float></arraymap></HighLo>";
	std::vector<float> arr;
	arr.push_back(2.5f);
	arr.push_back(6.3f);
	arr.push_back(23.0f);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteFloat("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Float_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><float key=\"object0\">2.5</float><float key=\"object1\">6.3</float><float key=\"object2\">23</float></arraymap></HighLo>";
	std::vector<float> arr;
	arr.push_back(2.5f);
	arr.push_back(6.3f);
	arr.push_back(23.0f);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteFloat("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Double_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><double key=\"object0\">2.5</double><double key=\"object1\">6.3</double><double key=\"object2\">23</double></arraymap></HighLo>";
	std::vector<double> arr;
	arr.push_back(2.5);
	arr.push_back(6.3);
	arr.push_back(23.0);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteDouble("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Double_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><double key=\"object0\">2.5</double><double key=\"object1\">6.3</double><double key=\"object2\">23</double></arraymap></HighLo>";
	std::vector<double> arr;
	arr.push_back(2.5);
	arr.push_back(6.3);
	arr.push_back(23.0);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteDouble("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Double_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><double key=\"object0\">2.5</double><double key=\"object1\">6.3</double><double key=\"object2\">23</double></arraymap></HighLo>";
	std::vector<double> arr;
	arr.push_back(2.5);
	arr.push_back(6.3);
	arr.push_back(23.0);

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteDouble("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Vec2_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><vector2 key=\"object0\"><x>2.5</x><y>2</y></vector2><vector2 key=\"object1\"><x>6.3</x><y>4.5</y></vector2><vector2 key=\"object2\"><x>23</x><y>9.5</y></vector2></arraymap></HighLo>";
	std::vector<glm::vec2> arr;
	arr.push_back({ 2.5f, 2.0f });
	arr.push_back({ 6.3f, 4.5f });
	arr.push_back({ 23.0f, 9.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec2("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec2_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><vector2 key=\"object0\"><x>2.5</x><y>2</y></vector2><vector2 key=\"object1\"><x>6.3</x><y>4.5</y></vector2><vector2 key=\"object2\"><x>23</x><y>9.5</y></vector2></arraymap></HighLo>";
	std::vector<glm::vec2> arr;
	arr.push_back({ 2.5f, 2.0f });
	arr.push_back({ 6.3f, 4.5f });
	arr.push_back({ 23.0f, 9.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec2("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec2_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><vector2 key=\"object0\"><x>2.5</x><y>2</y></vector2><vector2 key=\"object1\"><x>6.3</x><y>4.5</y></vector2><vector2 key=\"object2\"><x>23</x><y>9.5</y></vector2></arraymap></HighLo>";
	std::vector<glm::vec2> arr;
	arr.push_back({ 2.5f, 2.0f });
	arr.push_back({ 6.3f, 4.5f });
	arr.push_back({ 23.0f, 9.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec2("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Vec3_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><vector3 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z></vector3><vector3 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z></vector3><vector3 key=\"object2\"><x>23</x><y>9</y><z>5</z></vector3></arraymap></HighLo>";
	std::vector<glm::vec3> arr;
	arr.push_back({ 2.5f, 2.0f, 2.9f });
	arr.push_back({ 6.3f, 4.5f, 2.0f });
	arr.push_back({ 23.0f, 9.0f, 5.0f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec3("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec3_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><vector3 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z></vector3><vector3 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z></vector3><vector3 key=\"object2\"><x>23</x><y>9</y><z>5</z></vector3></arraymap></HighLo>";
	std::vector<glm::vec3> arr;
	arr.push_back({ 2.5f, 2.0f, 2.9f });
	arr.push_back({ 6.3f, 4.5f, 2.0f });
	arr.push_back({ 23.0f, 9.0f, 5.0f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec3("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec3_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><vector3 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z></vector3><vector3 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z></vector3><vector3 key=\"object2\"><x>23</x><y>9</y><z>5</z></vector3></arraymap></HighLo>";
	std::vector<glm::vec3> arr;
	arr.push_back({ 2.5f, 2.0f, 2.9f });
	arr.push_back({ 6.3f, 4.5f, 2.0f });
	arr.push_back({ 23.0f, 9.0f, 5.0f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec3("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Vec4_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><vector4 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z><w>6</w></vector4><vector4 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z><w>7.5</w></vector4><vector4 key=\"object2\"><x>23</x><y>9.5</y><z>5.5</z><w>1.5</w></vector4></arraymap></HighLo>";
	std::vector<glm::vec4> arr;
	arr.push_back({ 2.5f, 2.0f, 2.9f, 6.0f });
	arr.push_back({ 6.3f, 4.5f, 2.0f, 7.5f });
	arr.push_back({ 23.0f, 9.5f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec4("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec4_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><vector4 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z><w>6</w></vector4><vector4 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z><w>7.5</w></vector4><vector4 key=\"object2\"><x>23</x><y>9.5</y><z>5.5</z><w>1.5</w></vector4></arraymap></HighLo>";
	std::vector<glm::vec4> arr;
	arr.push_back({ 2.5f, 2.0f, 2.9f, 6.0f });
	arr.push_back({ 6.3f, 4.5f, 2.0f, 7.5f });
	arr.push_back({ 23.0f, 9.5f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec4("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Vec4_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><vector4 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z><w>6</w></vector4><vector4 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z><w>7.5</w></vector4><vector4 key=\"object2\"><x>23</x><y>9.5</y><z>5.5</z><w>1.5</w></vector4></arraymap></HighLo>";
	std::vector<glm::vec4> arr;
	arr.push_back({ 2.5f, 2.0f, 2.9f, 6.0f });
	arr.push_back({ 6.3f, 4.5f, 2.0f, 7.5f });
	arr.push_back({ 23.0f, 9.5f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteVec4("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Mat2_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><matrix2 key=\"object0\"><m00>0</m00><m01>0</m01><m10>0</m10><m11>0</m11></matrix2><matrix2 key=\"object1\"><m00>1</m00><m01>0</m01><m10>0</m10><m11>1</m11></matrix2><matrix2 key=\"object2\"><m00>23</m00><m01>9</m01><m10>5.5</m10><m11>1.5</m11></matrix2></arraymap></HighLo>";
	std::vector<glm::mat2> arr;
	arr.push_back(glm::mat2(0.0f));
	arr.push_back(glm::mat2(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat2("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat2_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><matrix2 key=\"object0\"><m00>0</m00><m01>0</m01><m10>0</m10><m11>0</m11></matrix2><matrix2 key=\"object1\"><m00>1</m00><m01>0</m01><m10>0</m10><m11>1</m11></matrix2><matrix2 key=\"object2\"><m00>23</m00><m01>9</m01><m10>5.5</m10><m11>1.5</m11></matrix2></arraymap></HighLo>";
	std::vector<glm::mat2> arr;
	arr.push_back(glm::mat2(0.0f));
	arr.push_back(glm::mat2(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat2("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat2_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><matrix2 key=\"object0\"><m00>0</m00><m01>0</m01><m10>0</m10><m11>0</m11></matrix2><matrix2 key=\"object1\"><m00>1</m00><m01>0</m01><m10>0</m10><m11>1</m11></matrix2><matrix2 key=\"object2\"><m00>23</m00><m01>9</m01><m10>5.5</m10><m11>1.5</m11></matrix2></arraymap></HighLo>";
	std::vector<glm::mat2> arr;
	arr.push_back(glm::mat2(0.0f));
	arr.push_back(glm::mat2(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat2("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Mat3_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><matrix3 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>0</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>0</m22></matrix3><matrix3 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>1</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>1</m22></matrix3><matrix3 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m10>1.5</m10><m11>6</m11><m12>1</m12><m20>7.5</m20><m21>0</m21><m22>4.5</m22></matrix3></arraymap></HighLo>";
	std::vector<glm::mat3> arr;
	arr.push_back(glm::mat3(0.0f));
	arr.push_back(glm::mat3(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat3("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat3_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><matrix3 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>0</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>0</m22></matrix3><matrix3 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>1</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>1</m22></matrix3><matrix3 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m10>1.5</m10><m11>6</m11><m12>1</m12><m20>7.5</m20><m21>0</m21><m22>4.5</m22></matrix3></arraymap></HighLo>";
	std::vector<glm::mat3> arr;
	arr.push_back(glm::mat3(0.0f));
	arr.push_back(glm::mat3(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat3("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat3_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><matrix3 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>0</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>0</m22></matrix3><matrix3 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>1</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>1</m22></matrix3><matrix3 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m10>1.5</m10><m11>6</m11><m12>1</m12><m20>7.5</m20><m21>0</m21><m22>4.5</m22></matrix3></arraymap></HighLo>";
	std::vector<glm::mat3> arr;
	arr.push_back(glm::mat3(0.0f));
	arr.push_back(glm::mat3(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat3("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Mat4_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><matrix4 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></matrix4><matrix4 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></matrix4><matrix4 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></matrix4></arraymap></HighLo>";
	std::vector<glm::mat4> arr;
	arr.push_back(glm::mat4(0.0f));
	arr.push_back(glm::mat4(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat4("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat4_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><matrix4 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></matrix4><matrix4 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></matrix4><matrix4 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></matrix4></arraymap></HighLo>";
	std::vector<glm::mat4> arr;
	arr.push_back(glm::mat4(0.0f));
	arr.push_back(glm::mat4(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat4("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Mat4_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><matrix4 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></matrix4><matrix4 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></matrix4><matrix4 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></matrix4></arraymap></HighLo>";
	std::vector<glm::mat4> arr;
	arr.push_back(glm::mat4(0.0f));
	arr.push_back(glm::mat4(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteMat4("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Quat_Array)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><quaternion key=\"object0\"><x>1</x><y>1</y><z>1</z><w>0</w></quaternion><quaternion key=\"object1\"><x>0</x><y>0</y><z>0</z><w>1</w></quaternion><quaternion key=\"object2\"><x>9</x><y>5.5</y><z>1.5</z><w>23</w></quaternion></arraymap></HighLo>";
	std::vector<glm::quat> arr;
	arr.push_back({ 0.0f, 1.0f, 1.0f, 1.0f });
	arr.push_back({ 1.0f, 0.0f, 0.0f, 0.0f });
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteQuaternion("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Quat_Array_With_Keys)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap key=\"arrayKeys\"><quaternion key=\"object0\"><x>1</x><y>1</y><z>1</z><w>0</w></quaternion><quaternion key=\"object1\"><x>0</x><y>0</y><z>0</z><w>1</w></quaternion><quaternion key=\"object2\"><x>9</x><y>5.5</y><z>1.5</z><w>23</w></quaternion></arraymap></HighLo>";
	std::vector<glm::quat> arr;
	arr.push_back({ 0.0f, 1.0f, 1.0f, 1.0f });
	arr.push_back({ 1.0f, 0.0f, 0.0f, 0.0f });
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteQuaternion("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray("arrayKeys", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Quat_Array_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><arraymap><quaternion key=\"object0\"><x>1</x><y>1</y><z>1</z><w>0</w></quaternion><quaternion key=\"object1\"><x>0</x><y>0</y><z>0</z><w>1</w></quaternion><quaternion key=\"object2\"><x>9</x><y>5.5</y><z>1.5</z><w>23</w></quaternion></arraymap></HighLo>";
	std::vector<glm::quat> arr;
	arr.push_back({ 0.0f, 1.0f, 1.0f, 1.0f });
	arr.push_back({ 1.0f, 0.0f, 0.0f, 0.0f });
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		bool writeSuccess = Writer->WriteQuaternion("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

///
/// Array
///

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Int32)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><int32>42</int32><int32>10</int32><int32>-10</int32></array></HighLo>";
	std::vector<int32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(-10);

	bool writeSuccess = Writer->WriteInt32Array("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Int32_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><int32>42</int32><int32>10</int32><int32>-10</int32></array></HighLo>";
	std::vector<int32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(-10);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt32Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_UInt32)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><uint32>42</uint32><uint32>10</uint32><uint32>100</uint32></array></HighLo>";
	std::vector<uint32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteUInt32Array("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_UInt32_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><uint32>42</uint32><uint32>10</uint32><uint32>100</uint32></array></HighLo>";
	std::vector<uint32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(100);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt32Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Int64)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><int64>452</int64><int64>105</int64><int64>100</int64></array></HighLo>";
	std::vector<int64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteInt64Array("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Int64_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><int64>452</int64><int64>105</int64><int64>100</int64></array></HighLo>";
	std::vector<int64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt64Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_UInt64)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><uint64>452</uint64><uint64>105</uint64><uint64>100</uint64></array></HighLo>";
	std::vector<uint64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteUInt64Array("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_UInt64_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><uint64>452</uint64><uint64>105</uint64><uint64>100</uint64></array></HighLo>";
	std::vector<uint64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt64Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Float)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><float>44</float><float>10.5</float><float>16</float></array></HighLo>";
	std::vector<float> arr;
	arr.push_back(44.0f);
	arr.push_back(10.5f);
	arr.push_back(16.0f);

	bool writeSuccess = Writer->WriteFloatArray("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Float_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><float>44</float><float>10.5</float><float>16</float></array></HighLo>";
	std::vector<float> arr;
	arr.push_back(44.0f);
	arr.push_back(10.5f);
	arr.push_back(16.0f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteFloatArray("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Double)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><double>44.1</double><double>10.5</double><double>16.6</double></array></HighLo>";
	std::vector<double> arr;
	arr.push_back(44.1);
	arr.push_back(10.5);
	arr.push_back(16.6);

	bool writeSuccess = Writer->WriteDoubleArray("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Double_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><double>44.1</double><double>10.5</double><double>16.6</double></array></HighLo>";
	std::vector<double> arr;
	arr.push_back(44.1);
	arr.push_back(10.5);
	arr.push_back(16.6);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteDoubleArray("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Bool)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><bool>1</bool><bool>0</bool><bool>1</bool></array></HighLo>";
	std::vector<bool> arr;
	arr.push_back(true);
	arr.push_back(false);
	arr.push_back(true);

	bool writeSuccess = Writer->WriteBoolArray("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Bool_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><bool>1</bool><bool>0</bool><bool>1</bool></array></HighLo>";
	std::vector<bool> arr;
	arr.push_back(true);
	arr.push_back(false);
	arr.push_back(true);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteBoolArray("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_String)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><string>Hello</string><string>world</string><string>!</string></array></HighLo>";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	bool writeSuccess = Writer->WriteStringArray("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_String_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><string>Hello</string><string>world</string><string>!</string></array></HighLo>";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteStringArray("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Vec2)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><vector2><x>1.5</x><y>6</y></vector2><vector2><x>2.5</x><y>1</y></vector2><vector2><x>1</x><y>0</y></vector2></array></HighLo>";
	std::vector<glm::vec2> arr;
	arr.push_back({ 1.5f, 6.0f });
	arr.push_back({ 2.5f, 1.0f });
	arr.push_back({ 1.0f, 0.0f });

	bool writeSuccess = Writer->WriteVec2Array("", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Vec2_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><vector2><x>1.5</x><y>6</y></vector2><vector2><x>2.5</x><y>1</y></vector2><vector2><x>1</x><y>0</y></vector2></array></HighLo>";
	std::vector<glm::vec2> arr;
	arr.push_back({ 1.5f, 6.0f });
	arr.push_back({ 2.5f, 1.0f });
	arr.push_back({ 1.0f, 0.0f });

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec2Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Vec3)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><vector3><x>1.5</x><y>6</y><z>5.3</z></vector3><vector3><x>2.5</x><y>1</y><z>5.8</z></vector3><vector3><x>1</x><y>0</y><z>5.5</z></vector3></array></HighLo>";
	std::vector<glm::vec3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f });
	arr.push_back({ 2.5f, 1.0f, 5.8f });
	arr.push_back({ 1.0f, 0.0f, 5.5f });

	bool writeSuccess = Writer->WriteVec3Array("", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Vec3_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><vector3><x>1.5</x><y>6</y><z>5.3</z></vector3><vector3><x>2.5</x><y>1</y><z>5.8</z></vector3><vector3><x>1</x><y>0</y><z>5.5</z></vector3></array></HighLo>";
	std::vector<glm::vec3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f });
	arr.push_back({ 2.5f, 1.0f, 5.8f });
	arr.push_back({ 1.0f, 0.0f, 5.5f });

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec3Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Vec4)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><vector4><x>1.5</x><y>6</y><z>5.3</z><w>2.7</w></vector4><vector4><x>2.5</x><y>1</y><z>5.8</z><w>2.9</w></vector4><vector4><x>1</x><y>0</y><z>5.5</z><w>2.5</w></vector4></array></HighLo>";
	std::vector<glm::vec4> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	bool writeSuccess = Writer->WriteVec4Array("", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Vec4_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><vector4><x>1.5</x><y>6</y><z>5.3</z><w>2.7</w></vector4><vector4><x>2.5</x><y>1</y><z>5.8</z><w>2.9</w></vector4><vector4><x>1</x><y>0</y><z>5.5</z><w>2.5</w></vector4></array></HighLo>";
	std::vector<glm::vec4> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec4Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Mat2)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><matrix2><m00>1.5</m00><m01>6</m01><m10>5.3</m10><m11>2.7</m11></matrix2><matrix2><m00>2.5</m00><m01>1</m01><m10>5.8</m10><m11>2.9</m11></matrix2><matrix2><m00>1</m00><m01>0</m01><m10>5.5</m10><m11>2.5</m11></matrix2></array></HighLo>";
	std::vector<glm::mat2> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	bool writeSuccess = Writer->WriteMat2Array("", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Mat2_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><matrix2><m00>1.5</m00><m01>6</m01><m10>5.3</m10><m11>2.7</m11></matrix2><matrix2><m00>2.5</m00><m01>1</m01><m10>5.8</m10><m11>2.9</m11></matrix2><matrix2><m00>1</m00><m01>0</m01><m10>5.5</m10><m11>2.5</m11></matrix2></array></HighLo>";
	std::vector<glm::mat2> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat2Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Mat3)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><matrix3><m00>1.5</m00><m01>6</m01><m02>5.3</m02><m10>2.7</m10><m11>142</m11><m12>155</m12><m20>231</m20><m21>1.753</m21><m22>3571.05</m22></matrix3><matrix3><m00>2.5</m00><m01>1</m01><m02>5.8</m02><m10>2.9</m10><m11>15</m11><m12>152</m12><m20>7451</m20><m21>175</m21><m22>1573.03</m22></matrix3><matrix3><m00>1</m00><m01>0</m01><m02>5.5</m02><m10>2.5</m10><m11>1.2</m11><m12>165</m12><m20>861</m20><m21>341</m21><m22>1375.75</m22></matrix3></array></HighLo>";
	std::vector<glm::mat3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f, 142.0f, 155.0f, 231.0f, 1.753f, 3571.05f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f, 15.0f, 152.0f, 7451.0f, 175.0f, 1573.03f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f, 1.20f, 165.0f, 861.0f, 341.0f, 1375.750f });

	bool writeSuccess = Writer->WriteMat3Array("", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Mat3_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><matrix3><m00>1.5</m00><m01>6</m01><m02>5.3</m02><m10>2.7</m10><m11>142</m11><m12>155</m12><m20>231</m20><m21>1.753</m21><m22>3571.05</m22></matrix3><matrix3><m00>2.5</m00><m01>1</m01><m02>5.8</m02><m10>2.9</m10><m11>15</m11><m12>152</m12><m20>7451</m20><m21>175</m21><m22>1573.03</m22></matrix3><matrix3><m00>1</m00><m01>0</m01><m02>5.5</m02><m10>2.5</m10><m11>1.2</m11><m12>165</m12><m20>861</m20><m21>341</m21><m22>1375.75</m22></matrix3></array></HighLo>";
	std::vector<glm::mat3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f, 142.0f, 155.0f, 231.0f, 1.753f, 3571.05f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f, 15.0f, 152.0f, 7451.0f, 175.0f, 1573.03f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f, 1.20f, 165.0f, 861.0f, 341.0f, 1375.750f });

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat3Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Mat4)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><matrix4><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></matrix4><matrix4><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></matrix4><matrix4><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></matrix4></array></HighLo>";
	std::vector<glm::mat4> arr;
	arr.push_back(glm::mat4(0.0f));
	arr.push_back(glm::mat4(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	bool writeSuccess = Writer->WriteMat4Array("", arr);

	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Mat4_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><matrix4><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></matrix4><matrix4><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></matrix4><matrix4><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></matrix4></array></HighLo>";
	std::vector<glm::mat4> arr;
	arr.push_back(glm::mat4(0.0f));
	arr.push_back(glm::mat4(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat4Array("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Quat)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array><quaternion><x>6</x><y>5.3</y><z>2.7</z><w>1.5</w></quaternion><quaternion><x>326.5</x><y>235.3</y><z>32</z><w>164.5</w></quaternion><quaternion><x>32.5</x><y>23.53</y><z>3.2</z><w>14.5</w></quaternion></array></HighLo>";
	std::vector<glm::quat> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 164.5f, 326.5f, 235.3f, 32.0f });
	arr.push_back({ 14.5f, 32.5f, 23.53f, 3.20f });

	bool writeSuccess = Writer->WriteQuaternionArray("", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(XMLWriteParserTests, XMLParser_Write_Complete_Array_Quat_WithTypeCheck)
{
	HLString expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo><array key=\"test\"><quaternion><x>6</x><y>5.3</y><z>2.7</z><w>1.5</w></quaternion><quaternion><x>326.5</x><y>235.3</y><z>32</z><w>164.5</w></quaternion><quaternion><x>32.5</x><y>23.53</y><z>3.2</z><w>14.5</w></quaternion></array></HighLo>";
	std::vector<glm::quat> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 164.5f, 326.5f, 235.3f, 32.0f });
	arr.push_back({ 14.5f, 32.5f, 23.53f, 3.20f });

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteQuaternionArray("test", arr);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

