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

TEST_F(JsonWriteParserTests, write_int32)
{
	int32 test = 42;
	Writer->WriteInt32("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_uint32)
{
	uint32 test = 42;
	Writer->WriteUInt32("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_int64)
{
	int64 test = 42;
	Writer->WriteInt64("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_uint64)
{
	uint64 test = 42;
	Writer->WriteUInt64("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_bool)
{
	bool test = false;
	Writer->WriteBool("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_float)
{
	float test = 5.3f;
	Writer->WriteFloat("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_double)
{
	double test = 4.2f;
	Writer->WriteDouble("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_vec2)
{
	glm::vec2 test = { 3.4f, 2.5f };
	Writer->WriteVec2("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_vec3)
{
	glm::vec3 test = { 3.4f, 2.5f, 8.8f };
	Writer->WriteVec3("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_vec4)
{
	glm::vec4 test = { 3.4f, 2.5f, 8.8f, 9.3f };
	Writer->WriteVec4("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_mat2)
{
	glm::mat2 test = { 3.4f, 2.5f, 8.8f, 9.3f };
	Writer->WriteMat2("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_mat3)
{
	glm::mat3 test = { 3.4f, 2.5f, 8.8f, 9.3f, 1.4f, 2.14f, 5.3f, 2.1f, 5.3f };
	Writer->WriteMat3("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_mat4)
{
	glm::mat4 test = { 3.4f, 2.5f, 8.8f, 9.3f, 1.4f, 2.14f, 5.3f, 2.1f, 5.3f, 2.1f, 5.3f, 6.3f, 646.3f, 2.3f, 2.4f, 2.4f };
	Writer->WriteMat4("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

TEST_F(JsonWriteParserTests, write_quat)
{
	glm::quat test = { 3.4f, 2.5f, 8.8f, 9.3f };
	Writer->WriteQuaternion("test", test);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;
}

