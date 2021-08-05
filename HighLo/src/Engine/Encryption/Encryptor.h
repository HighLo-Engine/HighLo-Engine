#pragma once

#include "Base64.h"

namespace highlo
{
	class Encryptor
	{
	public:

		static uint32 Encrypt(unsigned char *plainText, uint32 plainTextLength, unsigned char *cipherText);
		static uint32 Decrypt(unsigned char *cipherText, uint32 cipherTextLength, unsigned char *plainText);

		static HLString Encrypt(const HLString &plainText);
		static HLString Decrypt(const HLString &cipherText);

		static HLString EncryptBase64(const HLString &plainText);
		static HLString DecryptBase64(const HLString &cipherText);
	};
}

