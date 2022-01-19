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

struct XMLReadParserTests : public testing::Test
{
	Ref<DocumentWriter> Reader;

	XMLReadParserTests()
	{
		Reader = DocumentWriter::Create("", DocumentType::XML);
		Logger::Init();
	}

	virtual ~XMLReadParserTests()
	{
		Logger::Shutdown();
	}
};

TEST_F(XMLReadParserTests, XMLParser_Read_String)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><string>Hello World!</string></HighLo>";
	HLString expected = "Hello World!";
	HLString result = "";

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadString("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, JSONParser_Read_Int32)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><int32>-10</int32></HighLo>";
	int32 expected = -10;
	int32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_UInt32)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><uint32>42</uint32></HighLo>";
	uint32 expected = 42;
	uint32 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Int64)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><int64>42</int64></HighLo>";
	int64 expected = 42;
	int64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_UInt64)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><uint64>42</uint64></HighLo>";
	uint64 expected = 42;
	uint64 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Bool)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><bool>1</bool></HighLo>";
	bool expected = true;
	bool result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBool("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Float)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><float>20.2</float></HighLo>";
	float expected = 20.2f;
	float result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloat("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Double)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><double>20.2</double></HighLo>";
	double expected = 20.2;
	double result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDouble("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Vec2)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><vec2><x>20.2</x><y>10.5</y></vec2></HighLo>";
	glm::vec2 expected = { 20.2f, 10.5f };
	glm::vec2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector2("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Vec3)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><vec3><x>20.2</x><y>10.5</y><z>23.5</z></vec3></HighLo>";
	glm::vec3 expected = { 20.2f, 10.5f, 23.5f };
	glm::vec3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector3("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Vec4)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><vec4><x>20.2</x><y>10.5</y><z>23.5</z><w>11.5</w></vec4></HighLo>";
	glm::vec4 expected = { 20.2f, 10.5f, 23.5f, 11.5f };
	glm::vec4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVector4("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Mat2)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><mat2><m00>20.2</m00><m01>10.5</m01><m10>23.5</m10><m11>11.5</m11></mat2></HighLo>";
	glm::mat2 expected = glm::mat2(20.2f, 10.5f, 23.5f, 11.5f);
	glm::mat2 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix2("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Mat3)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><mat3><m00>20.2</m00><m01>10.5</m01><m02>23.5</m02><m10>11.5</m10><m11>534.5</m11><m12>234.5</m12><m20>12.5</m20><m21>45.7</m21><m22>42.4</m22></mat3></HighLo>";
	glm::mat3 expected = glm::mat3(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f);
	glm::mat3 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix3("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Mat4)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><mat4><m00>20.2</m00><m01>10.5</m01><m02>23.5</m02><m03>11.5</m03><m10>534.5</m10><m11>234.5</m11><m12>12.5</m12><m13>45.7</m13><m20>42.4</m20><m21>47.2</m21><m22>55.4</m22><m23>48.4</m23><m30>32</m30><m31>15.1</m31><m32>499.1</m32><m33>22.5</m33></mat4></HighLo>";
	glm::mat4 expected = glm::mat4(20.2f, 10.5f, 23.5f, 11.5f, 534.5f, 234.5f, 12.5f, 45.7f, 42.4f, 47.2f, 55.4f, 48.4f, 32.0f, 15.1f, 499.1f, 22.5f);
	glm::mat4 result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMatrix4("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

TEST_F(XMLReadParserTests, XMLParser_Read_quat)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><quat><w>20.2</w><x>10.5</x><y>23.1</y><z>11.5</z></quat></HighLo>";
	glm::quat expected = glm::quat(20.2f, 10.5f, 23.1f, 11.5f);
	glm::quat result;

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuaternion("", &result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(result, expected);
}

///
/// Array
/// 

TEST_F(XMLReadParserTests, XMLParser_Read_String_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><string>Hello</string><string>world</string><string>!</string></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_String_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKey\"><string>Hello</string><string>world</string><string>!</string></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Int32_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><int32>42</int32><int32>10</int32><int32>-10</int32></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Int32_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><int32>42</int32><int32>10</int32><int32>-10</int32></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_UInt32_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><uint32>42</uint32><uint32>10</uint32><uint32>12</uint32></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_UInt32_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><uint32>42</uint32><uint32>10</uint32><uint32>12</uint32></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Int64_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><int64>42</int64><int64>10</int64><int64>12</int64></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Int64_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><int64>42</int64><int64>10</int64><int64>12</int64></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_UInt64_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><uint64>42</uint64><uint64>10</uint64><uint64>12</uint64></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_UInt64_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><uint64>42</uint64><uint64>10</uint64><uint64>12</uint64></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Bool_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><bool>1</bool><bool>0</bool><bool>1</bool></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Bool_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><bool>1</bool><bool>0</bool><bool>1</bool></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Float_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><float>2.5</float><float>6.3</float><float>23</float></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Float_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><float>2.5</float><float>6.3</float><float>23</float></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Double_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><double>2.5</double><double>6.3</double><double>23</double></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Double_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><double>2.5</double><double>6.3</double><double>23</double></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Vec2_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><vec2><x>1.5</x><y>6</y></vec2><vec2><x>2.5</x><y>1</y></vec2><vec2><x>1</x><y>0</y></vec2></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Vec2_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><vec2><x>1.5</x><y>6</y></vec2><vec2><x>2.5</x><y>1</y></vec2><vec2><x>1</x><y>0</y></vec2></array></HighLo>";
	std::vector<glm::vec2> expected;
	std::vector<glm::vec2> result;

	expected.push_back({ 1.5f, 6.0f });
	expected.push_back({ 2.5f, 1.0f });
	expected.push_back({ 1.0f, 0.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Vec3_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><vec3><x>2.5</x><y>2</y><z>2.9</z></vec3><vec3><x>6.3</x><y>4.5</y><z>2</z></vec3><vec3><x>23</x><y>9</y><z>5</z></vec3></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Vec3_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><vec3><x>2.5</x><y>2</y><z>2.9</z></vec3><vec3><x>6.3</x><y>4.5</y><z>2</z></vec3><vec3><x>23</x><y>9</y><z>5</z></vec3></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Vec4_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><vec4><x>1.5</x><y>6</y><z>5.3</z><w>2.7</w></vec4><vec4><x>2.5</x><y>1</y><z>5.8</z><w>2.9</w></vec4><vec4><x>1</x><y>0</y><z>5.5</z><w>2.5</w></vec4></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Vec4_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><vec4><x>1.5</x><y>6</y><z>5.3</z><w>2.7</w></vec4><vec4><x>2.5</x><y>1</y><z>5.8</z><w>2.9</w></vec4><vec4><x>1</x><y>0</y><z>5.5</z><w>2.5</w></vec4></array></HighLo>";
	std::vector<glm::vec4> expected;
	std::vector<glm::vec4> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	expected.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	expected.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Mat2_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><mat2><m00>1.5</m00><m01>6</m01><m10>5.3</m10><m11>2.7</m11></mat2><mat2><m00>2.5</m00><m01>1</m01><m10>5.8</m10><m11>2.9</m11></mat2><mat2><m00>1</m00><m01>0</m01><m10>5.5</m10><m11>2.5</m11></mat2></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Mat2_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><mat2><m00>1.5</m00><m01>6</m01><m10>5.3</m10><m11>2.7</m11></mat2><mat2><m00>2.5</m00><m01>1</m01><m10>5.8</m10><m11>2.9</m11></mat2><mat2><m00>1</m00><m01>0</m01><m10>5.5</m10><m11>2.5</m11></mat2></array></HighLo>";
	std::vector<glm::mat2> expected;
	std::vector<glm::mat2> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	expected.push_back({ 2.5f, 1.0f, 5.8f, 2.9f });
	expected.push_back({ 1.0f, 0.0f, 5.5f, 2.5f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Mat3_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><mat3><m00>1.5</m00><m01>6</m01><m02>5.3</m02><m10>2.7</m10><m11>142</m11><m12>155</m12><m20>231</m20><m21>1.753</m21><m22>3571.05</m22></mat3><mat3><m00>2.5</m00><m01>1</m01><m02>5.8</m02><m10>2.9</m10><m11>15</m11><m12>152</m12><m20>7451</m20><m21>175</m21><m22>1573.03</m22></mat3><mat3><m00>1</m00><m01>0</m01><m02>5.5</m02><m10>2.5</m10><m11>1.2</m11><m12>165</m12><m20>861</m20><m21>341</m21><m22>1375.75</m22></mat3></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Mat3_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><mat3><m00>1.5</m00><m01>6</m01><m02>5.3</m02><m10>2.7</m10><m11>142</m11><m12>155</m12><m20>231</m20><m21>1.753</m21><m22>3571.05</m22></mat3><mat3><m00>2.5</m00><m01>1</m01><m02>5.8</m02><m10>2.9</m10><m11>15</m11><m12>152</m12><m20>7451</m20><m21>175</m21><m22>1573.03</m22></mat3><mat3><m00>1</m00><m01>0</m01><m02>5.5</m02><m10>2.5</m10><m11>1.2</m11><m12>165</m12><m20>861</m20><m21>341</m21><m22>1375.75</m22></mat3></array></HighLo>";
	std::vector<glm::mat3> expected;
	std::vector<glm::mat3> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f, 142.0f, 155.0f, 231.0f, 1.753f, 3571.05f });
	expected.push_back({ 2.5f, 1.0f, 5.8f, 2.9f, 15.0f, 152.0f, 7451.0f, 175.0f, 1573.03f });
	expected.push_back({ 1.0f, 0.0f, 5.5f, 2.5f, 1.20f, 165.0f, 861.0f, 341.0f, 1375.750f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Mat4_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><mat4><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></mat4><mat4><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></mat4><mat4><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></mat4></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Mat4_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><mat4><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></mat4><mat4><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></mat4><mat4><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></mat4></array></HighLo>";
	std::vector<glm::mat4> expected;
	std::vector<glm::mat4> result;

	expected.push_back(glm::mat4(0.0f));
	expected.push_back(glm::mat4(1.0f));
	expected.push_back({ 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4Array("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Quat_Array)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array><quat><w>1.5</w><x>6</x><y>5.3</y><z>2.7</z></quat><quat><w>164.5</w><x>326.5</x><y>235.3</y><z>32</z></quat><quat><w>14.5</w><x>32.5</x><y>23.53</y><z>3.2</z></quat></array></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Quat_Array_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><array key=\"arrayKeys\"><quat><w>1.5</w><x>6</x><y>5.3</y><z>2.7</z></quat><quat><w>164.5</w><x>326.5</x><y>235.3</y><z>32</z></quat><quat><w>14.5</w><x>32.5</x><y>23.53</y><z>3.2</z></quat></array></HighLo>";
	std::vector<glm::quat> expected;
	std::vector<glm::quat> result;

	expected.push_back({ 1.5f, 6.0f, 5.3f, 2.7f });
	expected.push_back({ 164.5f, 326.5f, 235.3f, 32.0f });
	expected.push_back({ 14.5f, 32.5f, 23.53f, 3.20f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArray("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(VectorEquals(result, expected), true);
}

///
/// ArrayMap
///

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Int32)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><int32 key=\"object0\">42</int32><int32 key=\"object1\">10</int32><int32 key=\"object2\">-10</int32></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Int32_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><int32 key=\"object0\">42</int32><int32 key=\"object1\">10</int32><int32 key=\"object2\">-10</int32></arraymap></HighLo>";
	std::map<HLString, int32> expected;
	std::map<HLString, int32> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", -10 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt32ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_UInt32)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><uint32 key=\"object0\">42</uint32><uint32 key=\"object1\">10</uint32><uint32 key=\"object2\">12</uint32></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_UInt32_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><uint32 key=\"object0\">42</uint32><uint32 key=\"object1\">10</uint32><uint32 key=\"object2\">12</uint32></arraymap></HighLo>";
	std::map<HLString, uint32> expected;
	std::map<HLString, uint32> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt32ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Int64)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><int64 key=\"object0\">42</int64><int64 key=\"object1\">10</int64><int64 key=\"object2\">12</int64></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Int64_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><int64 key=\"object0\">42</int64><int64 key=\"object1\">10</int64><int64 key=\"object2\">12</int64></arraymap></HighLo>";
	std::map<HLString, int64> expected;
	std::map<HLString, int64> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadInt64ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_UInt64)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><uint64 key=\"object0\">42</uint64><uint64 key=\"object1\">10</uint64><uint64 key=\"object2\">12</uint64></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_UInt64_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><uint64 key=\"object0\">42</uint64><uint64 key=\"object1\">10</uint64><uint64 key=\"object2\">12</uint64></arraymap></HighLo>";
	std::map<HLString, uint64> expected;
	std::map<HLString, uint64> result;

	expected.insert({ "object0", 42 });
	expected.insert({ "object1", 10 });
	expected.insert({ "object2", 12 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadUInt64ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Float)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><float key=\"object0\">2.5</float><float key=\"object1\">6.3</float><float key=\"object2\">23</float></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Float_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><float key=\"object0\">2.5</float><float key=\"object1\">6.3</float><float key=\"object2\">23</float></arraymap></HighLo>";
	std::map<HLString, float> expected;
	std::map<HLString, float> result;

	expected.insert({ "object0", 2.5f });
	expected.insert({ "object1", 6.3f });
	expected.insert({ "object2", 23.0f });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadFloatArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Double)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><double key=\"object0\">2.5</double><double key=\"object1\">6.3</double><double key=\"object2\">23</double></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Double_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><double key=\"object0\">2.5</double><double key=\"object1\">6.3</double><double key=\"object2\">23</double></arraymap></HighLo>";
	std::map<HLString, double> expected;
	std::map<HLString, double> result;

	expected.insert({ "object0", 2.5 });
	expected.insert({ "object1", 6.3 });
	expected.insert({ "object2", 23.0 });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadDoubleArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Bool)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><bool key=\"object0\">1</bool><bool key=\"object1\">0</bool><bool key=\"object2\">1</bool></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Bool_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><bool key=\"object0\">1</bool><bool key=\"object1\">0</bool><bool key=\"object2\">1</bool></arraymap></HighLo>";
	std::map<HLString, bool> expected;
	std::map<HLString, bool> result;

	expected.insert({ "object0", true });
	expected.insert({ "object1", false });
	expected.insert({ "object2", true });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadBoolArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_String)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><string key=\"object0\">Hello</string><string key=\"object1\">world</string><string key=\"object2\">!</string></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_String_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><string key=\"object0\">Hello</string><string key=\"object1\">world</string><string key=\"object2\">!</string></arraymap></HighLo>";
	std::map<HLString, HLString> expected;
	std::map<HLString, HLString> result;

	expected.insert({ "object0", "Hello" });
	expected.insert({ "object1", "world" });
	expected.insert({ "object2", "!" });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadStringArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Vec2)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><vec2 key=\"object0\"><x>2.5</x><y>2</y></vec2><vec2 key=\"object1\"><x>6.3</x><y>4.5</y></vec2><vec2 key=\"object2\"><x>23</x><y>9.5</y></vec2></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Vec2_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><vec2 key=\"object0\"><x>2.5</x><y>2</y></vec2><vec2 key=\"object1\"><x>6.3</x><y>4.5</y></vec2><vec2 key=\"object2\"><x>23</x><y>9.5</y></vec2></arraymap></HighLo>";
	std::map<HLString, glm::vec2> expected;
	std::map<HLString, glm::vec2> result;

	expected.insert({ "object0", { 2.5f, 2.0f } });
	expected.insert({ "object1", { 6.3f, 4.5f } });
	expected.insert({ "object2", { 23.0f, 9.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec2ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Vec3)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><vec3 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z></vec3><vec3 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z></vec3><vec3 key=\"object2\"><x>23</x><y>9</y><z>5</z></vec3></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Vec3_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><vec3 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z></vec3><vec3 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z></vec3><vec3 key=\"object2\"><x>23</x><y>9</y><z>5</z></vec3></arraymap></HighLo>";
	std::map<HLString, glm::vec3> expected;
	std::map<HLString, glm::vec3> result;

	expected.insert({ "object0", { 2.5f, 2.0f, 2.9f } });
	expected.insert({ "object1", { 6.3f, 4.5f, 2.0f } });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.0f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec3ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Vec4)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><vec4 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z><w>6</w></vec4><vec4 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z><w>7.5</w></vec4><vec4 key=\"object2\"><x>23</x><y>9.5</y><z>5.5</z><w>1.5</w></vec4></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Vec4_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><vec4 key=\"object0\"><x>2.5</x><y>2</y><z>2.9</z><w>6</w></vec4><vec4 key=\"object1\"><x>6.3</x><y>4.5</y><z>2</z><w>7.5</w></vec4><vec4 key=\"object2\"><x>23</x><y>9.5</y><z>5.5</z><w>1.5</w></vec4></arraymap></HighLo>";
	std::map<HLString, glm::vec4> expected;
	std::map<HLString, glm::vec4> result;

	expected.insert({ "object0", { 2.5f, 2.0f, 2.9f, 6.0f } });
	expected.insert({ "object1", { 6.3f, 4.5f, 2.0f, 7.5f } });
	expected.insert({ "object2", { 23.0f, 9.5f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadVec4ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Mat2)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><mat2 key=\"object0\"><m00>0</m00><m01>0</m01><m10>0</m10><m11>0</m11></mat2><mat2 key=\"object1\"><m00>1</m00><m01>0</m01><m10>0</m10><m11>1</m11></mat2><mat2 key=\"object2\"><m00>23</m00><m01>9</m01><m10>5.5</m10><m11>1.5</m11></mat2></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Mat2_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><mat2 key=\"object0\"><m00>0</m00><m01>0</m01><m10>0</m10><m11>0</m11></mat2><mat2 key=\"object1\"><m00>1</m00><m01>0</m01><m10>0</m10><m11>1</m11></mat2><mat2 key=\"object2\"><m00>23</m00><m01>9</m01><m10>5.5</m10><m11>1.5</m11></mat2></arraymap></HighLo>";
	std::map<HLString, glm::mat2> expected;
	std::map<HLString, glm::mat2> result;

	expected.insert({ "object0", glm::mat2(0.0f) });
	expected.insert({ "object1", glm::mat2(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat2ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Mat3)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><mat3 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>0</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>0</m22></mat3><mat3 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>1</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>1</m22></mat3><mat3 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m10>1.5</m10><m11>6</m11><m12>1</m12><m20>7.5</m20><m21>0</m21><m22>4.5</m22></mat3></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Mat3_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><mat3 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>0</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>0</m22></mat3><mat3 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m10>0</m10><m11>1</m11><m12>0</m12><m20>0</m20><m21>0</m21><m22>1</m22></mat3><mat3 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m10>1.5</m10><m11>6</m11><m12>1</m12><m20>7.5</m20><m21>0</m21><m22>4.5</m22></mat3></arraymap></HighLo>";
	std::map<HLString, glm::mat3> expected;
	std::map<HLString, glm::mat3> result;

	expected.insert({ "object0", glm::mat3(0.0f) });
	expected.insert({ "object1", glm::mat3(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat3ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Mat4)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><mat4 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></mat4><mat4 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></mat4><mat4 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></mat4></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Mat4_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><mat4 key=\"object0\"><m00>0</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>0</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>0</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>0</m33></mat4><mat4 key=\"object1\"><m00>1</m00><m01>0</m01><m02>0</m02><m03>0</m03><m10>0</m10><m11>1</m11><m12>0</m12><m13>0</m13><m20>0</m20><m21>0</m21><m22>1</m22><m23>0</m23><m30>0</m30><m31>0</m31><m32>0</m32><m33>1</m33></mat4><mat4 key=\"object2\"><m00>23</m00><m01>9</m01><m02>5.5</m02><m03>1.5</m03><m10>6</m10><m11>1</m11><m12>7.5</m12><m13>0</m13><m20>4.5</m20><m21>10.5</m21><m22>1.1</m22><m23>5.32</m23><m30>3.2</m30><m31>1.5</m31><m32>5.3</m32><m33>4.8</m33></mat4></arraymap></HighLo>";
	std::map<HLString, glm::mat4> expected;
	std::map<HLString, glm::mat4> result;

	expected.insert({ "object0", glm::mat4(0.0f) });
	expected.insert({ "object1", glm::mat4(1.0f) });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f, 6.0f, 1.0f, 7.5f, 0.0f, 4.5f, 10.5f, 1.1f, 5.32f, 3.2f, 1.5f, 5.3f, 4.8f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadMat4ArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Quat)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap><quat key=\"object0\"><w>0</w><x>1</x><y>1</y><z>1</z></quat><quat key=\"object1\"><w>1</w><x>0</x><y>0</y><z>0</z></quat><quat key=\"object2\"><w>23</w><x>9</x><y>5.5</y><z>1.5</z></quat></arraymap></HighLo>";
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

TEST_F(XMLReadParserTests, XMLParser_Read_Complete_Array_Quat_With_Keys)
{
	HLString input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><HighLo version=\"1.0.0\"><arraymap key=\"arrayKeys\"><quat key=\"object0\"><w>0</w><x>1</x><y>1</y><z>1</z></quat><quat key=\"object1\"><w>1</w><x>0</x><y>0</y><z>0</z></quat><quat key=\"object2\"><w>23</w><x>9</x><y>5.5</y><z>1.5</z></quat></arraymap></HighLo>";
	std::map<HLString, glm::quat> expected;
	std::map<HLString, glm::quat> result;

	expected.insert({ "object0", { 0.0f, 1.0f, 1.0f, 1.0f } });
	expected.insert({ "object1", { 1.0f, 0.0f, 0.0f, 0.0f } });
	expected.insert({ "object2", { 23.0f, 9.0f, 5.5f, 1.5f } });

	Reader->SetContent(input);
	bool readSuccess = Reader->ReadQuatArrayMap("arrayKeys", result);

	EXPECT_EQ(readSuccess, true);
	EXPECT_EQ(MapEquals(result, expected), true);
}

