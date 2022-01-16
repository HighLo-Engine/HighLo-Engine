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

struct XMLWriteParserTests : public testing::Test
{
	Ref<DocumentWriter> Writer;

	XMLWriteParserTests()
	{
		Writer = DocumentWriter::Create("", DocumentType::XML);
	}

	virtual ~XMLWriteParserTests()
	{
	}
};

