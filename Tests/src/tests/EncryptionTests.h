// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-18) initial release
//

#pragma once

#undef TEST_CATEGORY
#define TEST_CATEGORY EncryptionTests

// Just example keys and ivs from https://asecuritysite.com/encryption/keygen
#define KEY "E4D49345E9AFD7A487B6C3117FD574AEF92A27B38D80879A294CD1D72608BEAC"
#define IV "1A92E5E9C29D0A005E4CCC0D94BE886A"

#include <HighLo.h>
#include <gtest/gtest.h>

using namespace highlo;

TEST(TEST_CATEGORY, EncryptionWithString)
{
	HLString plainText = "Hello World! This is a Test!";
	Encryptor enc(KEY, IV, EncryptionAlgorithm::AES_256_CBC);

	HLString encryptedStr = enc.Encrypt(plainText);
	HLString resultPlainText = enc.Decrypt(encryptedStr);

	EXPECT_EQ(strcmp(*plainText, *resultPlainText), 0);
}

TEST(TEST_CATEGORY, EncryptionWithBase64String)
{
	HLString plainText = "Hello World! This is a Test!";
	Encryptor enc(KEY, IV, EncryptionAlgorithm::AES_256_CBC);

	HLString encryptedStr = enc.EncryptBase64(plainText);
	HLString resultPlainText = enc.DecryptBase64(encryptedStr);

	EXPECT_EQ(strcmp(*plainText, *resultPlainText), 0);
}

