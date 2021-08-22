#include "Tests.h"
using namespace highlo;

// Just example keys and ivs from https://asecuritysite.com/encryption/keygen
#define KEY "E4D49345E9AFD7A487B6C3117FD574AEF92A27B38D80879A294CD1D72608BEAC"
#define IV "1A92E5E9C29D0A005E4CCC0D94BE886A"

bool test_encryption_raw()
{
	highloUnit::Timer timer("test_encryption_raw");

	char *plainText = "Hello World! This is a test!";
	uint32 plainTextLength = (uint32)strlen(plainText);

	char cipherText[256];
	uint32 cipherTextLength = 0;

	char resultPlainText[256];
	uint32 resultPlainTextLength = 0;

	Encryptor enc(KEY, IV, EncryptionAlgorithm::AES_256_CBC);
	cipherTextLength = enc.Encrypt((unsigned char*)plainText, plainTextLength, (unsigned char*)cipherText);
	resultPlainTextLength = enc.Decrypt((unsigned char*)cipherText, cipherTextLength, (unsigned char*)resultPlainText);

	highloUnit::Test test;
	return test.AssertEqual(timer, plainTextLength, resultPlainTextLength)
		&& test.AssertEqual<char*>(timer, plainText, resultPlainText);
}

bool test_encryption_with_string()
{
	highloUnit::Timer timer("test_encryption_with_string");

	HLString plainText = "Hello World! This is a Test!";

	Encryptor enc(KEY, IV, EncryptionAlgorithm::AES_256_CBC);
	HLString encryptedStr = enc.Encrypt(plainText);
	HLString resultPlainText = enc.Decrypt(encryptedStr);

	highloUnit::Test test;
	return test.AssertEqual(timer, *plainText, *resultPlainText);
}

bool test_encryption_with_base64_string()
{
	highloUnit::Timer timer("test_encryption_with_base64_string");

	HLString plainText = "Hello World! This is a Test!";

	Encryptor enc(KEY, IV, EncryptionAlgorithm::AES_256_CBC);
	HLString encryptedStr = enc.EncryptBase64(plainText);
	HLString resultPlainText = enc.DecryptBase64(encryptedStr);

	highloUnit::Test test;
	return test.AssertEqual(timer, *plainText, *resultPlainText);
}

