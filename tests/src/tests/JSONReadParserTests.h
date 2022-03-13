// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-08) initial release
//

#pragma once

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

struct JsonReadParserTests : public testing::Test
{
	Ref<DocumentReader> Reader;

	JsonReadParserTests()
	{
		Reader = DocumentReader::Create("", DocumentType::Json);
	}

	virtual ~JsonReadParserTests()
	{
	}
};

