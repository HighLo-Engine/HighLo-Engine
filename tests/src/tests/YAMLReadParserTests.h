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

struct YAMLReadParserTests : public testing::Test
{
	Ref<DocumentWriter> Reader;

	YAMLReadParserTests()
	{
		Reader = DocumentWriter::Create("", DocumentType::Yaml);
		Logger::Init();
	}

	virtual ~YAMLReadParserTests()
	{
		Logger::Shutdown();
	}
};

TEST_F(YAMLReadParserTests, YAMLParser_Read_String)
{
	HLString input = "";
	HLString expected = "Hello World!";
	HLString result = "";

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadString("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Int32)
{
	HLString input = "";
	int32 expected = -10;
	int32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_UInt32)
{
	HLString input = "";
	uint32 expected = 42;
	uint32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Int64)
{
	HLString input = "";
	int64 expected = 42;
	int64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_UInt64)
{
	HLString input = "";
	uint64 expected = 42;
	uint64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Bool)
{
	HLString input = "";
	bool expected = true;
	bool result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBool("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Float)
{
	HLString input = "";
	float expected = 20.2f;
	float result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloat("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Double)
{
	HLString input = "";
	double expected = 20.2;
	double result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDouble("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Vec2)
{
	HLString input = "";
	glm::vec2 expected = { 20.2f, 10.5f };
	glm::vec2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector2("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Vec3)
{
	HLString input = "";
	glm::vec3 expected = { 20.2f, 10.5f, 23.5f };
	glm::vec3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector3("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Vec4)
{
	HLString input = "";
	glm::vec4 expected = { 20.2f, 10.5f, 23.5f, 11.5f };
	glm::vec4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector4("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Mat2)
{
	HLString input = "";
	glm::mat2 expected = glm::mat2(20.2f, 10.5f, 23.5f, 11.5f);
	glm::mat2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix2("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Mat3)
{
	HLString input = "";
	glm::mat3 expected = glm::mat3(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f);
	glm::mat3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix3("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Mat4)
{
	HLString input = "";
	glm::mat4 expected = glm::mat4(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f, 47.2f, 55.4f, 48.4f, 32.0f, 15.1f, 499.1f, 22.5f);
	glm::mat4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix4("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_quat)
{
	HLString input = "";
	glm::quat expected = glm::quat(20.2f, 10.5f, 23.1f, 11.5f);
	glm::quat result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuaternion("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

///
/// ArrayMap
/// 

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Int32)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_UInt32)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Int64)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_UInt64)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Float)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Double)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Bool)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_String)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Vec2)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Vec3)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Vec4)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Mat2)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Mat3)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Mat4)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Complete_Array_Quat)
{
	HLString input = "";
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

///
/// Array
/// 

TEST_F(YAMLReadParserTests, YAMLParser_Read_String_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Int32_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_UInt32_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Int64_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_UInt64_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Bool_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Float_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Double_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Vec2_Array)
{
	HLString input = "";
	std::vector<glm::vec2> expected;
	std::vector<glm::vec2> result;

	expected.push_back({ 1.5f, 6.0f });
	expected.push_back({ 2.5f, 1.0f });
	expected.push_back({ 1.0f, 0.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Vec3_Array)
{
	HLString input = "";
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

TEST_F(YAMLReadParserTests, YAMLParser_Read_Vec4_Array)
{
	HLString input = "";
	std::vector<glm::vec4> expected;
	std::vector<glm::vec4> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	expected.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	expected.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Mat2_Array)
{
	HLString input = "";
	std::vector<glm::mat2> expected;
	std::vector<glm::mat2> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	expected.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	expected.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Mat3_Array)
{
	HLString input = "";
	std::vector<glm::mat3> expected;
	std::vector<glm::mat3> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f, 142.0f, 155.0f, 231.0f, 1.753f, 3571.05f });
	expected.push_back({ 2.5f, 1.0f, 5.8f, 2.9f, 15.0f, 152.0f, 7451.0f, 175.0f, 1573.03f });
	expected.push_back({ 1.0f, 0.0f, 5.5f, 2.5f, 1.20f, 165.0f, 861.0f, 341.0f, 1375.750f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Mat4_Array)
{
	HLString input = "";
	std::vector<glm::mat4> expected;
	std::vector<glm::mat4> result;

	expected.push_back(glm::mat4(0.0f));
	expected.push_back(glm::mat4(1.0f));
	expected.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4Array("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(YAMLReadParserTests, YAMLParser_Read_Quat_Array)
{
	HLString input = "";
	std::vector<glm::quat> expected;
	std::vector<glm::quat> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	expected.push_back({ 164.5f, 326.5f, 235.3f, 32.0f });
	expected.push_back({ 14.5f, 32.5f, 23.53f, 3.20f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArray("", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

