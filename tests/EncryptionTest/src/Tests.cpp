#include "Tests.h"
using namespace highlo;

// Just example keys and ivs from https://asecuritysite.com/encryption/keygen
#define KEY "E4D49345E9AFD7A487B6C3117FD574AEF92A27B38D80879A294CD1D72608BEAC"
#define IV "1A92E5E9C29D0A005E4CCC0D94BE886A"

bool test_encryption()
{
	highloUnit::Timer timer("test_encryption_with_string");

	HLString plainText = "Hello World! This is a Test!";

	Encryptor enc(KEY, IV, EncryptionAlgorithm::AES_256_CBC);
	HLString encryptedStr = enc.Encrypt(plainText);
	HLString resultPlainText = enc.Decrypt(encryptedStr);

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *plainText, *resultPlainText);
}

bool test_encryption_with_base64()
{
	highloUnit::Timer timer("test_encryption_with_base64_string");

	HLString plainText = "Hello World! This is a Test!";

	Encryptor enc(KEY, IV, EncryptionAlgorithm::AES_256_CBC);
	HLString encryptedStr = enc.EncryptBase64(plainText);
	HLString resultPlainText = enc.DecryptBase64(encryptedStr);

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *plainText, *resultPlainText);
}

