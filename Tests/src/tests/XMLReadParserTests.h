// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-08) initial release
//

#pragma once

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

struct XMLReadParserTests : public testing::Test
{
	Ref<DocumentReader> Reader;

	XMLReadParserTests()
	{
		Reader = DocumentReader::Create("", DocumentType::XML);
	}

	virtual ~XMLReadParserTests()
	{
	}
};

