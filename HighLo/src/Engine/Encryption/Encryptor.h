#pragma once

#include "Engine/Core/HLCore.h"
#include "Base64.h"

namespace highlo
{
	enum class EncryptionAlgorithm
	{
		None = 0,

		AES_128_CBC,
		AES_256_CBC,
		AES_192_CBC,

		AES_128_CCM,
		AES_256_CCM,
		AES_192_CCM,

		AES_128_CFB,
		AES_256_CFB,
		AES_192_CFB,


	};

	class Encryptor
	{
	public:

		HLAPI Encryptor(const HLString &key, const HLString &iv, EncryptionAlgorithm algorithm = EncryptionAlgorithm::AES_256_CBC);
		HLAPI ~Encryptor();

		HLAPI uint32 Encrypt(unsigned char *plainText, uint32 plainTextLength, unsigned char *cipherText);
		HLAPI uint32 Decrypt(unsigned char *cipherText, uint32 cipherTextLength, unsigned char *plainText);

		HLAPI HLString Encrypt(const HLString &plainText);
		HLAPI HLString Decrypt(const HLString &cipherText);

		HLAPI HLString EncryptBase64(const HLString &plainText);
		HLAPI HLString DecryptBase64(const HLString &cipherText);

	private:

		HLString m_Key;
		HLString m_IV;
		EncryptionAlgorithm m_Algorithm;
	};
}

