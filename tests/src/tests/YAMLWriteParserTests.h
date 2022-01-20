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

struct YAMLWriteParserTests : public testing::Test
{
	Ref<DocumentWriter> Writer;

	YAMLWriteParserTests()
	{
		Writer = DocumentWriter::Create("", DocumentType::Yaml);
	}

	virtual ~YAMLWriteParserTests()
	{
	}
};

TEST_F(YAMLWriteParserTests, YAMLParser_Write_String)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteString("test", "Hello World!");
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Int32)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt32("test", -10);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_UInt32)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt32("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Int64)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_UInt64)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteUInt64("test", 42);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Bool)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteBool("test", true);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Float)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteFloat("test", 20.2f);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Double)
{
	HLString expected = "";

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteDouble("test", 20.2);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(YAMLWriteParserTests, YAMLParser_Write_Vec2)
{
	HLString expected = "";
	glm::vec2 v(20.2f, 10.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec2("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(YAMLWriteParserTests, YAMLParser_Write_Vec3)
{
	HLString expected = "";
	glm::vec3 v(20.2f, 10.5f, 23.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec3("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Vec4)
{
	HLString expected = "";
	glm::vec4 v(20.2f, 10.5f, 23.5f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteVec4("test", v);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Mat2)
{
	HLString expected = "";

	glm::mat2 m;
	m[0][0] = 20.2f;
	m[0][1] = 10.5f;
	m[1][0] = 23.5f;
	m[1][1] = 11.5f;

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteMat2("test", m);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


TEST_F(YAMLWriteParserTests, YAMLParser_Write_Mat3)
{
	HLString expected = "";

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
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Mat4)
{
	HLString expected = "";

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
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_quat)
{
	HLString expected = "";
	glm::quat q(20.2f, 10.5f, 23.1f, 11.5f);

	Writer->BeginObject();
	bool writeSuccess = Writer->WriteQuaternion("test", q);
	Writer->EndObject(true);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

///
/// Array
/// 

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Int32)
{
	HLString expected = "";
	std::vector<int32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(-10);

	bool writeSuccess = Writer->WriteInt32Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_UInt32)
{
	HLString expected = "";
	std::vector<uint32> arr;
	arr.push_back(42);
	arr.push_back(10);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteUInt32Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Int64)
{
	HLString expected = "";
	std::vector<int64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteInt64Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_UInt64)
{
	HLString expected = "";
	std::vector<uint64> arr;
	arr.push_back(452);
	arr.push_back(105);
	arr.push_back(100);

	bool writeSuccess = Writer->WriteUInt64Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Float)
{
	HLString expected = "";
	std::vector<float> arr;
	arr.push_back(44.0f);
	arr.push_back(10.5f);
	arr.push_back(16.0f);

	bool writeSuccess = Writer->WriteFloatArray("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Double)
{
	HLString expected = "";
	std::vector<double> arr;
	arr.push_back(44.1);
	arr.push_back(10.5);
	arr.push_back(16.6);

	bool writeSuccess = Writer->WriteDoubleArray("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Bool)
{
	HLString expected = "";
	std::vector<bool> arr;
	arr.push_back(true);
	arr.push_back(false);
	arr.push_back(true);

	bool writeSuccess = Writer->WriteBoolArray("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_String)
{
	HLString expected = "";
	std::vector<HLString> arr;
	arr.push_back("Hello");
	arr.push_back("world");
	arr.push_back("!");

	bool writeSuccess = Writer->WriteStringArray("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Vec2)
{
	HLString expected = "";
	std::vector<glm::vec2> arr;
	arr.push_back({ 1.5f, 6.0f });
	arr.push_back({ 2.5f, 1.0f });
	arr.push_back({ 1.0f, 0.0f });

	bool writeSuccess = Writer->WriteVec2Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Vec3)
{
	HLString expected = "";
	std::vector<glm::vec3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f });
	arr.push_back({ 2.5f, 1.0f, 5.8f });
	arr.push_back({ 1.0f, 0.0f, 5.5f });

	bool writeSuccess = Writer->WriteVec3Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Vec4)
{
	HLString expected = "";
	std::vector<glm::vec4> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	bool writeSuccess = Writer->WriteVec4Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Mat2)
{
	HLString expected = "";
	std::vector<glm::mat2> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	bool writeSuccess = Writer->WriteMat2Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Mat3)
{
	HLString expected = "";
	std::vector<glm::mat3> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f, 142.0f, 155.0f, 231.0f, 1.753f, 3571.05f });
	arr.push_back({ 2.5f, 1.0f, 5.8f, 2.9f, 15.0f, 152.0f, 7451.0f, 175.0f, 1573.03f });
	arr.push_back({ 1.0f, 0.0f, 5.5f, 2.5f, 1.20f, 165.0f, 861.0f, 341.0f, 1375.750f });

	bool writeSuccess = Writer->WriteMat3Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Mat4)
{
	HLString expected = "";
	std::vector<glm::mat4> arr;
	arr.push_back(glm::mat4(0.0f));
	arr.push_back(glm::mat4(1.0f));
	arr.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	bool writeSuccess = Writer->WriteMat4Array("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Complete_Array_Quat)
{
	HLString expected = "";
	std::vector<glm::quat> arr;
	arr.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	arr.push_back({ 164.5f, 326.5f, 235.3f, 32.0f });
	arr.push_back({ 14.5f, 32.5f, 23.53f, 3.20f });

	bool writeSuccess = Writer->WriteQuaternionArray("", arr);

	HLString content = Writer->GetContent();
	std::cout << *content << std::endl;

	EXPECT_EQ(writeSuccess, true);
	EXPECT_EQ(StringEquals(expected, content), true);
}


///
/// ArrayMap
/// 

TEST_F(YAMLWriteParserTests, YAMLParser_Write_String_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Int32_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_UInt32_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Int64_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_UInt64_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Bool_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Float_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Double_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Vec2_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Vec3_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Vec4_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Mat2_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Mat3_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Mat4_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

TEST_F(YAMLWriteParserTests, YAMLParser_Write_Quat_Array)
{
	HLString expected = "";
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
	std::cout << *content << std::endl;

	EXPECT_EQ(StringEquals(expected, content), true);
}

