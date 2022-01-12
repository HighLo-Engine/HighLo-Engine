// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-12) initial release
//

#pragma once

#undef TEST_CATEGORY
#define TEST_CATEGORY JSONParserTests

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace highlo;

TEST(TEST_CATEGORY, JSONParser_String)
{
	HLString expected = "{\"test\":\"Hello World!\"}";

	Ref<DocumentWriter> writer = DocumentWriter::Create("", DocumentType::Json);
	writer->WriteString("test", "Hello World!");
	HLString content = writer->GetContent();
	EXPECT_EQ(StringEquals(expected, content), true);
}

