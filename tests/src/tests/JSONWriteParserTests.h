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

/// =========================================================================================================
/// ==============================================  Int32 Tests  ============================================
/// =========================================================================================================
TEST_F(JsonWriteParserTests, write_int32)
{
	HLString expected = "{\"value\":42}";
	int32 value = 42;

	Writer->WriteInt32("value", value);
	HLString result = Writer->GetContent();

	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_multiple)
{
	HLString expected = "{\"value\":42,\"anotherValue\":10}";
	int32 value = 42;
	int32 anotherValue = 10;

	Writer->WriteInt32("value", value);
	Writer->WriteInt32("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_typesafe)
{
	int32 value = 42;
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"int32\"}}";

	Writer->BeginObject();
	Writer->WriteInt32("value", value);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_typesafe_multiple)
{
	int32 value = 42;
	int32 anotherValue = 10;
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"int32\"},\"anotherValue\":{\"value\":10,\"type\":\"int32\"}}";

	Writer->BeginObject();
	Writer->WriteInt32("value", value);
	Writer->WriteInt32("anotherValue", anotherValue);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_array)
{
	HLString expected = "{\"value\":[42,10]}";
	std::vector<int32> value;
	value.push_back(42);
	value.push_back(10);

	Writer->WriteInt32Array("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_array_multiple)
{
	HLString expected = "{\"value\":[42,10],\"anotherValue\":[42,10]}";
	std::vector<int32> value;
	value.push_back(42);
	value.push_back(10);

	std::vector<int32> anotherValue;
	anotherValue.push_back(42);
	anotherValue.push_back(10);

	Writer->WriteInt32Array("value", value);
	Writer->WriteInt32Array("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_arraymap)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"int32\"},{\"Entry2\":10,\"type\":\"int32\"}]}";
	std::map<HLString, int32> value;
	value.insert({"Entry1", 42});
	value.insert({"Entry2", 10});

	Writer->WriteInt32ArrayMap("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_arraymap_multiple)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"int32\"},{\"Entry2\":10,\"type\":\"int32\"}],\"anotherValue\":[{\"Entry3\":42,\"type\":\"int32\"},{\"Entry4\":10,\"type\":\"int32\"}]}";
	std::map<HLString, int32> value;
	value.insert({ "Entry1", 42 });
	value.insert({ "Entry2", 10 });

	std::map<HLString, int32> anotherValue;
	anotherValue.insert({ "Entry3", 42 });
	anotherValue.insert({ "Entry4", 10 });

	Writer->WriteInt32ArrayMap("value", value);
	Writer->WriteInt32ArrayMap("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_array_direct)
{
	int32 value = 42;
	HLString expected = "[42]";

	Writer->BeginArray();
	Writer->WriteInt32("value", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_array_direct_multiple)
{
	int32 value = 42;
	int32 anotherValue = 10;
	HLString expected = "[42,10]";

	Writer->BeginArray();
	Writer->WriteInt32("value", value);
	Writer->WriteInt32("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_direct_arraymap)
{
	int32 value = 42;
	HLString expected = "[{\"value\":42,\"type\":\"int32\"}]";

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteInt32("value", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int32_arraymap_direct_multiple)
{
	int32 value = 42;
	int32 anotherValue = 10;
	HLString expected = "[[{\"value\":42,\"type\":\"int32\"},{\"anotherValue\":10,\"type\":\"int32\"}]]";

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteInt32("value", value);
	Writer->WriteInt32("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  UInt32 Tests  ===========================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_uint32)
{
	HLString expected = "{\"value\":42}";
	uint32 value = 42;

	Writer->WriteUInt32("value", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_multiple)
{
	HLString expected = "{\"value\":42,\"anotherValue\":10}";
	uint32 value = 42;
	uint32 anotherValue = 10;

	Writer->WriteUInt32("value", value);
	Writer->WriteUInt32("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_typesafe)
{
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"uint32\"}}";
	uint32 value = 42;

	Writer->BeginObject();
	Writer->WriteUInt32("value", value);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_typesafe_multiple)
{
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"uint32\"},\"anotherValue\":{\"value\":10,\"type\":\"uint32\"}}";
	uint32 value = 42;
	uint32 anotherValue = 10;

	Writer->BeginObject();
	Writer->WriteUInt32("value", value);
	Writer->WriteUInt32("anotherValue", anotherValue);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_array)
{
	HLString expected = "{\"value\":[42,10]}";
	std::vector<uint32> value;
	value.push_back(42);
	value.push_back(10);

	Writer->WriteUInt32Array("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_array_multiple)
{
	HLString expected = "{\"value\":[42,10],\"anotherValue\":[42,10]}";
	std::vector<uint32> value;
	value.push_back(42);
	value.push_back(10);

	std::vector<uint32> anotherValue;
	anotherValue.push_back(42);
	anotherValue.push_back(10);

	Writer->WriteUInt32Array("value", value);
	Writer->WriteUInt32Array("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_arraymap)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"uint32\"},{\"Entry2\":10,\"type\":\"uint32\"}]}";
	std::map<HLString, uint32> value;
	value.insert({"Entry1", 42});
	value.insert({"Entry2", 10});

	Writer->WriteUInt32ArrayMap("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_arraymap_multiple)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"uint32\"},{\"Entry2\":10,\"type\":\"uint32\"}],\"anotherValue\":[{\"Entry3\":42,\"type\":\"uint32\"},{\"Entry4\":10,\"type\":\"uint32\"}]}";
	std::map<HLString, uint32> value;
	value.insert({ "Entry1", 42 });
	value.insert({ "Entry2", 10 });

	std::map<HLString, uint32> anotherValue;
	anotherValue.insert({ "Entry3", 42 });
	anotherValue.insert({ "Entry4", 10 });

	Writer->WriteUInt32ArrayMap("value", value);
	Writer->WriteUInt32ArrayMap("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_array_direct)
{
	HLString expected = "[42]";
	uint32 value = 42;

	Writer->BeginArray();
	Writer->WriteUInt32("value", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_array_direct_multiple)
{
	HLString expected = "[42,10]";
	uint32 value = 42;
	uint32 anotherValue = 10;

	Writer->BeginArray();
	Writer->WriteUInt32("value", value);
	Writer->WriteUInt32("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_arraymap_direct)
{
	HLString expected = "[{\"value\":42,\"type\":\"uint32\"}]";
	uint32 value = 42;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteUInt32("value", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint32_arraymap_direct_multiple)
{
	HLString expected = "[[{\"value\":42,\"type\":\"uint32\"},{\"anotherValue\":10,\"type\":\"uint32\"}]]";
	uint32 value = 42;
	uint32 anotherValue = 10;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteUInt32("value", value);
	Writer->WriteUInt32("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Int64 Tests  ============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_int64)
{
	HLString expected = "{\"value\":42}";
	int64 value = 42;

	Writer->WriteInt64("value", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_multiple)
{
	HLString expected = "{\"value\":42,\"anotherValue\":10}";
	int64 value = 42;
	int64 anotherValue = 10;

	Writer->WriteInt64("value", value);
	Writer->WriteInt64("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_typesafe)
{
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"int64\"}}";
	int64 value = 42;
	
	Writer->BeginObject();
	Writer->WriteInt64("value", value);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_typesafe_multiple)
{
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"int64\"},\"anotherValue\":{\"value\":10,\"type\":\"int64\"}}";
	int64 value = 42;
	int64 anotherValue = 10;

	Writer->BeginObject();
	Writer->WriteInt64("value", value);
	Writer->WriteInt64("anotherValue", anotherValue);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_array)
{
	HLString expected = "{\"value\":[42,10]}";
	std::vector<int64> value;
	value.push_back(42);
	value.push_back(10);

	Writer->WriteInt64Array("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_array_multiple)
{
	HLString expected = "{\"value\":[42,10],\"anotherValue\":[42,10]}";
	std::vector<int64> value;
	value.push_back(42);
	value.push_back(10);

	std::vector<int64> anotherValue;
	anotherValue.push_back(42);
	anotherValue.push_back(10);

	Writer->WriteInt64Array("value", value);
	Writer->WriteInt64Array("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_arraymap)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"int64\"},{\"Entry2\":10,\"type\":\"int64\"}]}";
	std::map<HLString, int64> value;
	value.insert({ "Entry1", 42 });
	value.insert({ "Entry2", 10 });

	Writer->WriteInt64ArrayMap("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_arraymap_multiple)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"int64\"},{\"Entry2\":10,\"type\":\"int64\"}],\"anotherValue\":[{\"Entry3\":42,\"type\":\"int64\"},{\"Entry4\":10,\"type\":\"int64\"}]}";
	std::map<HLString, int64> value;
	value.insert({ "Entry1", 42 });
	value.insert({ "Entry2", 10 });

	std::map<HLString, int64> anotherValue;
	anotherValue.insert({ "Entry3", 42 });
	anotherValue.insert({ "Entry4", 10 });

	Writer->WriteInt64ArrayMap("value", value);
	Writer->WriteInt64ArrayMap("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_array_direct)
{
	HLString expected = "[42]";
	int64 value = 42;

	Writer->BeginArray();
	Writer->WriteInt64("value", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_array_direct_multiple)
{
	HLString expected = "[42,10]";
	int64 value = 42;
	int64 anotherValue = 10;

	Writer->BeginArray();
	Writer->WriteInt64("value", value);
	Writer->WriteInt64("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_arraymap_direct)
{
	HLString expected = "[{\"value\":42,\"type\":\"int64\"}]";
	int64 value = 42;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteInt64("value", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_int64_arraymap_direct_multiple)
{
	HLString expected = "[[{\"value\":42,\"type\":\"int64\"},{\"anotherValue\":10,\"type\":\"int64\"}]]";
	int64 value = 42;
	int64 anotherValue = 10;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteInt64("value", value);
	Writer->WriteInt64("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}


/// =========================================================================================================
/// ==============================================  UInt64 Tests  ===========================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_uint64)
{
	HLString expected = "{\"value\":42}";
	uint64 value = 42;

	Writer->WriteUInt64("value", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_multiple)
{
	HLString expected = "{\"value\":42,\"anotherValue\":10}";
	uint64 value = 42;
	uint64 anotherValue = 10;

	Writer->WriteUInt64("value", value);
	Writer->WriteUInt64("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_typesafe)
{
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"uint64\"}}";
	uint64 value = 42;

	Writer->BeginObject();
	Writer->WriteUInt64("value", value);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_typesafe_multiple)
{
	HLString expected = "{\"value\":{\"value\":42,\"type\":\"uint64\"},\"anotherValue\":{\"value\":10,\"type\":\"uint64\"}}";
	uint64 value = 42;
	uint64 anotherValue = 10;

	Writer->BeginObject();
	Writer->WriteUInt64("value", value);
	Writer->WriteUInt64("anotherValue", anotherValue);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_array)
{
	HLString expected = "{\"value\":[42,10]}";
	std::vector<uint64> value;
	value.push_back(42);
	value.push_back(10);

	Writer->WriteUInt64Array("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_array_multiple)
{
	HLString expected = "{\"value\":[42,10],\"anotherValue\":[42,10]}";
	std::vector<uint64> value;
	value.push_back(42);
	value.push_back(10);

	std::vector<uint64> anotherValue;
	anotherValue.push_back(42);
	anotherValue.push_back(10);

	Writer->WriteUInt64Array("value", value);
	Writer->WriteUInt64Array("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_arraymap)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"uint64\"},{\"Entry2\":10,\"type\":\"uint64\"}]}";
	std::map<HLString, uint64> value;
	value.insert({ "Entry1", 42 });
	value.insert({ "Entry2", 10 });

	Writer->WriteUInt64ArrayMap("value", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_arraymap_multiple)
{
	HLString expected = "{\"value\":[{\"Entry1\":42,\"type\":\"uint64\"},{\"Entry2\":10,\"type\":\"uint64\"}],\"anotherValue\":[{\"Entry3\":42,\"type\":\"uint64\"},{\"Entry4\":10,\"type\":\"uint64\"}]}";
	std::map<HLString, uint64> value;
	value.insert({ "Entry1", 42 });
	value.insert({ "Entry2", 10 });

	std::map<HLString, uint64> anotherValue;
	anotherValue.insert({ "Entry3", 42 });
	anotherValue.insert({ "Entry4", 10 });

	Writer->WriteUInt64ArrayMap("value", value);
	Writer->WriteUInt64ArrayMap("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_array_direct)
{
	HLString expected = "[42]";
	uint64 value = 42;

	Writer->BeginArray();
	Writer->WriteUInt64("value", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_array_direct_multiple)
{
	HLString expected = "[42,10]";
	uint64 value = 42;
	uint64 anotherValue = 10;

	Writer->BeginArray();
	Writer->WriteUInt64("value", value);
	Writer->WriteUInt64("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_arraymap_direct)
{
	HLString expected = "[{\"value\":42,\"type\":\"uint64\"}]";
	uint64 value = 42;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteUInt64("value", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_uint64_arraymap_direct_multiple)
{
	HLString expected = "[[{\"value\":42,\"type\":\"uint64\"},{\"anotherValue\":10,\"type\":\"uint64\"}]]";
	uint64 value = 42;
	uint64 anotherValue = 10;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteUInt64("value", value);
	Writer->WriteUInt64("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Float Tests  ============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_float)
{
	HLString expected = "{\"value\":42.5}";
	float value = 42.5f;

	Writer->WriteFloat("value", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_float_multiple)
{
	HLString expected = "{\"value\":42.5,\"anotherValue\":12.5}";
	float value = 42.5f;
	float anotherValue = 12.5f;

	Writer->WriteFloat("value", value);
	Writer->WriteFloat("anotherValue", anotherValue);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_float_typesafe)
{
	HLString expected = "{\"value\":{\"value\":42.5,\"type\":\"float\"}}";
	float value = 42.5f;

	Writer->BeginObject();
	Writer->WriteFloat("value", value);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_float_typesafe_multiple)
{
	HLString expected = "{\"value\":{\"value\":42.5,\"type\":\"float\"},\"anotherValue\":{\"value\":12.5,\"type\":\"float\"}}";
	float value = 42.5f;
	float anotherValue = 12.5f;

	Writer->BeginObject();
	Writer->WriteFloat("value", value);
	Writer->WriteFloat("anotherValue", anotherValue);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_float_array)
{

}

TEST_F(JsonWriteParserTests, write_float_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_float_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_float_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_float_array_direct)
{
	HLString expected = "[42.5]";
	float value = 42.5f;

	Writer->BeginArray();
	Writer->WriteFloat("value", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_float_array_direct_multiple)
{
	HLString expected = "[42.5,12.5]";
	float value = 42.5f;
	float anotherValue = 12.5f;

	Writer->BeginArray();
	Writer->WriteFloat("value", value);
	Writer->WriteFloat("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_float_arraymap_direct)
{
	HLString expected = "[{\"value\":42.5,\"type\":\"float\"}]";
	float value = 42.5f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteFloat("value", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_float_arraymap_direct_multiple)
{
	HLString expected = "[[{\"value\":42.5,\"type\":\"float\"},{\"anotherValue\":12.5,\"type\":\"float\"}]]";
	float value = 42.5f;
	float anotherValue = 12.5f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteFloat("value", value);
	Writer->WriteFloat("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Double Tests  ===========================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_double)
{
	HLString expected = "{\"value\":42.5}";
	double value = 42.5;

	Writer->WriteDouble("value", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_double_multiple)
{
	HLString expected = "{\"value\":42.5,\"anotherValue\":12.5}";
	double value = 42.5;
	double anotherValue = 12.5;

	Writer->WriteDouble("value", value);
	Writer->WriteDouble("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_double_typesafe)
{
	HLString expected = "{\"value\":{\"value\":42.5,\"type\":\"double\"}}";
	double value = 42.5;

	Writer->BeginObject();
	Writer->WriteDouble("value", value);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_double_typesafe_multiple)
{
	HLString expected = "{\"value\":{\"value\":42.5,\"type\":\"double\"},\"anotherValue\":{\"value\":12.5,\"type\":\"double\"}}";
	double value = 42.5;
	double anotherValue = 12.5;

	Writer->BeginObject();
	Writer->WriteDouble("value", value);
	Writer->WriteDouble("anotherValue", anotherValue);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_double_array)
{

}

TEST_F(JsonWriteParserTests, write_double_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_double_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_double_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_double_array_direct)
{
	HLString expected = "[42.5]";
	double value = 42.5;

	Writer->BeginArray();
	Writer->WriteDouble("value", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_double_array_direct_multiple)
{
	HLString expected = "[42.5,12.5]";
	double value = 42.5;
	double anotherValue = 12.5;

	Writer->BeginArray();
	Writer->WriteDouble("value", value);
	Writer->WriteDouble("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_double_arraymap_direct)
{
	HLString expected = "[{\"value\":42.5,\"type\":\"double\"}]";
	double value = 42.5;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteDouble("value", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_double_arraymap_direct_multiple)
{
	HLString expected = "[[{\"value\":42.5,\"type\":\"double\"},{\"anotherValue\":12.5,\"type\":\"double\"}]]";
	double value = 42.5;
	double anotherValue = 12.5;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteDouble("value", value);
	Writer->WriteDouble("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Bool Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_bool)
{
	HLString expected = "{\"value\":true}";
	bool value = true;

	Writer->WriteBool("value", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_bool_multiple)
{
	HLString expected = "{\"value\":true,\"anotherValue\":false}";
	bool value = true;
	bool anotherValue = false;

	Writer->WriteBool("value", value);
	Writer->WriteBool("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_bool_typesafe)
{
	HLString expected = "{\"value\":{\"value\":true,\"type\":\"bool\"}}";
	bool value = true;

	Writer->BeginObject();
	Writer->WriteBool("value", value);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_bool_typesafe_multiple)
{
	HLString expected = "{\"value\":{\"value\":true,\"type\":\"bool\"},\"anotherValue\":{\"value\":false,\"type\":\"bool\"}}";
	bool value = true;
	bool anotherValue = false;

	Writer->BeginObject();
	Writer->WriteBool("value", value);
	Writer->WriteBool("anotherValue", anotherValue);
	Writer->EndObject();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_bool_array)
{

}

TEST_F(JsonWriteParserTests, write_bool_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_bool_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_bool_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_bool_array_direct)
{
	HLString expected = "[true]";
	bool value = true;

	Writer->BeginArray();
	Writer->WriteBool("value", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_bool_array_direct_multiple)
{
	HLString expected = "[true,false]";
	bool value = true;
	bool anotherValue = false;

	Writer->BeginArray();
	Writer->WriteBool("value", value);
	Writer->WriteBool("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_bool_arraymap_direct)
{
	HLString expected = "[{\"value\":true,\"type\":\"bool\"}]";
	bool value = true;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteBool("value", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_bool_arraymap_direct_multiple)
{
	HLString expected = "[[{\"value\":true,\"type\":\"bool\"},{\"anotherValue\":false,\"type\":\"bool\"}]]";
	bool value = true;
	bool anotherValue = false;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteBool("value", value);
	Writer->WriteBool("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Vec2 Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_vec2)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5],\"type\":\"vec2\"}}";
	glm::vec2 value = { 12.5f, 42.5f };

	Writer->WriteVec2("test", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec2_multiple)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5],\"type\":\"vec2\"},\"anotherValue\":{\"value\":[6.0,1205.4],\"type\":\"vec2\"}}";
	glm::vec2 value = { 12.5f, 42.5f };
	glm::vec2 anotherValue = { 6.0f, 1205.4f };

	Writer->WriteVec2("test", value);
	Writer->WriteVec2("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec2_array)
{

}

TEST_F(JsonWriteParserTests, write_vec2_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_vec2_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_vec2_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_vec2_array_direct)
{
	HLString expected = "[{\"value\":[12.5,42.5],\"type\":\"vec2\"}]";
	glm::vec2 value = { 12.5f, 42.5f };

	Writer->BeginArray();
	Writer->WriteVec2("test", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec2_array_direct_multiple)
{
	HLString expected = "[{\"value\":[12.5,42.5],\"type\":\"vec2\"},{\"value\":[6.0,1205.4],\"type\":\"vec2\"}]";
	glm::vec2 value = { 12.5f, 42.5f };
	glm::vec2 anotherValue = { 6.0f, 1205.4f };

	Writer->BeginArray();
	Writer->WriteVec2("test", value);
	Writer->WriteVec2("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec2_arraymap_direct)
{
	HLString expected = "[{\"test\":[12.5,42.5],\"type\":\"vec2\"}]";
	glm::vec2 value = { 12.5f, 42.5f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteVec2("test", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec2_arraymap_direct_multiple)
{
	HLString expected = "[[{\"test\":[12.5,42.5],\"type\":\"vec2\"},{\"anotherValue\":[6.0,1205.4],\"type\":\"vec2\"}]]";
	glm::vec2 value = { 12.5f, 42.5f };
	glm::vec2 anotherValue = { 6.0f, 1205.4f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteVec2("test", value);
	Writer->WriteVec2("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Vec3 Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_vec3)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5],\"type\":\"vec3\"}}";
	glm::vec3 value = { 12.5f, 42.5f, 16.5f };

	Writer->WriteVec3("test", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec3_multiple)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5],\"type\":\"vec3\"},\"anotherValue\":{\"value\":[6.0,1205.4,42.5],\"type\":\"vec3\"}}";
	glm::vec3 value = { 12.5f, 42.5f, 16.5f };
	glm::vec3 anotherValue = { 6.0f, 1205.4f, 42.5f };

	Writer->WriteVec3("test", value);
	Writer->WriteVec3("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec3_array)
{

}

TEST_F(JsonWriteParserTests, write_vec3_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_vec3_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_vec3_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_vec3_array_direct)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5],\"type\":\"vec3\"}]";
	glm::vec3 value = { 12.5f, 42.5f, 16.5f };

	Writer->BeginArray();
	Writer->WriteVec3("test", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec3_array_direct_multiple)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5],\"type\":\"vec3\"},{\"value\":[6.0,1205.4,42.5],\"type\":\"vec3\"}]";
	glm::vec3 value = { 12.5f, 42.5f, 16.5f };
	glm::vec3 anotherValue = { 6.0f, 1205.4f, 42.5f };

	Writer->BeginArray();
	Writer->WriteVec3("test", value);
	Writer->WriteVec3("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec3_arraymap_direct)
{
	HLString expected = "[{\"test\":[12.5,42.5,16.5],\"type\":\"vec3\"}]";
	glm::vec3 value = { 12.5f, 42.5f, 16.5f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteVec3("test", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec3_arraymap_direct_multiple)
{
	HLString expected = "[[{\"test\":[12.5,42.5,16.5],\"type\":\"vec3\"},{\"anotherValue\":[6.0,1205.4,42.5],\"type\":\"vec3\"}]]";
	glm::vec3 value = { 12.5f, 42.5f, 16.5f };
	glm::vec3 anotherValue = { 6.0f, 1205.4f, 42.5f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteVec3("test", value);
	Writer->WriteVec3("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Vec4 Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_vec4)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"vec4\"}}";
	glm::vec4 value = { 12.5f, 42.5f, 16.5f, 498.5f };

	Writer->WriteVec4("test", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec4_multiple)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"vec4\"},\"anotherValue\":{\"value\":[16.5,157.0,1234.5,146.0],\"type\":\"vec4\"}}";
	glm::vec4 value = { 12.5f, 42.5f, 16.5f, 498.5f };
	glm::vec4 anotherValue = { 16.5f, 157.0f, 1234.5f, 146.0f };

	Writer->WriteVec4("test", value);
	Writer->WriteVec4("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec4_array)
{

}

TEST_F(JsonWriteParserTests, write_vec4_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_vec4_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_vec4_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_vec4_array_direct)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"vec4\"}]";
	glm::vec4 value = { 12.5f, 42.5f, 16.5f, 498.5f };

	Writer->BeginArray();
	Writer->WriteVec4("test", value);
	Writer->EndArray();
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec4_array_direct_multiple)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"vec4\"},{\"value\":[16.5,157.0,1234.5,146.0],\"type\":\"vec4\"}]";
	glm::vec4 value = { 12.5f, 42.5f, 16.5f, 498.5f };
	glm::vec4 anotherValue = { 16.5f, 157.0f, 1234.5f, 146.0f };

	Writer->BeginArray();
	Writer->WriteVec4("test", value);
	Writer->WriteVec4("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec4_arraymap_direct)
{
	HLString expected = "[{\"test\":[12.5,42.5,16.5,498.5],\"type\":\"vec4\"}]";
	glm::vec4 value = { 12.5f, 42.5f, 16.5f, 498.5f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteVec4("test", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_vec4_arraymap_direct_multiple)
{
	HLString expected = "[[{\"test\":[12.5,42.5,16.5,498.5],\"type\":\"vec4\"},{\"anotherValue\":[16.5,157.0,1234.5,146.0],\"type\":\"vec4\"}]]";
	glm::vec4 value = { 12.5f, 42.5f, 16.5f, 498.5f };
	glm::vec4 anotherValue = { 16.5f, 157.0f, 1234.5f, 146.0f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteVec4("test", value);
	Writer->WriteVec4("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Mat2 Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_mat2)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"mat2\"}}";
	
	glm::mat2 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[1][0] = 16.5f;
	value[1][1] = 498.5f;

	Writer->WriteMat2("test", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat2_multiple)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"mat2\"},\"anotherValue\":{\"value\":[2324.5,123.5,1345.0,464.0],\"type\":\"mat2\"}}";

	glm::mat2 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[1][0] = 16.5f;
	value[1][1] = 498.5f;

	glm::mat2 anotherValue;
	anotherValue[0][0] = 2324.5f;
	anotherValue[0][1] = 123.5f;
	anotherValue[1][0] = 1345.0f;
	anotherValue[1][1] = 464.0f;

	Writer->WriteMat2("test", value);
	Writer->WriteMat2("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat2_array)
{

}

TEST_F(JsonWriteParserTests, write_mat2_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_mat2_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_mat2_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_mat2_array_direct)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"mat2\"}]";

	glm::mat2 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[1][0] = 16.5f;
	value[1][1] = 498.5f;

	Writer->BeginArray();
	Writer->WriteMat2("test", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat2_array_direct_multiple)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"mat2\"},{\"value\":[2324.5,123.5,1345.0,464.0],\"type\":\"mat2\"}]";

	glm::mat2 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[1][0] = 16.5f;
	value[1][1] = 498.5f;

	glm::mat2 anotherValue;
	anotherValue[0][0] = 2324.5f;
	anotherValue[0][1] = 123.5f;
	anotherValue[1][0] = 1345.0f;
	anotherValue[1][1] = 464.0f;

	Writer->BeginArray();
	Writer->WriteMat2("test", value);
	Writer->WriteMat2("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat2_arraymap_direct)
{
	HLString expected = "[{\"test\":[12.5,42.5,16.5,498.5],\"type\":\"mat2\"}]";

	glm::mat2 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[1][0] = 16.5f;
	value[1][1] = 498.5f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteMat2("test", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat2_arraymap_direct_multiple)
{
	HLString expected = "[[{\"test\":[12.5,42.5,16.5,498.5],\"type\":\"mat2\"},{\"anotherValue\":[2324.5,123.5,1345.0,464.0],\"type\":\"mat2\"}]]";

	glm::mat2 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[1][0] = 16.5f;
	value[1][1] = 498.5f;

	glm::mat2 anotherValue;
	anotherValue[0][0] = 2324.5f;
	anotherValue[0][1] = 123.5f;
	anotherValue[1][0] = 1345.0f;
	anotherValue[1][1] = 464.0f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteMat2("test", value);
	Writer->WriteMat2("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Mat3 Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_mat3)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,456.0,16.5,498.5,457.0,4245.0,32.5,343245.0],\"type\":\"mat3\"}}";

	glm::mat3 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;
	
	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;

	Writer->WriteMat3("test", value);

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat3_multiple)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,456.0,16.5,498.5,457.0,4245.0,32.5,343245.0],\"type\":\"mat3\"},\"anotherValue\":{\"value\":[246.0,536.0,757.0,757.5,646.5,75.5,753.0,645.0,64.0],\"type\":\"mat3\"}}";

	glm::mat3 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;

	glm::mat3 anotherValue;
	anotherValue[0][0] = 246.0f;
	anotherValue[0][1] = 536.0f;
	anotherValue[0][2] = 757.0f;

	anotherValue[1][0] = 757.5f;
	anotherValue[1][1] = 646.5f;
	anotherValue[1][2] = 75.5f;

	anotherValue[2][0] = 753.0f;
	anotherValue[2][1] = 645.0f;
	anotherValue[2][2] = 64.0f;

	Writer->WriteMat3("test", value);
	Writer->WriteMat3("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat3_array)
{

}

TEST_F(JsonWriteParserTests, write_mat3_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_mat3_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_mat3_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_mat3_array_direct)
{
	HLString expected = "[{\"value\":[12.5,42.5,456.0,16.5,498.5,457.0,4245.0,32.5,343245.0],\"type\":\"mat3\"}]";

	glm::mat3 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;

	Writer->BeginArray();
	Writer->WriteMat3("test", value);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat3_array_direct_multiple)
{
	HLString expected = "[{\"value\":[12.5,42.5,456.0,16.5,498.5,457.0,4245.0,32.5,343245.0],\"type\":\"mat3\"},{\"value\":[246.0,536.0,757.0,757.5,646.5,75.5,753.0,645.0,64.0],\"type\":\"mat3\"}]";

	glm::mat3 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;

	glm::mat3 anotherValue;
	anotherValue[0][0] = 246.0f;
	anotherValue[0][1] = 536.0f;
	anotherValue[0][2] = 757.0f;

	anotherValue[1][0] = 757.5f;
	anotherValue[1][1] = 646.5f;
	anotherValue[1][2] = 75.5f;

	anotherValue[2][0] = 753.0f;
	anotherValue[2][1] = 645.0f;
	anotherValue[2][2] = 64.0f;

	Writer->BeginArray();
	Writer->WriteMat3("test", value);
	Writer->WriteMat3("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat3_arraymap_direct)
{
	HLString expected = "[{\"test\":[12.5,42.5,456.0,16.5,498.5,457.0,4245.0,32.5,343245.0],\"type\":\"mat3\"}]";

	glm::mat3 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteMat3("test", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat3_arraymap_direct_multiple)
{
	HLString expected = "[[{\"test\":[12.5,42.5,456.0,16.5,498.5,457.0,4245.0,32.5,343245.0],\"type\":\"mat3\"},{\"anotherValue\":[246.0,536.0,757.0,757.5,646.5,75.5,753.0,645.0,64.0],\"type\":\"mat3\"}]]";

	glm::mat3 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;

	glm::mat3 anotherValue;
	anotherValue[0][0] = 246.0f;
	anotherValue[0][1] = 536.0f;
	anotherValue[0][2] = 757.0f;

	anotherValue[1][0] = 757.5f;
	anotherValue[1][1] = 646.5f;
	anotherValue[1][2] = 75.5f;

	anotherValue[2][0] = 753.0f;
	anotherValue[2][1] = 645.0f;
	anotherValue[2][2] = 64.0f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteMat3("test", value);
	Writer->WriteMat3("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Mat4 Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_mat4)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,456.0,435.0,16.5,498.5,457.0,324.0,4245.0,32.5,343245.0,4353.5,3245.0,757.5,4353.0,475.5],\"type\":\"mat4\"}}";

	glm::mat4 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;
	value[0][3] = 435.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;
	value[1][3] = 324.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;
	value[2][3] = 4353.5f;

	value[3][0] = 3245.0f;
	value[3][1] = 757.5f;
	value[3][2] = 4353.0f;
	value[3][3] = 475.5f;

	Writer->WriteMat4("test", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat4_multiple)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,456.0,435.0,16.5,498.5,457.0,324.0,4245.0,32.5,343245.0,4353.5,3245.0,757.5,4353.0,475.5],\"type\":\"mat4\"},\"anotherValue\":{\"value\":[246.0,536.0,757.0,457.0,757.5,646.5,75.5,46.5,753.0,645.0,64.0,5445.0,53.5,324.5,423.5,63.5],\"type\":\"mat4\"}}";

	glm::mat4 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;
	value[0][3] = 435.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;
	value[1][3] = 324.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;
	value[2][3] = 4353.5f;

	value[3][0] = 3245.0f;
	value[3][1] = 757.5f;
	value[3][2] = 4353.0f;
	value[3][3] = 475.5f;

	glm::mat4 anotherValue;
	anotherValue[0][0] = 246.0f;
	anotherValue[0][1] = 536.0f;
	anotherValue[0][2] = 757.0f;
	anotherValue[0][3] = 457.0f;

	anotherValue[1][0] = 757.5f;
	anotherValue[1][1] = 646.5f;
	anotherValue[1][2] = 75.5f;
	anotherValue[1][3] = 46.5f;

	anotherValue[2][0] = 753.0f;
	anotherValue[2][1] = 645.0f;
	anotherValue[2][2] = 64.0f;
	anotherValue[2][3] = 5445.0f;

	anotherValue[3][0] = 53.5f;
	anotherValue[3][1] = 324.5f;
	anotherValue[3][2] = 423.5f;
	anotherValue[3][3] = 63.5f;

	Writer->WriteMat4("test", value);
	Writer->WriteMat4("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat4_array)
{

}

TEST_F(JsonWriteParserTests, write_mat4_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_mat4_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_mat4_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_mat4_array_direct)
{
	HLString expected = "[{\"value\":[12.5,42.5,456.0,435.0,16.5,498.5,457.0,324.0,4245.0,32.5,343245.0,4353.5,3245.0,757.5,4353.0,475.5],\"type\":\"mat4\"}]";

	glm::mat4 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;
	value[0][3] = 435.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;
	value[1][3] = 324.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;
	value[2][3] = 4353.5f;

	value[3][0] = 3245.0f;
	value[3][1] = 757.5f;
	value[3][2] = 4353.0f;
	value[3][3] = 475.5f;

	Writer->BeginArray();
	Writer->WriteMat4("test", value);
	Writer->EndArray();
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat4_array_direct_multiple)
{
	HLString expected = "[{\"value\":[12.5,42.5,456.0,435.0,16.5,498.5,457.0,324.0,4245.0,32.5,343245.0,4353.5,3245.0,757.5,4353.0,475.5],\"type\":\"mat4\"},{\"value\":[246.0,536.0,757.0,457.0,757.5,646.5,75.5,46.5,753.0,645.0,64.0,5445.0,53.5,324.5,423.5,63.5],\"type\":\"mat4\"}]";

	glm::mat4 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;
	value[0][3] = 435.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;
	value[1][3] = 324.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;
	value[2][3] = 4353.5f;

	value[3][0] = 3245.0f;
	value[3][1] = 757.5f;
	value[3][2] = 4353.0f;
	value[3][3] = 475.5f;

	glm::mat4 anotherValue;
	anotherValue[0][0] = 246.0f;
	anotherValue[0][1] = 536.0f;
	anotherValue[0][2] = 757.0f;
	anotherValue[0][3] = 457.0f;

	anotherValue[1][0] = 757.5f;
	anotherValue[1][1] = 646.5f;
	anotherValue[1][2] = 75.5f;
	anotherValue[1][3] = 46.5f;

	anotherValue[2][0] = 753.0f;
	anotherValue[2][1] = 645.0f;
	anotherValue[2][2] = 64.0f;
	anotherValue[2][3] = 5445.0f;

	anotherValue[3][0] = 53.5f;
	anotherValue[3][1] = 324.5f;
	anotherValue[3][2] = 423.5f;
	anotherValue[3][3] = 63.5f;

	Writer->BeginArray();
	Writer->WriteMat4("test", value);
	Writer->WriteMat4("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat4_arraymap_direct)
{
	HLString expected = "[{\"test\":[12.5,42.5,456.0,435.0,16.5,498.5,457.0,324.0,4245.0,32.5,343245.0,4353.5,3245.0,757.5,4353.0,475.5],\"type\":\"mat4\"}]";

	glm::mat4 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;
	value[0][3] = 435.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;
	value[1][3] = 324.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;
	value[2][3] = 4353.5f;

	value[3][0] = 3245.0f;
	value[3][1] = 757.5f;
	value[3][2] = 4353.0f;
	value[3][3] = 475.5f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteMat4("test", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_mat4_arraymap_direct_multiple)
{
	HLString expected = "[[{\"test\":[12.5,42.5,456.0,435.0,16.5,498.5,457.0,324.0,4245.0,32.5,343245.0,4353.5,3245.0,757.5,4353.0,475.5],\"type\":\"mat4\"},{\"anotherValue\":[246.0,536.0,757.0,457.0,757.5,646.5,75.5,46.5,753.0,645.0,64.0,5445.0,53.5,324.5,423.5,63.5],\"type\":\"mat4\"}]]";

	glm::mat4 value;
	value[0][0] = 12.5f;
	value[0][1] = 42.5f;
	value[0][2] = 456.0f;
	value[0][3] = 435.0f;

	value[1][0] = 16.5f;
	value[1][1] = 498.5f;
	value[1][2] = 457.0f;
	value[1][3] = 324.0f;

	value[2][0] = 4245.0f;
	value[2][1] = 32.5f;
	value[2][2] = 343245.0f;
	value[2][3] = 4353.5f;

	value[3][0] = 3245.0f;
	value[3][1] = 757.5f;
	value[3][2] = 4353.0f;
	value[3][3] = 475.5f;

	glm::mat4 anotherValue;
	anotherValue[0][0] = 246.0f;
	anotherValue[0][1] = 536.0f;
	anotherValue[0][2] = 757.0f;
	anotherValue[0][3] = 457.0f;

	anotherValue[1][0] = 757.5f;
	anotherValue[1][1] = 646.5f;
	anotherValue[1][2] = 75.5f;
	anotherValue[1][3] = 46.5f;

	anotherValue[2][0] = 753.0f;
	anotherValue[2][1] = 645.0f;
	anotherValue[2][2] = 64.0f;
	anotherValue[2][3] = 5445.0f;

	anotherValue[3][0] = 53.5f;
	anotherValue[3][1] = 324.5f;
	anotherValue[3][2] = 423.5f;
	anotherValue[3][3] = 63.5f;

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteMat4("test", value);
	Writer->WriteMat4("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

/// =========================================================================================================
/// ==============================================  Quat Tests  =============================================
/// =========================================================================================================

TEST_F(JsonWriteParserTests, write_quat)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"quat\"}}";
	glm::quat value = { 12.5f, 42.5f, 16.5f, 498.5f };

	Writer->WriteQuaternion("test", value);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_quat_multiple)
{
	HLString expected = "{\"test\":{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"quat\"},\"anotherValue\":{\"value\":[49.5,457.0,46.5,496.5],\"type\":\"quat\"}}";
	glm::quat value = { 12.5f, 42.5f, 16.5f, 498.5f };
	glm::quat anotherValue = { 49.5f, 457.0f, 46.5f, 496.5f };

	Writer->WriteQuaternion("test", value);
	Writer->WriteQuaternion("anotherValue", anotherValue);
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_quat_array)
{

}

TEST_F(JsonWriteParserTests, write_quat_array_multiple)
{

}

TEST_F(JsonWriteParserTests, write_quat_arraymap)
{

}

TEST_F(JsonWriteParserTests, write_quat_arraymap_multiple)
{

}

TEST_F(JsonWriteParserTests, write_quat_array_direct)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"quat\"}]";
	glm::quat value = { 12.5f, 42.5f, 16.5f, 498.5f };

	Writer->BeginArray();
	Writer->WriteQuaternion("test", value);
	Writer->EndArray();
	
	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_quat_array_direct_multiple)
{
	HLString expected = "[{\"value\":[12.5,42.5,16.5,498.5],\"type\":\"quat\"},{\"value\":[49.5,457.0,46.5,496.5],\"type\":\"quat\"}]";
	glm::quat value = { 12.5f, 42.5f, 16.5f, 498.5f };
	glm::quat anotherValue = { 49.5f, 457.0f, 46.5f, 496.5f };

	Writer->BeginArray();
	Writer->WriteQuaternion("test", value);
	Writer->WriteQuaternion("anotherValue", anotherValue);
	Writer->EndArray();

	HLString result = Writer->GetContent();
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_quat_arraymap_direct)
{
	HLString expected = "[{\"test\":[12.5,42.5,16.5,498.5],\"type\":\"quat\"}]";
	glm::quat value = { 12.5f, 42.5f, 16.5f, 498.5f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteQuaternion("test", value);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(JsonWriteParserTests, write_quat_arraymap_direct_multiple)
{
	HLString expected = "[[{\"test\":[12.5,42.5,16.5,498.5],\"type\":\"quat\"},{\"anotherValue\":[49.5,457.0,46.5,496.5],\"type\":\"quat\"}]]";
	glm::quat value = { 12.5f, 42.5f, 16.5f, 498.5f };
	glm::quat anotherValue = { 49.5f, 457.0f, 46.5f, 496.5f };

	Writer->BeginArray();
	Writer->BeginObject();
	Writer->WriteQuaternion("test", value);
	Writer->WriteQuaternion("anotherValue", anotherValue);
	Writer->EndObject();
	Writer->EndArray();

	HLString result = Writer->GetContent();
	std::cout << *result << std::endl;
	EXPECT_EQ(StringEquals(result, expected), true);
}

