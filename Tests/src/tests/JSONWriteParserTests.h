// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-08) initial release
//

/**
 * =========================================================================================================
 *													API usage
 * =========================================================================================================
 *
 * Example #1:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->WriteString("Hello", "World");
 * writer->WriteUint64("number", 64);
 * writer->WriteOut();
 *
 * -> should result in json:
 * {
 *    "Hello":"World",
 *    "number":64
 * }
 *
 * Example #2:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->BeginObject();
 * writer->WriteString("Hello", "World");
 * writer->WriteUInt64("number", 64);
 * writer->EndObject();
 * writer->WriterOut();
 *
 * -> should result in json:
 * {
 *     "Hello":
 *     {
 *       "value":"World",
 *       "type":"string"
 *     },
 *     "number":
 *     {
 *       "value":64,
 *       "type":"uint64"
 *     }
 * }
 *
 * Example #3:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->BeginArray();
 * writer->WriteString("Hello", "World");
 * writer->WriteUInt64("number", 64);
 * writer->EndArray();
 * writer->WriteOut();
 *
 * -> should result in json:
 * [
 *   "World",
 *   64
 * ]
 *
 * Example #4:
 * -----------
 *
 * Ref<DocumentWriter> writer = DocumentWriter::Create("<filepath>");
 * writer->BeginArray();
 * for ()
 *   writer->BeginObject();
 *   writer->WriteString("Hello", "World");
 *   writer->WriteUInt64("number", 64);
 *   writer->EndObject();
 * endfor
 * writer->EndArray();
 * writer->WriteOut();
 *
 * -> should result in json:
 * [
 *  {
 *     "Hello":"World",
 *     "type":"string"
 *   },
 *   {
 *     "number":64,
 *     "type":"uint64"
 *   }
 * ]
 */

#pragma once

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"
#include "utils/TestData.h"

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

#pragma region BasicWriting

TEST_F(JsonWriteParserTests, write_int32)
{
	int32 test = 42;
	char *testResult = "{\"test\":42}";

	Writer->WriteInt32("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_uint32)
{
	uint32 test = 42;
	char *testResult = "{\"test\":42}";

	Writer->WriteUInt32("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_int64)
{
	int64 test = 42;
	char *testResult = "{\"test\":42}";

	Writer->WriteInt64("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_uint64)
{
	uint64 test = 42;
	char *testResult = "{\"test\":42}";

	Writer->WriteUInt64("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_bool)
{
	bool test = false;
	char *testResult = "{\"test\":false}";

	Writer->WriteBool("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_float)
{
	float test = 5.3f;
	char *testResult = "{\"test\":5.3}";
	
	Writer->WriteFloat("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_double)
{
	double test = 4.2;
	char *testResult = "{\"test\":4.2}";

	Writer->WriteDouble("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_vec2)
{
	glm::vec2 test = testing::utils::GetVector2();
	char *testResult = "{\"test\":[2.5,6.8]}";

	Writer->WriteVec2("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_vec3)
{
	glm::vec3 test = testing::utils::GetVector3();
	char *testResult = "{\"test\":[2.5,6.8,10.5]}";

	Writer->WriteVec3("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_vec4)
{
	glm::vec4 test = ::testing::utils::GetVector4();
	char *testResult = "{\"test\":[2.5,6.8,10.5,12.5]}";

	Writer->WriteVec4("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_mat2)
{
	glm::mat2 test = testing::utils::GetMatrix2();
	char *testResult = "{\"test\":[2.5,6.8,10.5,12.5]}";

	Writer->WriteMat2("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_mat3)
{
	glm::mat3 test = testing::utils::GetMatrix3();
	char *testResult = "{\"test\":[2.5,6.8,10.5,12.5,1.5,6.4,32.5,5.3,6.4]}";

	Writer->WriteMat3("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_mat4)
{
	glm::mat4 test = testing::utils::GetMatrix4();
	char *testResult = "{\"test\":[2.5,6.8,10.5,12.5,1.5,6.4,32.5,5.3,6.4,4.5,63.0,74.4,42.4,42.5,6.3,74.4]}";

	Writer->WriteMat4("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

TEST_F(JsonWriteParserTests, write_quat)
{
	glm::quat test = testing::utils::GetQuaternion();
	char *testResult = "{\"test\":[2.5,6.8,10.5,12.5]}";

	Writer->WriteQuaternion("test", test);

	HLString content = Writer->GetContent();
	EXPECT_STRING_EQUALS(content, testResult);
}

#pragma endregion

#pragma region ArrayWriting

TEST_F(JsonWriteParserTests, write_int32_array)
{
	std::vector<int32> test = { 10, 42, 32, 53, 9354 };

	Writer->WriteInt32Array("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

#pragma endregion

#pragma region MapWriting

TEST_F(JsonWriteParserTests, write_int32_map)
{
	std::map<HLString, int32> test = {
		{ "Hello", 10 },
		{ "World", 42 },
		{ "!", 32 },
	};

	Writer->WriteInt32ArrayMap("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

#pragma endregion

#pragma region UnorderedMapWriting

TEST_F(JsonWriteParserTests, write_int32_unordered_map)
{
	std::unordered_map<HLString, int32> test = {
		{ "Hello", 10 },
		{ "World", 42 },
		{ "!", 32 },
	};

	Writer->WriteInt32ArrayMap("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

#pragma endregion

