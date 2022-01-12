// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-12) initial release
//

#pragma once

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
	}

	virtual ~JsonReadParserTests()
	{
	}
};

