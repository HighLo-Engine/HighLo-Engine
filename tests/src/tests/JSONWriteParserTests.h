// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-12) initial release
//

#pragma once

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace highlo;

struct JsonWriteParserTests : public testing::Test
{
	Ref<DocumentWriter> Writer;

	JsonWriteParserTests()
	{
		Writer = DocumentWriter::Create("", DocumentType::Json);
	}

	virtual ~JsonWriteParserTests()
	{
	}
};

TEST_F(JsonWriteParserTests, JSONParser_Write_String)
{
	HLString expected = "{\"test\":\"Hello World!\"}";

	Writer->BeginObject();
	Writer->WriteString("test", "Hello World!");
	Writer->EndObject(true);
	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_String_WithTypeCheck)
{
	HLString expected = "{\"type\":\"string\",\"value\":{\"test\":\"Hello World!\"}}";

	Writer->BeginObject();
	Writer->WriteString("test", "Hello World!");
	Writer->EndObject();
	HLString content = Writer->GetContent();

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Int32)
{
	HLString expected = "{\"test\":-10}";

	Writer->BeginObject();
	Writer->WriteInt32("test", -10);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Int32_WithTypeCheck)
{
	HLString expected = "{\"type\":\"int32\",\"value\":{\"test\":-10}}";

	Writer->BeginObject();
	Writer->WriteInt32("test", -10);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt32)
{
	HLString expected = "{\"test\":42}";

	Writer->BeginObject();
	Writer->WriteUInt32("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt32_WithTypeCheck)
{
	HLString expected = "{\"type\":\"uint32\",\"value\":{\"test\":42}}";

	Writer->BeginObject();
	Writer->WriteUInt32("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Int64)
{
	HLString expected = "{\"test\":42}";

	Writer->BeginObject();
	Writer->WriteInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Int64_WithTypeCheck)
{
	HLString expected = "{\"type\":\"int64\",\"value\":{\"test\":42}}";

	Writer->BeginObject();
	Writer->WriteInt64("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt64)
{
	HLString expected = "{\"test\":42}";

	Writer->BeginObject();
	Writer->WriteUInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt64_WithTypeCheck)
{
	HLString expected = "{\"type\":\"uint64\",\"value\":{\"test\":42}}";

	Writer->BeginObject();
	Writer->WriteUInt64("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Bool)
{
	HLString expected = "{\"test\":true}";

	Writer->BeginObject();
	Writer->WriteBool("test", true);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Bool_WithTypeCheck)
{
	HLString expected = "{\"type\":\"bool\",\"value\":{\"test\":true}}";

	Writer->BeginObject();
	Writer->WriteBool("test", true);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Float)
{
	HLString expected = "{\"test\":20.2}";

	Writer->BeginObject();
	Writer->WriteFloat("test", 20.2f);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Float_WithTypeCheck)
{
	HLString expected = "{\"type\":\"float\",\"value\":{\"test\":20.2}}";

	Writer->BeginObject();
	Writer->WriteFloat("test", 20.2f);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Double)
{
	HLString expected = "{\"test\":20.2}";

	Writer->BeginObject();
	Writer->WriteDouble("test", 20.2);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Double_WithTypeCheck)
{
	HLString expected = "{\"type\":\"double\",\"value\":{\"test\":20.2}}";

	Writer->BeginObject();
	Writer->WriteDouble("test", 20.2);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec2)
{
	HLString expected = "{\"test\":[20.2,10.5]}";
	glm::vec2 v(20.2f, 10.5f);

	Writer->BeginObject();
	Writer->WriteVec2("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec2_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec2\",\"value\":{\"test\":[20.2,10.5]}}";
	glm::vec2 v(20.2f, 10.5f);

	Writer->BeginObject();
	Writer->WriteVec2("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec3)
{
	HLString expected = "{\"test\":[20.2,10.5,23.5]}";
	glm::vec3 v(20.2f, 10.5f, 23.5f);

	Writer->BeginObject();
	Writer->WriteVec3("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec3_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec3\",\"value\":{\"test\":[20.2,10.5,23.5]}}";
	glm::vec3 v(20.2f, 10.5f, 23.5f);

	Writer->BeginObject();
	Writer->WriteVec3("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec4)
{
	HLString expected = "{\"test\":[20.2,10.5,23.5,11.5]}";
	glm::vec4 v(20.2f, 10.5f, 23.5f, 11.5f);

	Writer->BeginObject();
	Writer->WriteVec4("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec4_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec4\",\"value\":{\"test\":[20.2,10.5,23.5,11.5]}}";
	glm::vec4 v(20.2f, 10.5f, 23.5f, 11.5f);

	Writer->BeginObject();
	Writer->WriteVec4("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat2)
{
	HLString expected = "{\"test\":[20.2,10.5,23.5,11.5]}";

	glm::mat2 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[1][0] = 23.5f;
	m[1][1] = 11.5f;

	Writer->BeginObject();
	Writer->WriteMat2("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat2_WithTypeCheck)
{
	HLString expected = "{\"type\":\"mat2\",\"value\":{\"test\":[20.2,10.5,23.5,11.5]}}";

	glm::mat2 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[1][0] = 23.5f;
	m[1][1] = 11.5f;

	Writer->BeginObject();
	Writer->WriteMat2("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat3)
{
	HLString expected = "{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4]}";

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
	Writer->WriteMat3("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat3_WithTypeCheck)
{
	HLString expected = "{\"type\":\"mat3\",\"value\":{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4]}}";

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
	Writer->WriteMat3("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat4)
{
	HLString expected = "{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4,47.2,55.4,48.4,32.0,15.1,499.1,22.5]}";

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
	Writer->WriteMat4("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat4_WithTypeCheck)
{
	HLString expected = "{\"type\":\"mat4\",\"value\":{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4,47.2,55.4,48.4,32.0,15.1,499.1,22.5]}}";

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
	Writer->WriteMat4("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Quaternion)
{
	HLString expected = "{\"test\":[20.2,10.5,23.1,11.5]}";
	glm::quat q(20.2f, 10.5f, 23.1f, 11.5f);

	Writer->BeginObject();
	Writer->WriteQuaternion("test", q);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Quaternion_WithTypeCheck)
{
	HLString expected = "{\"type\":\"quat\",\"value\":{\"test\":[20.2,10.5,23.1,11.5]}}";
	glm::quat q(20.2f, 10.5f, 23.1f, 11.5f);

	Writer->BeginObject();
	Writer->WriteQuaternion("test", q);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

///
/// Array
/// 

TEST_F(JsonWriteParserTests, JSONParser_Write_String_Array)
{
	HLString expected = "[{\"object0\":\"Hello\"},{\"object1\":\"world\"},{\"object2\":\"!\"}]";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();
	}
	Writer->EndArray("", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_String_Array_With_Keys)
{
	HLString expected = "{\"arrayKey\":[{\"object0\":\"Hello\"},{\"object1\":\"world\"},{\"object2\":\"!\"}]}";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();
	}
	Writer->EndArray("arrayKey", true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_String_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"string\",\"value\":{\"object0\":\"Hello\"}},{\"type\":\"string\",\"value\":{\"object1\":\"world\"}},{\"type\":\"string\",\"value\":{\"object2\":\"!\"}}]";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	Writer->BeginArray();
	for (uint32 i = 0; i < arr.size(); ++i)
	{
		Writer->BeginObject();
		Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

