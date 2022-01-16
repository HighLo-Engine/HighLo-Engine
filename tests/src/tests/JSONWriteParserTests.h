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
	bool writeSuccess = Writer->WriteString("test", "Hello World!");
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_String_WithTypeCheck)
{
	HLString expected = "{\"type\":\"string\",\"value\":{\"test\":\"Hello World!\"}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteString("test", "Hello World!");
	Writer->EndObject();
	HLString content = Writer->GetContent();

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Int32)
{
	HLString expected = "{\"test\":-10}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt32("test", -10);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Int32_WithTypeCheck)
{
	HLString expected = "{\"type\":\"int32\",\"value\":{\"test\":-10}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt32("test", -10);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_UInt32)
{
	HLString expected = "{\"test\":42}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt32("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt32_WithTypeCheck)
{
	HLString expected = "{\"type\":\"uint32\",\"value\":{\"test\":42}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt32("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Int64)
{
	HLString expected = "{\"test\":42}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Int64_WithTypeCheck)
{
	HLString expected = "{\"type\":\"int64\",\"value\":{\"test\":42}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt64("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_UInt64)
{
	HLString expected = "{\"test\":42}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt64_WithTypeCheck)
{
	HLString expected = "{\"type\":\"uint64\",\"value\":{\"test\":42}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt64("test", 42);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Bool)
{
	HLString expected = "{\"test\":true}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteBool("test", true);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Bool_WithTypeCheck)
{
	HLString expected = "{\"type\":\"bool\",\"value\":{\"test\":true}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteBool("test", true);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Float)
{
	HLString expected = "{\"test\":20.2}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteFloat("test", 20.2f);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Float_WithTypeCheck)
{
	HLString expected = "{\"type\":\"float\",\"value\":{\"test\":20.2}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteFloat("test", 20.2f);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Double)
{
	HLString expected = "{\"test\":20.2}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteDouble("test", 20.2);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Double_WithTypeCheck)
{
	HLString expected = "{\"type\":\"double\",\"value\":{\"test\":20.2}}";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteDouble("test", 20.2);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Vec2)
{
	HLString expected = "{\"test\":[20.2,10.5]}";
	glm::vec2 v(20.2f, 10.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec2("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec2_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec2\",\"value\":{\"test\":[20.2,10.5]}}";
	glm::vec2 v(20.2f, 10.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec2("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Vec3)
{
	HLString expected = "{\"test\":[20.2,10.5,23.5]}";
	glm::vec3 v(20.2f, 10.5f, 23.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec3("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec3_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec3\",\"value\":{\"test\":[20.2,10.5,23.5]}}";
	glm::vec3 v(20.2f, 10.5f, 23.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec3("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Vec4)
{
	HLString expected = "{\"test\":[20.2,10.5,23.5,11.5]}";
	glm::vec4 v(20.2f, 10.5f, 23.5f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec4("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec4_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec4\",\"value\":{\"test\":[20.2,10.5,23.5,11.5]}}";
	glm::vec4 v(20.2f, 10.5f, 23.5f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec4("test", v);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
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
	bool writeSuccess = Writer->WriteMat2("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
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
	bool writeSuccess = Writer->WriteMat2("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
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
	bool writeSuccess = Writer->WriteMat3("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
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
	bool writeSuccess = Writer->WriteMat3("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
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
	bool writeSuccess = Writer->WriteMat4("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
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
	bool writeSuccess = Writer->WriteMat4("test", m);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Quaternion)
{
	HLString expected = "{\"test\":[20.2,10.5,23.1,11.5]}";
	glm::quat q(20.2f, 10.5f, 23.1f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteQuaternion("test", q);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Quaternion_WithTypeCheck)
{
	HLString expected = "{\"type\":\"quat\",\"value\":{\"test\":[20.2,10.5,23.1,11.5]}}";
	glm::quat q(20.2f, 10.5f, 23.1f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteQuaternion("test", q);
	Writer->EndObject();

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
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
		bool writeSuccess = Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
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
		bool writeSuccess = Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
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
		bool writeSuccess = Writer->WriteString("object" + HLString::ToString(i), arr[i]);
		Writer->EndObject();

		EXPECT_EQ(writeSuccess, true);
	}
	Writer->EndArray();

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(JsonWriteParserTests, JSONParser_Write_Int32_Array)
{
	HLString expected = "[{\"object0\":42},{\"object1\":10},{\"object2\":-10}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Int32_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":-10}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Int32_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"int32\",\"value\":{\"object0\":42}},{\"type\":\"int32\",\"value\":{\"object1\":10}},{\"type\":\"int32\",\"value\":{\"object2\":-10}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_UInt32_Array)
{
	HLString expected = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt32_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":12}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt32_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"uint32\",\"value\":{\"object0\":42}},{\"type\":\"uint32\",\"value\":{\"object1\":10}},{\"type\":\"uint32\",\"value\":{\"object2\":12}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Int64_Array)
{
	HLString expected = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Int64_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":12}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Int64_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"int64\",\"value\":{\"object0\":42}},{\"type\":\"int64\",\"value\":{\"object1\":10}},{\"type\":\"int64\",\"value\":{\"object2\":12}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_UInt64_Array)
{
	HLString expected = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt64_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":12}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_UInt64_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"uint64\",\"value\":{\"object0\":42}},{\"type\":\"uint64\",\"value\":{\"object1\":10}},{\"type\":\"uint64\",\"value\":{\"object2\":12}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Bool_Array)
{
	HLString expected = "[{\"object0\":true},{\"object1\":false},{\"object2\":true}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Bool_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":true},{\"object1\":false},{\"object2\":true}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Bool_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"bool\",\"value\":{\"object0\":true}},{\"type\":\"bool\",\"value\":{\"object1\":false}},{\"type\":\"bool\",\"value\":{\"object2\":true}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Float_Array)
{
	HLString expected = "[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Float_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Float_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"float\",\"value\":{\"object0\":2.5}},{\"type\":\"float\",\"value\":{\"object1\":6.3}},{\"type\":\"float\",\"value\":{\"object2\":23.0}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Double_Array)
{
	HLString expected = "[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Double_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Double_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"double\",\"value\":{\"object0\":2.5}},{\"type\":\"double\",\"value\":{\"object1\":6.3}},{\"type\":\"double\",\"value\":{\"object2\":23.0}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Vec2_Array)
{
	HLString expected = "[{\"object0\":[2.5,2.0]},{\"object1\":[6.3,4.5]},{\"object2\":[23.0,9.5]}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec2_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":[2.5,2.0]},{\"object1\":[6.3,4.5]},{\"object2\":[23.0,9.5]}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec2_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"vec2\",\"value\":{\"object0\":[2.5,2.0]}},{\"type\":\"vec2\",\"value\":{\"object1\":[6.3,4.5]}},{\"type\":\"vec2\",\"value\":{\"object2\":[23.0,9.5]}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Vec3_Array)
{
	HLString expected = "[{\"object0\":[2.5,2.0,2.9]},{\"object1\":[6.3,4.5,2.0]},{\"object2\":[23.0,9.0,5.0]}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec3_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":[2.5,2.0,2.9]},{\"object1\":[6.3,4.5,2.0]},{\"object2\":[23.0,9.0,5.0]}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec3_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"vec3\",\"value\":{\"object0\":[2.5,2.0,2.9]}},{\"type\":\"vec3\",\"value\":{\"object1\":[6.3,4.5,2.0]}},{\"type\":\"vec3\",\"value\":{\"object2\":[23.0,9.0,5.0]}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Vec4_Array)
{
	HLString expected = "[{\"object0\":[2.5,2.0,2.9,6.0]},{\"object1\":[6.3,4.5,2.0,7.5]},{\"object2\":[23.0,9.5,5.5,1.5]}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec4_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":[2.5,2.0,2.9,6.0]},{\"object1\":[6.3,4.5,2.0,7.5]},{\"object2\":[23.0,9.5,5.5,1.5]}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Vec4_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"vec4\",\"value\":{\"object0\":[2.5,2.0,2.9,6.0]}},{\"type\":\"vec4\",\"value\":{\"object1\":[6.3,4.5,2.0,7.5]}},{\"type\":\"vec4\",\"value\":{\"object2\":[23.0,9.5,5.5,1.5]}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Mat2_Array)
{
	HLString expected = "[{\"object0\":[0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat2_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":[0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat2_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"mat2\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0]}},{\"type\":\"mat2\",\"value\":{\"object1\":[1.0,0.0,0.0,1.0]}},{\"type\":\"mat2\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5]}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Mat3_Array)
{
	HLString expected = "[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat3_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat3_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"mat3\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]}},{\"type\":\"mat3\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]}},{\"type\":\"mat3\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Mat4_Array)
{
	HLString expected = "[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat4_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Mat4_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"mat4\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]}},{\"type\":\"mat4\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]}},{\"type\":\"mat4\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}}]";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Quat_Array)
{
	HLString expected = "[{\"object0\":[0.0,1.0,1.0,1.0]},{\"object1\":[1.0,0.0,0.0,0.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Quat_Array_With_Keys)
{
	HLString expected = "{\"arrayKeys\":[{\"object0\":[0.0,1.0,1.0,1.0]},{\"object1\":[1.0,0.0,0.0,0.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]}";
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

TEST_F(JsonWriteParserTests, JSONParser_Write_Quat_Array_WithTypeCheck)
{
	HLString expected = "[{\"type\":\"quat\",\"value\":{\"object0\":[0.0,1.0,1.0,1.0]}},{\"type\":\"quat\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0]}},{\"type\":\"quat\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5]}}]";
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
/// Direct Array Tests
///

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Int32)
{
	HLString expected = "{\"test\":[42,10,-10]}";
	std::vector<int32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(-10);

	bool writeSuccess = Writer->WriteInt32Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Int32_WithTypeCheck)
{
	HLString expected = "{\"type\":\"int32\",\"value\":{\"test\":[42,10,-10]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_UInt32)
{
	HLString expected = "{\"test\":[42,10,100]}";
	std::vector<uint32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteUInt32Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_UInt32_WithTypeCheck)
{
	HLString expected = "{\"type\":\"uint32\",\"value\":{\"test\":[42,10,100]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Int64)
{
	HLString expected = "{\"test\":[452,105,100]}";
	std::vector<int64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteInt64Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Int64_WithTypeCheck)
{
	HLString expected = "{\"type\":\"int64\",\"value\":{\"test\":[452,105,100]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_UInt64)
{
	HLString expected = "{\"test\":[452,105,100]}";
	std::vector<uint64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteUInt64Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_UInt64_WithTypeCheck)
{
	HLString expected = "{\"type\":\"uint64\",\"value\":{\"test\":[452,105,100]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Float)
{
	HLString expected = "{\"test\":[44.0,10.5,16.0]}";
	std::vector<float> arr;
	arr.push_back(44.0f);
	arr.push_back(10.5f);
	arr.push_back(16.0f);

	bool writeSuccess = Writer->WriteFloatArray("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Float_WithTypeCheck)
{
	HLString expected = "{\"type\":\"float\",\"value\":{\"test\":[44.0,10.5,16.0]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Double)
{
	HLString expected = "{\"test\":[44.1,10.5,16.6]}";
	std::vector<double> arr;
	arr.push_back(44.1);
	arr.push_back(10.5);
	arr.push_back(16.6);

	bool writeSuccess = Writer->WriteDoubleArray("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Double_WithTypeCheck)
{
	HLString expected = "{\"type\":\"double\",\"value\":{\"test\":[44.1,10.5,16.6]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Bool)
{
	HLString expected = "{\"test\":[true,false,true]}";
	std::vector<bool> arr;
	arr.push_back(true);
	arr.push_back(false);
	arr.push_back(true);

	bool writeSuccess = Writer->WriteBoolArray("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Bool_WithTypeCheck)
{
	HLString expected = "{\"type\":\"bool\",\"value\":{\"test\":[true,false,true]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_String)
{
	HLString expected = "{\"test\":[\"Hello\",\"world\",\"!\"]}";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	bool writeSuccess = Writer->WriteStringArray("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_String_WithTypeCheck)
{
	HLString expected = "{\"type\":\"string\",\"value\":{\"test\":[\"Hello\",\"world\",\"!\"]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Vec2)
{
	HLString expected = "{\"test\":[[1.5,6.0],[2.5,1.0],[1.0,0.0]]}";
	std::vector<glm::vec2> arr;
	arr.push_back({ 1.5f, 6.0f });
	arr.push_back({ 2.5f, 1.0f });
	arr.push_back({ 1.0f, 0.0f });

	bool writeSuccess = Writer->WriteVec2Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Vec2_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec2\",\"value\":{\"test\":[[1.5,6.0],[2.5,1.0],[1.0,0.0]]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Vec3)
{
	HLString expected = "{\"test\":[[1.5,6.0,5.3],[2.5,1.0,5.8],[1.0,0.0,5.5]]}";
	std::vector<glm::vec3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f });
	arr.push_back({ 2.5f, 1.0f, 5.8f });
	arr.push_back({ 1.0f, 0.0f, 5.5f });

	bool writeSuccess = Writer->WriteVec3Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Vec3_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec3\",\"value\":{\"test\":[[1.5,6.0,5.3],[2.5,1.0,5.8],[1.0,0.0,5.5]]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Vec4)
{
	HLString expected = "{\"test\":[[1.5,6.0,5.3,2.7],[2.5,1.0,5.8,2.9],[1.0,0.0,5.5,2.5]]}";
	std::vector<glm::vec4> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	bool writeSuccess = Writer->WriteVec4Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Vec4_WithTypeCheck)
{
	HLString expected = "{\"type\":\"vec4\",\"value\":{\"test\":[[1.5,6.0,5.3,2.7],[2.5,1.0,5.8,2.9],[1.0,0.0,5.5,2.5]]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Mat2)
{
	HLString expected = "{\"test\":[[1.5,6.0,5.3,2.7],[2.5,1.0,5.8,2.9],[1.0,0.0,5.5,2.5]]}";
	std::vector<glm::mat2> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	bool writeSuccess = Writer->WriteMat2Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Mat2_WithTypeCheck)
{
	HLString expected = "{\"type\":\"mat2\",\"value\":{\"test\":[[1.5,6.0,5.3,2.7],[2.5,1.0,5.8,2.9],[1.0,0.0,5.5,2.5]]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Mat3)
{
	HLString expected = "{\"test\":[[1.5,6.0,5.3,2.7,142.0,155.0,231.0,1.753,3571.05],[2.5,1.0,5.8,2.9,15.0,152.0,7451.0,175.0,1573.03],[1.0,0.0,5.5,2.5,1.2,165.0,861.0,341.0,1375.75]]}";
	std::vector<glm::mat3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f, 142.0f, 155.0f, 231.0f, 1.753f, 3571.05f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f, 15.0f, 152.0f, 7451.0f, 175.0f, 1573.03f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f, 1.20f, 165.0f, 861.0f, 341.0f, 1375.750f });

	bool writeSuccess = Writer->WriteMat3Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Mat3_WithTypeCheck)
{
	HLString expected = "{\"type\":\"mat3\",\"value\":{\"test\":[[1.5,6.0,5.3,2.7,142.0,155.0,231.0,1.753,3571.05],[2.5,1.0,5.8,2.9,15.0,152.0,7451.0,175.0,1573.03],[1.0,0.0,5.5,2.5,1.2,165.0,861.0,341.0,1375.75]]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Mat4)
{
	HLString expected = "{\"test\":[[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0],[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0],[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]]}";
	std::vector<glm::mat4> arr;
	arr.push_back(glm::mat4(0.0f));
	arr.push_back(glm::mat4(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	bool writeSuccess = Writer->WriteMat4Array("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Mat4_WithTypeCheck)
{
	HLString expected = "{\"type\":\"mat4\",\"value\":{\"test\":[[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0],[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0],[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]]}}";
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


TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Quat)
{
	HLString expected = "{\"test\":[[1.5,6.0,5.3,2.7],[164.5,326.5,235.3,32.0],[14.5,32.5,23.53,3.2]]}";
	std::vector<glm::quat> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 164.5f, 326.5f, 235.3f, 32.0f });
	arr.push_back({ 14.5f, 32.5f, 23.53f, 3.20f });

	bool writeSuccess = Writer->WriteQuaternionArray("test", arr);

	HLString content = Writer->GetContent();
	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(JsonWriteParserTests, JSONParser_Write_Complete_Array_Quat_WithTypeCheck)
{
	HLString expected = "{\"type\":\"quat\",\"value\":{\"test\":[[1.5,6.0,5.3,2.7],[164.5,326.5,235.3,32.0],[14.5,32.5,23.53,3.2]]}}";
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

