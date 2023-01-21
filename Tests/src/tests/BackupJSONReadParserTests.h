// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-12) initial release
//

#pragma once

#if 0

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace highlo;

struct JsonReadParserTests : public testing::Test
{
	Ref<DocumentWriter> Reader;

	JsonReadParserTests()
	{
		Reader = DocumentWriter::Create("", DocumentType::Json);
		Logger::Init();
	}

	virtual ~JsonReadParserTests()
	{
		Logger::Shutdown();
	}
};

TEST_F(JsonReadParserTests, JSONParser_Read_String)
{
	HLString input = "{\"test\":\"Hello World!\"}";
	HLString expected = "Hello World!";
	HLString result = "";

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadString("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_String_WithTypeCheck)
{
	HLString input = "{\"type\":\"string\",\"value\":{\"test\":\"Hello World!\"}}";
	HLString expected = "Hello World!";
	HLString result = "";

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadString("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int32)
{
	HLString input = "{\"test\":-10}";
	int32 expected = -10;
	int32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int32_WithTypeCheck)
{
	HLString input = "{\"type\":\"int32\",\"value\":{\"test\":-10}}";
	int32 expected = -10;
	int32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt32)
{
	HLString input = "{\"test\":42}";
	uint32 expected = 42;
	uint32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt32_WithTypeCheck)
{
	HLString input = "{\"type\":\"uint32\",\"value\":{\"test\":42}}";
	uint32 expected = 42;
	uint32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int64)
{
	HLString input = "{\"test\":42}";
	int64 expected = 42;
	int64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int64_WithTypeCheck)
{
	HLString input = "{\"type\":\"int64\",\"value\":{\"test\":42}}";
	int64 expected = 42;
	int64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt64)
{
	HLString input = "{\"test\":42}";
	uint64 expected = 42;
	uint64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt64_WithTypeCheck)
{
	HLString input = "{\"type\":\"uint64\",\"value\":{\"test\":42}}";
	uint64 expected = 42;
	uint64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Bool)
{
	HLString input = "{\"test\":true}";
	bool expected = true;
	bool result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBool("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Bool_WithTypeCheck)
{
	HLString input = "{\"type\":\"bool\",\"value\":{\"test\":true}}";
	bool expected = true;
	bool result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBool("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Float)
{
	HLString input = "{\"test\":20.2}";
	float expected = 20.2f;
	float result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloat("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Float_WithTypeCheck)
{
	HLString input = "{\"type\":\"float\",\"value\":{\"test\":20.2}}";
	float expected = 20.2f;
	float result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloat("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Double)
{
	HLString input = "{\"test\":20.2}";
	double expected = 20.2;
	double result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDouble("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Double_WithTypeCheck)
{
	HLString input = "{\"type\":\"double\",\"value\":{\"test\":20.2}}";
	double expected = 20.2;
	double result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDouble("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec2)
{
	HLString input = "{\"test\":[20.2,10.5]}";
	glm::vec2 expected = { 20.2f, 10.5f };
	glm::vec2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector2("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec2_WithTypeCheck)
{
	HLString input = "{\"type\":\"vec2\",\"value\":{\"test\":[20.2,10.5]}}";
	glm::vec2 expected = { 20.2f, 10.5f };
	glm::vec2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector2("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec3)
{
	HLString input = "{\"test\":[20.2,10.5,23.5]}";
	glm::vec3 expected = { 20.2f, 10.5f, 23.5f };
	glm::vec3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector3("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec3_WithTypeCheck)
{
	HLString input = "{\"type\":\"vec3\",\"value\":{\"test\":[20.2,10.5,23.5]}}";
	glm::vec3 expected = { 20.2f, 10.5f, 23.5f };
	glm::vec3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector3("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec4)
{
	HLString input = "{\"test\":[20.2,10.5,23.5,11.5]}";
	glm::vec4 expected = { 20.2f, 10.5f, 23.5f, 11.5f };
	glm::vec4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector4("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec4_WithTypeCheck)
{
	HLString input = "{\"type\":\"vec4\",\"value\":{\"test\":[20.2,10.5,23.5,11.5]}}";
	glm::vec4 expected = { 20.2f, 10.5f, 23.5f, 11.5f };
	glm::vec4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector4("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat2)
{
	HLString input = "{\"test\":[20.2,10.5,23.5,11.5]}";
	glm::mat2 expected = glm::mat2(20.2f, 10.5f, 23.5f, 11.5f);
	glm::mat2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix2("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat2_WithTypeCheck)
{
	HLString input = "{\"type\":\"mat2\",\"value\":{\"test\":[20.2,10.5,23.5,11.5]}}";
	glm::mat2 expected = glm::mat2(20.2f, 10.5f, 23.5f, 11.5f);
	glm::mat2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix2("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat3)
{
	HLString input = "{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4]}";
	glm::mat3 expected = glm::mat3(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f);
	glm::mat3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix3("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat3_WithTypeCheck)
{
	HLString input = "{\"type\":\"mat3\",\"value\":{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4]}}";
	glm::mat3 expected = glm::mat3(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f);
	glm::mat3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix3("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat4)
{
	HLString input = "{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4,47.2,55.4,48.4,32.0,15.1,499.1,22.5]}";
	glm::mat4 expected = glm::mat4(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f, 47.2f, 55.4f, 48.4f, 32.0f, 15.1f, 499.1f, 22.5f);
	glm::mat4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix4("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat4_WithTypeCheck)
{
	HLString input = "{\"type\":\"mat4\",\"value\":{\"test\":[20.2,10.5,23.5,11.5,534.5,234.5,12.5,45.7,42.4,47.2,55.4,48.4,32.0,15.1,499.1,22.5]}}";
	glm::mat4 expected = glm::mat4(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f, 47.2f, 55.4f, 48.4f, 32.0f, 15.1f, 499.1f, 22.5f);
	glm::mat4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix4("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Quaternion)
{
	HLString input = "{\"test\":[20.2,10.5,23.1,11.5]}";
	glm::quat expected = glm::quat(20.2f, 10.5f, 23.1f, 11.5f);
	glm::quat result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuaternion("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Quaternion_WithTypeCheck)
{
	HLString input = "{\"type\":\"quat\",\"value\":{\"test\":[20.2,10.5,23.1,11.5]}}";
	glm::quat expected = glm::quat(20.2f, 10.5f, 23.1f, 11.5f);
	glm::quat result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuaternion("test", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

///
/// Array
/// 

TEST_F(JsonReadParserTests, JSONParser_Read_String_Array)
{
	HLString input = "[{\"object0\":\"Hello\"},{\"object1\":\"world\"},{\"object2\":\"!\"}]";
	std::vector<HLString> expected;
	std::vector<HLString> result;

	expected.push_back("Hello");
	expected.push_back("world");
	expected.push_back("!");

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadStringArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_String_Array_With_Keys)
{
	HLString input = "{\"arrayKey\":[{\"object0\":\"Hello\"},{\"object1\":\"world\"},{\"object2\":\"!\"}]}";
	std::vector<HLString> expected;
	std::vector<HLString> result;

	expected.push_back("Hello");
	expected.push_back("world");
	expected.push_back("!");

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadStringArray("arrayKey", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_String_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"string\",\"value\":{\"object0\":\"Hello\"}},{\"type\":\"string\",\"value\":{\"object1\":\"world\"}},{\"type\":\"string\",\"value\":{\"object2\":\"!\"}}]";
	std::vector<HLString> expected;
	std::vector<HLString> result;

	expected.push_back("Hello");
	expected.push_back("world");
	expected.push_back("!");

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadStringArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Int32_Array)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":-10}]";
	std::vector<int32> expected;
	std::vector<int32> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(-10);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int32_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":-10}]}";
	std::vector<int32> expected;
	std::vector<int32> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(-10);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int32_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"int32\",\"value\":{\"object0\":42}},{\"type\":\"int32\",\"value\":{\"object1\":10}},{\"type\":\"int32\",\"value\":{\"object2\":-10}}]";
	std::vector<int32> expected;
	std::vector<int32> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(-10);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_UInt32_Array)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
	std::vector<uint32> expected;
	std::vector<uint32> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt32_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":12}]}";
	std::vector<uint32> expected;
	std::vector<uint32> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt32_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"uint32\",\"value\":{\"object0\":42}},{\"type\":\"uint32\",\"value\":{\"object1\":10}},{\"type\":\"uint32\",\"value\":{\"object2\":12}}]";
	std::vector<uint32> expected;
	std::vector<uint32> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Int64_Array)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
	std::vector<int64> expected;
	std::vector<int64> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int64_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":12}]}";
	std::vector<int64> expected;
	std::vector<int64> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Int64_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"int64\",\"value\":{\"object0\":42}},{\"type\":\"int64\",\"value\":{\"object1\":10}},{\"type\":\"int64\",\"value\":{\"object2\":12}}]";
	std::vector<int64> expected;
	std::vector<int64> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_UInt64_Array)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
	std::vector<uint64> expected;
	std::vector<uint64> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt64_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":42},{\"object1\":10},{\"object2\":12}]}";
	std::vector<uint64> expected;
	std::vector<uint64> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_UInt64_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"uint64\",\"value\":{\"object0\":42}},{\"type\":\"uint64\",\"value\":{\"object1\":10}},{\"type\":\"uint64\",\"value\":{\"object2\":12}}]";
	std::vector<uint64> expected;
	std::vector<uint64> result;

	expected.push_back(42);
	expected.push_back(10);
	expected.push_back(12);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Bool_Array)
{
	HLString input = "[{\"object0\":true},{\"object1\":false},{\"object2\":true}]";
	std::vector<bool> expected;
	std::vector<bool> result;

	expected.push_back(true);
	expected.push_back(false);
	expected.push_back(true);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBoolArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Bool_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":true},{\"object1\":false},{\"object2\":true}]}";
	std::vector<bool> expected;
	std::vector<bool> result;

	expected.push_back(true);
	expected.push_back(false);
	expected.push_back(true);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBoolArray("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Bool_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"bool\",\"value\":{\"object0\":true}},{\"type\":\"bool\",\"value\":{\"object1\":false}},{\"type\":\"bool\",\"value\":{\"object2\":true}}]";
	std::vector<bool> expected;
	std::vector<bool> result;

	expected.push_back(true);
	expected.push_back(false);
	expected.push_back(true);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBoolArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Float_Array)
{
	HLString input = "[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]";
	std::vector<float> expected;
	std::vector<float> result;

	expected.push_back(2.5f);
	expected.push_back(6.3f);
	expected.push_back(23.0f);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloatArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Float_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]}";
	std::vector<float> expected;
	std::vector<float> result;

	expected.push_back(2.5f);
	expected.push_back(6.3f);
	expected.push_back(23.0f);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloatArray("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Float_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"float\",\"value\":{\"object0\":2.5}},{\"type\":\"float\",\"value\":{\"object1\":6.3}},{\"type\":\"float\",\"value\":{\"object2\":23.0}}]";
	std::vector<float> expected;
	std::vector<float> result;

	expected.push_back(2.5f);
	expected.push_back(6.3f);
	expected.push_back(23.0f);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloatArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Double_Array)
{
	HLString input = "[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]";
	std::vector<double> expected;
	std::vector<double> result;

	expected.push_back(2.5);
	expected.push_back(6.3);
	expected.push_back(23.0);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDoubleArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Double_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]}";
	std::vector<double> expected;
	std::vector<double> result;

	expected.push_back(2.5);
	expected.push_back(6.3);
	expected.push_back(23.0);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDoubleArray("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Double_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"double\",\"value\":{\"object0\":2.5}},{\"type\":\"double\",\"value\":{\"object1\":6.3}},{\"type\":\"double\",\"value\":{\"object2\":23.0}}]";
	std::vector<double> expected;
	std::vector<double> result;

	expected.push_back(2.5);
	expected.push_back(6.3);
	expected.push_back(23.0);

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDoubleArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Vec2_Array)
{
	HLString input = "[{\"object0\":[2.5,2.0]},{\"object1\":[6.3,4.5]},{\"object2\":[23.0,9.5]}]";
	std::vector<glm::vec2> expected;
	std::vector<glm::vec2> result;

	expected.push_back({ 2.5f, 2.0f });
	expected.push_back({ 6.3f, 4.5f });
	expected.push_back({ 23.0f, 9.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec2_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":[2.5,2.0]},{\"object1\":[6.3,4.5]},{\"object2\":[23.0,9.5]}]}";
	std::vector<glm::vec2> expected;
	std::vector<glm::vec2> result;

	expected.push_back({ 2.5f, 2.0f });
	expected.push_back({ 6.3f, 4.5f });
	expected.push_back({ 23.0f, 9.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec2_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"vec2\",\"value\":{\"object0\":[2.5,2.0]}},{\"type\":\"vec2\",\"value\":{\"object1\":[6.3,4.5]}},{\"type\":\"vec2\",\"value\":{\"object2\":[23.0,9.5]}}]";
	std::vector<glm::vec2> expected;
	std::vector<glm::vec2> result;

	expected.push_back({ 2.5f, 2.0f });
	expected.push_back({ 6.3f, 4.5f });
	expected.push_back({ 23.0f, 9.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Vec3_Array)
{
	HLString input = "[{\"object0\":[2.5,2.0,2.9]},{\"object1\":[6.3,4.5,2.0]},{\"object2\":[23.0,9.0,5.0]}]";
	std::vector<glm::vec3> expected;
	std::vector<glm::vec3> result;

	expected.push_back({ 2.5f, 2.0f, 2.9f });
	expected.push_back({ 6.3f, 4.5f, 2.0f });
	expected.push_back({ 23.0f, 9.0f, 5.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec3Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec3_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":[2.5,2.0,2.9]},{\"object1\":[6.3,4.5,2.0]},{\"object2\":[23.0,9.0,5.0]}]}";
	std::vector<glm::vec3> expected;
	std::vector<glm::vec3> result;

	expected.push_back({ 2.5f, 2.0f, 2.9f });
	expected.push_back({ 6.3f, 4.5f, 2.0f });
	expected.push_back({ 23.0f, 9.0f, 5.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec3Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec3_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"vec3\",\"value\":{\"object0\":[2.5,2.0,2.9]}},{\"type\":\"vec3\",\"value\":{\"object1\":[6.3,4.5,2.0]}},{\"type\":\"vec3\",\"value\":{\"object2\":[23.0,9.0,5.0]}}]";
	std::vector<glm::vec3> expected;
	std::vector<glm::vec3> result;

	expected.push_back({ 2.5f, 2.0f, 2.9f });
	expected.push_back({ 6.3f, 4.5f, 2.0f });
	expected.push_back({ 23.0f, 9.0f, 5.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec3Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Vec4_Array)
{
	HLString input = "[{\"object0\":[2.5,2.0,2.9,6.0]},{\"object1\":[6.3,4.5,2.0,7.5]},{\"object2\":[23.0,9.5,5.5,1.5]}]";
	std::vector<glm::vec4> expected;
	std::vector<glm::vec4> result;

	expected.push_back({ 2.5f, 2.0f, 2.9f, 6.0f });
	expected.push_back({ 6.3f, 4.5f, 2.0f, 7.5f });
	expected.push_back({ 23.0f, 9.5f, 5.5f, 1.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec4_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":[2.5,2.0,2.9,6.0]},{\"object1\":[6.3,4.5,2.0,7.5]},{\"object2\":[23.0,9.5,5.5,1.5]}]}";
	std::vector<glm::vec4> expected;
	std::vector<glm::vec4> result;

	expected.push_back({ 2.5f, 2.0f, 2.9f, 6.0f });
	expected.push_back({ 6.3f, 4.5f, 2.0f, 7.5f });
	expected.push_back({ 23.0f, 9.5f, 5.5f, 1.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Vec4_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"vec4\",\"value\":{\"object0\":[2.5,2.0,2.9,6.0]}},{\"type\":\"vec4\",\"value\":{\"object1\":[6.3,4.5,2.0,7.5]}},{\"type\":\"vec4\",\"value\":{\"object2\":[23.0,9.5,5.5,1.5]}}]";
	std::vector<glm::vec4> expected;
	std::vector<glm::vec4> result;

	expected.push_back({ 2.5f, 2.0f, 2.9f, 6.0f });
	expected.push_back({ 6.3f, 4.5f, 2.0f, 7.5f });
	expected.push_back({ 23.0f, 9.5f, 5.5f, 1.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Mat2_Array)
{
	HLString input = "[{\"object0\":[0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]";
	std::vector<glm::mat2> expected;
	std::vector<glm::mat2> result;

	expected.push_back(glm::mat2(0.0f));
	expected.push_back(glm::mat2(1.0f));
	expected.push_back(glm::mat2(23.0f, 9.0f, 5.5f, 1.5f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat2_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":[0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]}";
	std::vector<glm::mat2> expected;
	std::vector<glm::mat2> result;

	expected.push_back(glm::mat2(0.0f));
	expected.push_back(glm::mat2(1.0f));
	expected.push_back(glm::mat2(23.0f, 9.0f, 5.5f, 1.5f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat2_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"mat2\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0]}},{\"type\":\"mat2\",\"value\":{\"object1\":[1.0,0.0,0.0,1.0]}},{\"type\":\"mat2\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5]}}]";
	std::vector<glm::mat2> expected;
	std::vector<glm::mat2> result;

	expected.push_back(glm::mat2(0.0f));
	expected.push_back(glm::mat2(1.0f));
	expected.push_back(glm::mat2(23.0f, 9.0f, 5.5f, 1.5f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Mat3_Array)
{
	HLString input = "[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}]";
	std::vector<glm::mat3> expected;
	std::vector<glm::mat3> result;

	expected.push_back(glm::mat3(0.0f));
	expected.push_back(glm::mat3(1.0f));
	expected.push_back(glm::mat3(23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat3_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}]}";
	std::vector<glm::mat3> expected;
	std::vector<glm::mat3> result;

	expected.push_back(glm::mat3(0.0f));
	expected.push_back(glm::mat3(1.0f));
	expected.push_back(glm::mat3(23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat3_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"mat3\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]}},{\"type\":\"mat3\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]}},{\"type\":\"mat3\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}}]";
	std::vector<glm::mat3> expected;
	std::vector<glm::mat3> result;

	expected.push_back(glm::mat3(0.0f));
	expected.push_back(glm::mat3(1.0f));
	expected.push_back(glm::mat3(23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Mat4_Array)
{
	HLString input = "[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}]";
	std::vector<glm::mat4> expected;
	std::vector<glm::mat4> result;

	expected.push_back(glm::mat4(0.0f));
	expected.push_back(glm::mat4(1.0f));
	expected.push_back(glm::mat4(23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat4_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}]}";
	std::vector<glm::mat4> expected;
	std::vector<glm::mat4> result;

	expected.push_back(glm::mat4(0.0f));
	expected.push_back(glm::mat4(1.0f));
	expected.push_back(glm::mat4(23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Mat4_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"mat4\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]}},{\"type\":\"mat4\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]}},{\"type\":\"mat4\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}}]";
	std::vector<glm::mat4> expected;
	std::vector<glm::mat4> result;

	expected.push_back(glm::mat4(0.0f));
	expected.push_back(glm::mat4(1.0f));
	expected.push_back(glm::mat4(23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f));

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Quat_Array)
{
	HLString input = "[{\"object0\":[0.0,1.0,1.0,1.0]},{\"object1\":[1.0,0.0,0.0,0.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]";
	std::vector<glm::quat> expected;
	std::vector<glm::quat> result;

	expected.push_back({ 0.0f, 1.0f, 1.0f, 1.0f });
	expected.push_back({ 1.0f, 0.0f, 0.0f, 0.0f });
	expected.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Quat_Array_With_Keys)
{
	HLString input = "{\"arrayKeys\":[{\"object0\":[0.0,1.0,1.0,1.0]},{\"object1\":[1.0,0.0,0.0,0.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]}";
	std::vector<glm::quat> expected;
	std::vector<glm::quat> result;

	expected.push_back({ 0.0f, 1.0f, 1.0f, 1.0f });
	expected.push_back({ 1.0f, 0.0f, 0.0f, 0.0f });
	expected.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArray("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Quat_Array_WithTypeCheck)
{
	HLString input = "[{\"type\":\"quat\",\"value\":{\"object0\":[0.0,1.0,1.0,1.0]}},{\"type\":\"quat\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0]}},{\"type\":\"quat\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5]}}]";
	std::vector<glm::quat> expected;
	std::vector<glm::quat> result;

	expected.push_back({ 0.0f, 1.0f, 1.0f, 1.0f });
	expected.push_back({ 1.0f, 0.0f, 0.0f, 0.0f });
	expected.push_back({ 23.0f, 9.0f, 5.5f, 1.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}


///
/// Direct Array Tests
///

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Int32)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":-10}]";
	std::map<HLString, int32> expected;
	std::map<HLString, int32> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", -10 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Int32_WithTypeCheck)
{
	HLString input = "[{\"type\":\"int32\",\"value\":{\"object0\":42}},{\"type\":\"int32\",\"value\":{\"object1\":10}},{\"type\":\"int32\",\"value\":{\"object2\":-10}}]";
	std::map<HLString, int32> expected;
	std::map<HLString, int32> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", -10 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_UInt32)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
	std::map<HLString, uint32> expected;
	std::map<HLString, uint32> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_UInt32_WithTypeCheck)
{
	HLString input = "[{\"type\":\"uint32\",\"value\":{\"object0\":42}},{\"type\":\"uint32\",\"value\":{\"object1\":10}},{\"type\":\"uint32\",\"value\":{\"object2\":12}}]";
	std::map<HLString, uint32> expected;
	std::map<HLString, uint32> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Int64)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
	std::map<HLString, int64> expected;
	std::map<HLString, int64> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Int64_WithTypeCheck)
{
	HLString input = "[{\"type\":\"int64\",\"value\":{\"object0\":42}},{\"type\":\"int64\",\"value\":{\"object1\":10}},{\"type\":\"int64\",\"value\":{\"object2\":12}}]";
	std::map<HLString, int64> expected;
	std::map<HLString, int64> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_UInt64)
{
	HLString input = "[{\"object0\":42},{\"object1\":10},{\"object2\":12}]";
	std::map<HLString, uint64> expected;
	std::map<HLString, uint64> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_UInt64_WithTypeCheck)
{
	HLString input = "[{\"type\":\"uint64\",\"value\":{\"object0\":42}},{\"type\":\"uint64\",\"value\":{\"object1\":10}},{\"type\":\"uint64\",\"value\":{\"object2\":12}}]";
	std::map<HLString, uint64> expected;
	std::map<HLString, uint64> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Float)
{
	HLString input = "[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]";
	std::map<HLString, float> expected;
	std::map<HLString, float> result;

	expected.insert({ "object0", 2.5f });
	expected.insert({ "object1", 6.3f });
	expected.insert({ "object2", 23.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloatArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Float_WithTypeCheck)
{
	HLString input = "[{\"type\":\"float\",\"value\":{\"object0\":2.5}},{\"type\":\"float\",\"value\":{\"object1\":6.3}},{\"type\":\"float\",\"value\":{\"object2\":23.0}}]";
	std::map<HLString, float> expected;
	std::map<HLString, float> result;

	expected.insert({ "object0", 2.5f });
	expected.insert({ "object1", 6.3f });
	expected.insert({ "object2", 23.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloatArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Double)
{
	HLString input = "[{\"object0\":2.5},{\"object1\":6.3},{\"object2\":23.0}]";
	std::map<HLString, double> expected;
	std::map<HLString, double> result;

	expected.insert({ "object0", 2.5 });
	expected.insert({ "object1", 6.3 });
	expected.insert({ "object2", 23.0 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDoubleArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Double_WithTypeCheck)
{
	HLString input = "[{\"type\":\"double\",\"value\":{\"object0\":2.5}},{\"type\":\"double\",\"value\":{\"object1\":6.3}},{\"type\":\"double\",\"value\":{\"object2\":23.0}}]";
	std::map<HLString, double> expected;
	std::map<HLString, double> result;

	expected.insert({ "object0", 2.5 });
	expected.insert({ "object1", 6.3 });
	expected.insert({ "object2", 23.0 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDoubleArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Bool)
{
	HLString input = "[{\"object0\":true},{\"object1\":false},{\"object2\":true}]";
	std::map<HLString, bool> expected;
	std::map<HLString, bool> result;

	expected.insert({ "object0", true });
	expected.insert({ "object1", false });
	expected.insert({ "object2", true });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBoolArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Bool_WithTypeCheck)
{
	HLString input = "[{\"type\":\"bool\",\"value\":{\"object0\":true}},{\"type\":\"bool\",\"value\":{\"object1\":false}},{\"type\":\"bool\",\"value\":{\"object2\":true}}]";
	std::map<HLString, bool> expected;
	std::map<HLString, bool> result;

	expected.insert({ "object0", true });
	expected.insert({ "object1", false });
	expected.insert({ "object2", true });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBoolArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_String)
{
	HLString input = "[{\"object0\":\"Hello\"},{\"object1\":\"world\"},{\"object2\":\"!\"}]";
	std::map<HLString, HLString> expected;
	std::map<HLString, HLString> result;

	expected.insert({ "object0", "Hello" });
	expected.insert({ "object1", "world" });
	expected.insert({ "object2", "!" });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadStringArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_String_WithTypeCheck)
{
	HLString input = "[{\"type\":\"string\",\"value\":{\"object0\":\"Hello\"}},{\"type\":\"string\",\"value\":{\"object1\":\"world\"}},{\"type\":\"string\",\"value\":{\"object2\":\"!\"}}]";
	std::map<HLString, HLString> expected;
	std::map<HLString, HLString> result;

	expected.insert({ "object0", "Hello" });
	expected.insert({ "object1", "world" });
	expected.insert({ "object2", "!" });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadStringArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Vec2)
{
	HLString input = "[{\"object0\":[2.5,2.0]},{\"object1\":[6.3,4.5]},{\"object2\":[23.0,9.5]}]";
	std::map<HLString, glm::vec2> expected;
	std::map<HLString, glm::vec2> result;

	expected.insert({ "object0", { 2.5f, 2.0f } });
	expected.insert({ "object1", { 6.3f, 4.5f } });
	expected.insert({ "object2", { 23.0f, 9.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Vec2_WithTypeCheck)
{
	HLString input = "[{\"type\":\"vec2\",\"value\":{\"object0\":[2.5,2.0]}},{\"type\":\"vec2\",\"value\":{\"object1\":[6.3,4.5]}},{\"type\":\"vec2\",\"value\":{\"object2\":[23.0,9.5]}}]";
	std::map<HLString, glm::vec2> expected;
	std::map<HLString, glm::vec2> result;

	expected.insert({ "object0", { 2.5f, 2.0f } });
	expected.insert({ "object1", { 6.3f, 4.5f } });
	expected.insert({ "object2", { 23.0f, 9.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Vec3)
{
	HLString input = "[{\"object0\":[2.5,2.0,2.9]},{\"object1\":[6.3,4.5,2.0]},{\"object2\":[23.0,9.0,5.0]}]";
	std::map<HLString, glm::vec3> expected;
	std::map<HLString, glm::vec3> result;

	expected.insert({ "object0", { 2.5f, 2.0f, 2.9f } });
	expected.insert({ "object1", { 6.3f, 4.5f, 2.0f } });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.0f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec3ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Vec3_WithTypeCheck)
{
	HLString input = "[{\"type\":\"vec3\",\"value\":{\"object0\":[2.5,2.0,2.9]}},{\"type\":\"vec3\",\"value\":{\"object1\":[6.3,4.5,2.0]}},{\"type\":\"vec3\",\"value\":{\"object2\":[23.0,9.0,5.0]}}]";
	std::map<HLString, glm::vec3> expected;
	std::map<HLString, glm::vec3> result;

	expected.insert({ "object0", { 2.5f, 2.0f, 2.9f } });
	expected.insert({ "object1", { 6.3f, 4.5f, 2.0f } });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.0f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec3ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Vec4)
{
	HLString input = "[{\"object0\":[2.5,2.0,2.9,6.0]},{\"object1\":[6.3,4.5,2.0,7.5]},{\"object2\":[23.0,9.5,5.5,1.5]}]";
	std::map<HLString, glm::vec4> expected;
	std::map<HLString, glm::vec4> result;

	expected.insert({ "object0", { 2.5f, 2.0f, 2.9f, 6.0f } });
	expected.insert({ "object1", { 6.3f, 4.5f, 2.0f, 7.5f } });
	expected.insert({ "object2", { 23.0f, 9.5f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Vec4_WithTypeCheck)
{
	HLString input = "[{\"type\":\"vec4\",\"value\":{\"object0\":[2.5,2.0,2.9,6.0]}},{\"type\":\"vec4\",\"value\":{\"object1\":[6.3,4.5,2.0,7.5]}},{\"type\":\"vec4\",\"value\":{\"object2\":[23.0,9.5,5.5,1.5]}}]";
	std::map<HLString, glm::vec4> expected;
	std::map<HLString, glm::vec4> result;

	expected.insert({ "object0", { 2.5f, 2.0f, 2.9f, 6.0f } });
	expected.insert({ "object1", { 6.3f, 4.5f, 2.0f, 7.5f } });
	expected.insert({ "object2", { 23.0f, 9.5f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Mat2)
{
	HLString input = "[{\"object0\":[0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]";
	std::map<HLString, glm::mat2> expected;
	std::map<HLString, glm::mat2> result;

	expected.insert({ "object0", glm::mat2(0.0f) });
	expected.insert({ "object1", glm::mat2(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Mat2_WithTypeCheck)
{
	HLString input = "[{\"type\":\"mat2\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0]}},{\"type\":\"mat2\",\"value\":{\"object1\":[1.0,0.0,0.0,1.0]}},{\"type\":\"mat2\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5]}}]";
	std::map<HLString, glm::mat2> expected;
	std::map<HLString, glm::mat2> result;

	expected.insert({ "object0", glm::mat2(0.0f) });
	expected.insert({ "object1", glm::mat2(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Mat3)
{
	HLString input = "[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}]";
	std::map<HLString, glm::mat3> expected;
	std::map<HLString, glm::mat3> result;

	expected.insert({ "object0", glm::mat3(0.0f) });
	expected.insert({ "object1", glm::mat3(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Mat3_WithTypeCheck)
{
	HLString input = "[{\"type\":\"mat3\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]}},{\"type\":\"mat3\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]}},{\"type\":\"mat3\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5]}}]";
	std::map<HLString, glm::mat3> expected;
	std::map<HLString, glm::mat3> result;

	expected.insert({ "object0", glm::mat3(0.0f) });
	expected.insert({ "object1", glm::mat3(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Mat4)
{
	HLString input = "[{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]},{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]},{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}]";
	std::map<HLString, glm::mat4> expected;
	std::map<HLString, glm::mat4> result;

	expected.insert({ "object0", glm::mat4(0.0f) });
	expected.insert({ "object1", glm::mat4(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Mat4_WithTypeCheck)
{
	HLString input = "[{\"type\":\"mat4\",\"value\":{\"object0\":[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]}},{\"type\":\"mat4\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]}},{\"type\":\"mat4\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5,6.0,1.0,7.5,0.0,4.5,10.5,1.1,5.32,3.2,1.5,5.3,4.8]}}]";
	std::map<HLString, glm::mat4> expected;
	std::map<HLString, glm::mat4> result;

	expected.insert({ "object0", glm::mat4(0.0f) });
	expected.insert({ "object1", glm::mat4(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4ArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}


TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Quat)
{
	HLString input = "[{\"object0\":[0.0,1.0,1.0,1.0]},{\"object1\":[1.0,0.0,0.0,0.0]},{\"object2\":[23.0,9.0,5.5,1.5]}]";
	std::map<HLString, glm::quat> expected;
	std::map<HLString, glm::quat> result;

	expected.insert({ "object0", { 0.0f, 1.0f, 1.0f, 1.0f } });
	expected.insert({ "object1", { 1.0f, 0.0f, 0.0f, 0.0f } });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(JsonReadParserTests, JSONParser_Read_Complete_Array_Quat_WithTypeCheck)
{
	HLString input = "[{\"type\":\"quat\",\"value\":{\"object0\":[0.0,1.0,1.0,1.0]}},{\"type\":\"quat\",\"value\":{\"object1\":[1.0,0.0,0.0,0.0]}},{\"type\":\"quat\",\"value\":{\"object2\":[23.0,9.0,5.5,1.5]}}]";
	std::map<HLString, glm::quat> expected;
	std::map<HLString, glm::quat> result;

	expected.insert({ "object0", { 0.0f, 1.0f, 1.0f, 1.0f } });
	expected.insert({ "object1", { 1.0f, 0.0f, 0.0f, 0.0f } });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArrayMap("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

#endif

